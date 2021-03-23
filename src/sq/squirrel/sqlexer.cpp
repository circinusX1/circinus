/*
    see copyright notice in squirrel.h
*/

#include <stdint.h>
#include "sqpcheader.h"
#include <ctype.h>
#include <stdlib.h>
#include "sqtable.h"
#include "sqstring.h"
#include "sqcompiler.h"
#include "sqlexer.h"

#define CUR_CHAR (_currdata)
#define RETURN_TOKEN(t) { _prevtoken = _curtoken; _curtoken = t; return t;}
#define IS_EOB() (CUR_CHAR <= SQUIRREL_EOB)
#define NEXT() {Next();_currentcolumn++;}
#define INIT_TEMP_STRING() { _longstr.resize(0);}
#define APPEND_CHAR(c) { _longstr.push_back(c);}
#define TERMINATE_BUFFER() {_longstr.push_back(_SC('\0'));}
#define ADD_KEYWORD(key,id) _keywords->NewSlot( SQString::Create(ss, _SC(#key)) ,isize_t(id))

SQLexer::SQLexer(){}
SQLexer::~SQLexer()
{
    _keywords->Release();
}

void SQLexer::Init(SQSharedState *ss, SQLEXREADFUNC rg, PVOID up,CompilerErrorMemb efunc,void *ed)
{
    _errfunc = efunc;
    _errtarget = ed;
    _sharedstate = ss;
    _keywords = SQTable::Create(ss, 37);
    ADD_KEYWORD(while, TK_WHILE);
    ADD_KEYWORD(do, TK_DO);
    ADD_KEYWORD(if, TK_IF);
    ADD_KEYWORD(else, TK_ELSE);
    ADD_KEYWORD(break, TK_BREAK);
    ADD_KEYWORD(continue, TK_CONTINUE);
    ADD_KEYWORD(return, TK_RETURN);
    ADD_KEYWORD(null, TK_NULL);
    ADD_KEYWORD(function, TK_FUNCTION);
    ADD_KEYWORD(local, TK_LOCAL);
    ADD_KEYWORD(int8_t,TK_INT8);     // mco start
    ADD_KEYWORD(uint8_t,TK_UINT8);
    ADD_KEYWORD(int16_t, TK_INT16);
    ADD_KEYWORD(uint16_t,TK_UINT16);
    ADD_KEYWORD(int32_t, TK_INT32);
    ADD_KEYWORD(uint16_t, TK_UINT32);
    ADD_KEYWORD(int64_t, TK_INT64);
    ADD_KEYWORD(uint64_t, TK_UINT64);
    ADD_KEYWORD(float, TK_REAL);
    ADD_KEYWORD(double, TK_DOUBLE); //  mco end
    ADD_KEYWORD(var, TK_LOCAL);// mco-mco
    ADD_KEYWORD(for, TK_FOR);
    ADD_KEYWORD(foreach, TK_FOREACH);
    ADD_KEYWORD(in, TK_IN);
    ADD_KEYWORD(typeof, TK_TYPEOF);
    ADD_KEYWORD(base, TK_BASE);
    ADD_KEYWORD(super, TK_BASE); // mco-mco
    ADD_KEYWORD(delete, TK_DELETE);
    ADD_KEYWORD(try, TK_TRY);
    ADD_KEYWORD(catch, TK_CATCH);
    ADD_KEYWORD(throw, TK_THROW);
    ADD_KEYWORD(clone, TK_CLONE);
    ADD_KEYWORD(yield, TK_YIELD);
    ADD_KEYWORD(resume, TK_RESUME);
    ADD_KEYWORD(switch, TK_SWITCH);
    ADD_KEYWORD(case, TK_CASE);
    ADD_KEYWORD(default, TK_DEFAULT);
    ADD_KEYWORD(this, TK_THIS);
    ADD_KEYWORD(class,TK_CLASS);
    ADD_KEYWORD(extends,TK_EXTENDS);
    ADD_KEYWORD(public,TK_EXTENDS); // mco-mco
    ADD_KEYWORD(constructor,TK_CONSTRUCTOR);
    ADD_KEYWORD(instanceof,TK_INSTANCEOF);
    ADD_KEYWORD(true,TK_TRUE);
    ADD_KEYWORD(false,TK_FALSE);
    ADD_KEYWORD(static,TK_STATIC);
    ADD_KEYWORD(enum,TK_ENUM);
    ADD_KEYWORD(const,TK_CONST);
    ADD_KEYWORD(__LINE__,TK___LINE__);
    ADD_KEYWORD(__FILE__,TK___FILE__);
    ADD_KEYWORD(rawcall, TK_RAWCALL);
    _readf = rg;
    _up = up;
    _lasttokenline = _currentline = 1;
    _currentcolumn = 0;
    _prevtoken = -1;
    _reached_eof = SQFalse;
    Next();
}

void SQLexer::Error(const SQChar *err)
{
    _errfunc(_errtarget,err);
}

void SQLexer::Next()
{
    isize_t t = _readf(_up);
    if(t > MAX_CHAR) Error(_SC("Invalid character"));
    if(t != 0) {
        _currdata = (LexChar)t;
        return;
    }
    _currdata = SQUIRREL_EOB;
    _reached_eof = SQTrue;
}

const SQChar *SQLexer::Tok2Str(isize_t tok)
{
    SQObjectPtr itr, key, val;
    isize_t nitr;
    while((nitr = _keywords->Next(false,itr, key, val)) != -1) {
        itr = (isize_t)nitr;
        if(((isize_t)_integer(val)) == tok)
            return _stringval(key);
    }
    return NULL;
}

void SQLexer::LexBlockComment()
{
    bool done = false;
    while(!done) {
        switch(CUR_CHAR) {
        case _SC('*'): { NEXT(); if(CUR_CHAR == _SC('/')) { done = true; NEXT(); }}; continue;
        case _SC('\n'): _currentline++; NEXT(); continue;
        case SQUIRREL_EOB: Error(_SC("missing \"*/\" in comment"));
            [[fallthrough]];
        default: NEXT();
        }
    }
}
void SQLexer::LexLineComment()
{
    do { NEXT(); } while (CUR_CHAR != _SC('\n') && (!IS_EOB()));
}
// mco-mco lex
isize_t SQLexer::Lex(isize_t inst_tok)
{
    _lasttokenline = _currentline;
    while(CUR_CHAR != SQUIRREL_EOB) {
        switch(CUR_CHAR)
        {
        case _SC('\t'):
        case _SC('\r'):
        case _SC(' '):
            NEXT(); continue;
        case _SC('\n'):
            _currentline++;
            _prevtoken=_curtoken;
            _curtoken=_SC('\n');
            NEXT();
            _currentcolumn=1;
            continue;
        case _SC('#'): LexLineComment(); continue;
        case _SC('/'):
            NEXT();
            switch(CUR_CHAR){
            case _SC('*'):
                NEXT();
                LexBlockComment();
                continue;
            case _SC('/'):
                LexLineComment();
                continue;
            case _SC('='):
                NEXT();
                RETURN_TOKEN(TK_DIVEQ);
                continue;
            case _SC('>'):
                NEXT();
                RETURN_TOKEN(TK_ATTR_CLOSE);
                continue;
            default:
                RETURN_TOKEN('/');
            }

        case _SC('='):
            NEXT();
            if (CUR_CHAR != _SC('=')){ RETURN_TOKEN('=') }
            else { NEXT(); RETURN_TOKEN(TK_EQ); }
        case _SC('<'):
            NEXT();
            switch(CUR_CHAR) {
            case _SC('='):
                NEXT();
                if(CUR_CHAR == _SC('>')) {
                    NEXT();
                    RETURN_TOKEN(TK_3WAYSCMP);
                }
                RETURN_TOKEN(TK_LE)
                        break;
            case _SC('-'):
                NEXT();
                RETURN_TOKEN(TK_NEWSLOT); break;
            case _SC('<'): NEXT(); RETURN_TOKEN(TK_SHIFTL); break;
            case _SC('/'): NEXT(); RETURN_TOKEN(TK_ATTR_OPEN); break;
            }
            RETURN_TOKEN('<');
        case _SC('>'):
            NEXT();
            if (CUR_CHAR == _SC('=')){ NEXT(); RETURN_TOKEN(TK_GE);}
            else if(CUR_CHAR == _SC('>')){
                NEXT();
                if(CUR_CHAR == _SC('>')){
                    NEXT();
                    RETURN_TOKEN(TK_USHIFTR);
                }
                RETURN_TOKEN(TK_SHIFTR);
            }
            else { RETURN_TOKEN('>') }
        case _SC('!'):
            NEXT();
            if (CUR_CHAR != _SC('=')){ RETURN_TOKEN('!')}
            else { NEXT(); RETURN_TOKEN(TK_NE); }
        case _SC('@'): {
            isize_t stype;
            NEXT();
            if(CUR_CHAR != _SC('"')) {
                RETURN_TOKEN('@');
            }
            if((stype=ReadString('"',true))!=-1) {
                RETURN_TOKEN(stype);
            }
            Error(_SC("error parsing the string"));
        }
            [[fallthrough]];
        case _SC('"'):
        case _SC('\''): {
            isize_t stype;
            if((stype=ReadString(CUR_CHAR,false))!=-1){
                RETURN_TOKEN(stype);
            }
            Error(_SC("error parsing the string"));
        }
            [[fallthrough]];
        case _SC('{'): case _SC('}'): case _SC('('): case _SC(')'): case _SC('['): case _SC(']'):
        case _SC(';'): case _SC(','): case _SC('?'): case _SC('^'): case _SC('~'):
        {
            isize_t ret = CUR_CHAR;
            NEXT(); RETURN_TOKEN(ret); }
        case _SC('.'):
            NEXT();
            if (CUR_CHAR != _SC('.')){ RETURN_TOKEN('.') }
            NEXT();
            if (CUR_CHAR != _SC('.')){ Error(_SC("invalid token '..'")); }
            NEXT();
            RETURN_TOKEN(TK_VARPARAMS);
        case _SC('&'):
            NEXT();
            if (CUR_CHAR != _SC('&')){ RETURN_TOKEN('&') }
            else { NEXT(); RETURN_TOKEN(TK_AND); }
        case _SC('|'):
            NEXT();
            if (CUR_CHAR != _SC('|')){ RETURN_TOKEN('|') }
            else { NEXT(); RETURN_TOKEN(TK_OR); }
        case _SC(':'):
            NEXT();
            if (CUR_CHAR == _SC('=')){ // mco-mco := instead stupid <-
                NEXT();
                RETURN_TOKEN(TK_NEWSLOT); break;
            }
            if (CUR_CHAR != _SC(':')){ RETURN_TOKEN(':') }
            else { NEXT(); RETURN_TOKEN(TK_DOUBLE_COLON); }
        case _SC('*'):
            NEXT();
            if (CUR_CHAR == _SC('=')){ NEXT(); RETURN_TOKEN(TK_MULEQ);}
            else RETURN_TOKEN('*');
        case _SC('%'):
            NEXT();
            if (CUR_CHAR == _SC('=')){ NEXT(); RETURN_TOKEN(TK_MODEQ);}
            else RETURN_TOKEN('%');
        case _SC('-'):
            NEXT();
            if (CUR_CHAR == _SC('=')){ NEXT(); RETURN_TOKEN(TK_MINUSEQ);}
            else if  (CUR_CHAR == _SC('-')){ NEXT(); RETURN_TOKEN(TK_MINUSMINUS);}
            else RETURN_TOKEN('-');
        case _SC('+'):
            NEXT();
            if (CUR_CHAR == _SC('=')){ NEXT(); RETURN_TOKEN(TK_PLUSEQ);}
            else if (CUR_CHAR == _SC('+')){ NEXT(); RETURN_TOKEN(TK_PLUSPLUS);}
            else RETURN_TOKEN('+');
        case SQUIRREL_EOB:
            return 0;
        default:{
            if (scisdigit(CUR_CHAR)) {
                isize_t ret = ReadNumber(inst_tok); //returns the type
                RETURN_TOKEN(ret);
            }
            else if (scisalpha(CUR_CHAR) || CUR_CHAR == _SC('_')) {
                isize_t t = ReadID();
                RETURN_TOKEN(t);
            }
            else {
                isize_t c = CUR_CHAR;
                if (sciscntrl((isize_t)c)) Error(_SC("unexpected character(control)"));
                NEXT();
                RETURN_TOKEN(c);
            }
            RETURN_TOKEN(0);
        }
        }
    }
    return 0;
}

isize_t SQLexer::GetIDType(const SQChar *s,isize_t len)
{
    SQObjectPtr t;
    if(_keywords->GetStr(s,len, t)) {
        return isize_t(_integer(t));
    }
    return TK_IDENTIFIER;
}

#ifdef SQUNICODE
#if WCHAR_SIZE == 2
isize_t SQLexer::AddUTF16(size_t ch)
{
    if (ch >= 0x10000)
    {
        size_t code = (ch - 0x10000);
        APPEND_CHAR((SQChar)(0xD800 | (code >> 10)));
        APPEND_CHAR((SQChar)(0xDC00 | (code & 0x3FF)));
        return 2;
    }
    else {
        APPEND_CHAR((SQChar)ch);
        return 1;
    }
}
#endif
#else
isize_t SQLexer::AddUTF8(size_t ch)
{
    if (ch < 0x80) {
        APPEND_CHAR((char)ch);
        return 1;
    }
    if (ch < 0x800) {
        APPEND_CHAR((SQChar)((ch >> 6) | 0xC0));
        APPEND_CHAR((SQChar)((ch & 0x3F) | 0x80));
        return 2;
    }
    if (ch < 0x10000) {
        APPEND_CHAR((SQChar)((ch >> 12) | 0xE0));
        APPEND_CHAR((SQChar)(((ch >> 6) & 0x3F) | 0x80));
        APPEND_CHAR((SQChar)((ch & 0x3F) | 0x80));
        return 3;
    }
    if (ch < 0x110000) {
        APPEND_CHAR((SQChar)((ch >> 18) | 0xF0));
        APPEND_CHAR((SQChar)(((ch >> 12) & 0x3F) | 0x80));
        APPEND_CHAR((SQChar)(((ch >> 6) & 0x3F) | 0x80));
        APPEND_CHAR((SQChar)((ch & 0x3F) | 0x80));
        return 4;
    }
    return 0;
}
#endif

isize_t SQLexer::ProcessStringHexEscape(SQChar *dest, isize_t maxdigits)
{
    NEXT();
    if (!isxdigit(CUR_CHAR)) Error(_SC("hexadecimal number expected"));
    isize_t n = 0;
    while (isxdigit(CUR_CHAR) && n < maxdigits) {
        dest[n] = CUR_CHAR;
        n++;
        NEXT();
    }
    dest[n] = 0;
    return n;
}

isize_t SQLexer::ReadString(isize_t ndelim,bool verbatim)
{
    INIT_TEMP_STRING();
    NEXT();
    if(IS_EOB()) return -1;
    for(;;) {
        while(CUR_CHAR != ndelim) {
            isize_t x = CUR_CHAR;
            switch (x) {
            case SQUIRREL_EOB:
                Error(_SC("unfinished string"));
                return -1;
            case _SC('\n'):
                if(!verbatim) Error(_SC("newline in a constant"));
                APPEND_CHAR(CUR_CHAR); NEXT();
                _currentline++;
                break;
            case _SC('\\'):
                if(verbatim) {
                    APPEND_CHAR('\\'); NEXT();
                }
                else {
                    NEXT();
                    switch(CUR_CHAR) {
                    case _SC('x'):  {
                        const isize_t maxdigits = sizeof(SQChar) * 2;
                        SQChar temp[maxdigits + 1];
                        ProcessStringHexEscape(temp, maxdigits);
                        SQChar *stemp;
                        APPEND_CHAR((SQChar)scstrtoul(temp, &stemp, 16));
                    }
                        break;
                    case _SC('U'):
                    case _SC('u'):  {
                        const isize_t maxdigits = CUR_CHAR == 'u' ? 4 : 8;
                        SQChar temp[8 + 1];
                        ProcessStringHexEscape(temp, maxdigits);
                        SQChar *stemp;
#ifdef SQUNICODE
#if WCHAR_SIZE == 2
                        AddUTF16(scstrtoul(temp, &stemp, 16));
#else
                        APPEND_CHAR((SQChar)scstrtoul(temp, &stemp, 16));
#endif
#else
                        AddUTF8(scstrtoul(temp, &stemp, 16));
#endif
                    }
                        break;
                    case _SC('t'): APPEND_CHAR(_SC('\t')); NEXT(); break;
                    case _SC('a'): APPEND_CHAR(_SC('\a')); NEXT(); break;
                    case _SC('b'): APPEND_CHAR(_SC('\b')); NEXT(); break;
                    case _SC('n'): APPEND_CHAR(_SC('\n')); NEXT(); break;
                    case _SC('r'): APPEND_CHAR(_SC('\r')); NEXT(); break;
                    case _SC('v'): APPEND_CHAR(_SC('\v')); NEXT(); break;
                    case _SC('f'): APPEND_CHAR(_SC('\f')); NEXT(); break;
                    case _SC('0'): APPEND_CHAR(_SC('\0')); NEXT(); break;
                    case _SC('\\'): APPEND_CHAR(_SC('\\')); NEXT(); break;
                    case _SC('"'): APPEND_CHAR(_SC('"')); NEXT(); break;
                    case _SC('\''): APPEND_CHAR(_SC('\'')); NEXT(); break;
                    default:
                        Error(_SC("unrecognised escaper char"));
                        break;
                    }
                }
                break;
            default:
                APPEND_CHAR(CUR_CHAR);
                NEXT();
            }
        }
        NEXT();
        if(verbatim && CUR_CHAR == '"') { //double quotation
            APPEND_CHAR(CUR_CHAR);
            NEXT();
        }
        else {
            break;
        }
    }
    TERMINATE_BUFFER();
    isize_t len = _longstr.size()-1;
    if(ndelim == _SC('\'')) {
        if(len == 0) Error(_SC("empty constant"));
        if(len > 1) Error(_SC("constant too long"));
        _nvalue = _longstr[0];
        return TK_INTEGER;
    }
    _svalue = &_longstr[0];
    return TK_STRING_LITERAL;
}

void LexHexadecimal(const SQChar *s,size_t *res)
{
    *res = 0;
    while(*s != 0)
    {
        if(scisdigit(*s))
            *res = ((*res)*16+((*s++)-'0'));
        else if(scisxdigit(*s))
            *res = ((*res)*16+(toupper(*s++)-'A'+10));
        else {
            assert(0);
        }
    }
}

void LexInteger(const SQChar *s,size_t *res)
{
    *res = 0;
    while(*s != 0)
    {
        *res = (*res)*10+((*s++)-'0');
    }
}

isize_t scisodigit(isize_t c) { return c >= _SC('0') && c <= _SC('7'); }

void LexOctal(const SQChar *s,size_t *res)
{
    *res = 0;
    while(*s != 0)
    {
        if(scisodigit(*s)) *res = (*res)*8+((*s++)-'0');
        else { assert(0); }
    }
}

isize_t isexponent(isize_t c) { return c == 'e' || c=='E'; }

#define MAX_HEX_DIGITS (sizeof(isize_t)*2)

isize_t SQLexer::ReadNumber(isize_t inst_tok)
{
#define TINT        1
#define TFLOAT      334
#define TDOUBLE     335
#define THEX        3
#define TSCIENTIFIC 4
#define TOCTAL      5
#define TU8         327//6
#define TI8         326//7
#define TU16        329//8
#define TI16        328//9
#define TU32        331//10
#define TI32        330//11


    isize_t type = TINT, firstchar = CUR_CHAR;
    SQChar *sTemp;
    INIT_TEMP_STRING();
    NEXT();
    //mco-mco
    if(firstchar == _SC('0') && ((CUR_CHAR) == _SC('x') ||
                                 (CUR_CHAR) == _SC('o') || // 1 suigned octet
                                 (CUR_CHAR) == _SC('c') || // 1 unsgined octet
                                 (CUR_CHAR) == _SC('s') || // signend 2 octates
                                 (CUR_CHAR) == _SC('w') || // unsigned 2 bytes
                                 (CUR_CHAR) == _SC('i') ||  //isize_t 32 signed
                                 (CUR_CHAR) == _SC('u') ||  // usigned isize_t
                                 scisodigit(CUR_CHAR)) )
    {
        if(scisodigit(CUR_CHAR))
        {
            type = TOCTAL;
            while(scisodigit(CUR_CHAR)) {
                APPEND_CHAR(CUR_CHAR);
                NEXT();
            }
            if(scisdigit(CUR_CHAR)) Error(_SC("invalid octal number"));
        }
        else
        {
            if(CUR_CHAR==_SC('x'))
                type = THEX;
            else if(CUR_CHAR==_SC('o'))
                type = TU8;
            else if(CUR_CHAR==_SC('c'))
                type = TI8;
            else if(CUR_CHAR==_SC('s'))
                type = TI16;
            else if(CUR_CHAR==_SC('w'))
                type = TU16;
            else if(CUR_CHAR==_SC('i'))
                type = TI32;
            else if(CUR_CHAR==_SC('u'))
                type = TU32;

            NEXT();
            while(isxdigit(CUR_CHAR))
            {
                APPEND_CHAR(CUR_CHAR);
                NEXT();
            }
            if(_longstr.size() > MAX_HEX_DIGITS) Error(_SC("too many digits for an Hex number"));
        }
    }
    else
    {

        if(inst_tok && inst_tok != TK_LOCAL)
        {
            type = inst_tok;
        }



        APPEND_CHAR((isize_t)firstchar);
        while (CUR_CHAR == _SC('.') || scisdigit(CUR_CHAR) || isexponent(CUR_CHAR)) {
            if(CUR_CHAR == _SC('.') || isexponent(CUR_CHAR)) type = TFLOAT;
            if(isexponent(CUR_CHAR)) {
                if(type != TFLOAT) Error(_SC("invalid numeric format"));
                type = TSCIENTIFIC;
                APPEND_CHAR(CUR_CHAR);
                NEXT();
                if(CUR_CHAR == '+' || CUR_CHAR == '-'){
                    APPEND_CHAR(CUR_CHAR);
                    NEXT();
                }
                if(!scisdigit(CUR_CHAR)) Error(_SC("exponent expected"));
            }

            APPEND_CHAR(CUR_CHAR);
            NEXT();
        }
    }
    TERMINATE_BUFFER();
    switch(type) {
    case TSCIENTIFIC:
    case TFLOAT:
        _fvalue = (SQFloat)scstrtod(&_longstr[0],&sTemp);
        return TK_FLOAT;
        //mco-mco types 0b 0c 0s 0w 0i 0u  0x(64) 0y(64)
    case TINT:
        LexInteger(&_longstr[0],(size_t *)&_nvalue);
        return TK_INTEGER;
    case TU8:
        LexHexadecimal(&_longstr[0],(size_t *)&_nvalue);
        _nvalue=(uint8_t)(_nvalue & 0xff);
        return TK_INTEGER;
    case TI8:
        LexHexadecimal(&_longstr[0],(size_t *)&_nvalue);
        _nvalue=(int8_t)(_nvalue & 0xff);
        return TK_INTEGER;
    case TU16:
        LexHexadecimal(&_longstr[0],(size_t *)&_nvalue);
        _nvalue=(uint16_t)(_nvalue & 0xffff);
        return TK_INTEGER;
    case TI16:
        LexHexadecimal(&_longstr[0],(size_t *)&_nvalue);
        _nvalue=(int16_t)(_nvalue & 0xffff);
        return TK_INTEGER;
    case TU32:
        LexHexadecimal(&_longstr[0],(size_t *)&_nvalue);
        _nvalue=(int32_t)(_nvalue & 0xffffffff);
        return TK_INTEGER;
    case TI32:
        LexHexadecimal(&_longstr[0],(size_t *)&_nvalue);
        _nvalue=(int32_t)(_nvalue & 0xffffffff);
        return TK_INTEGER;
    case THEX:
        LexHexadecimal(&_longstr[0],(size_t *)&_nvalue);
        return TK_INTEGER;
    case TOCTAL:
        LexOctal(&_longstr[0],(size_t *)&_nvalue);
        return TK_INTEGER;
    }
    return 0;
}

isize_t SQLexer::ReadID()
{
    isize_t res;
    INIT_TEMP_STRING();
    do {
        APPEND_CHAR(CUR_CHAR);
        NEXT();
    } while(scisalnum(CUR_CHAR) || CUR_CHAR == _SC('_'));
    TERMINATE_BUFFER();
    //mco return the type tk_u8
    res = GetIDType(&_longstr[0],_longstr.size() - 1);
    if(res == TK_IDENTIFIER || res == TK_CONSTRUCTOR) {
        _svalue = &_longstr[0];
    }
    return res;
}
