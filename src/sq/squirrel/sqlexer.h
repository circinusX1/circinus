/*  see copyright notice in squirrel.h */
#ifndef _SQLEXER_H_
#define _SQLEXER_H_

#ifdef SQUNICODE
typedef SQChar LexChar;
#else
typedef unsigned char LexChar;
#endif

struct SQLexer
{
    SQLexer();
    ~SQLexer();
    void Init(SQSharedState *ss,SQLEXREADFUNC rg,PVOID up,CompilerErrorMemb efunc,void *ed);
    void Error(const SQChar *err);
    isize_t Lex(isize_t inst_tok=0);
    const SQChar *Tok2Str(isize_t tok);
private:
    isize_t GetIDType(const SQChar *s,isize_t len);
    isize_t ReadString(isize_t ndelim,bool verbatim);
    isize_t ReadNumber(isize_t tok=0);
    void LexBlockComment();
    void LexLineComment();
    isize_t ReadID();
    void Next();
#ifdef SQUNICODE
#if WCHAR_SIZE == 2
    isize_t AddUTF16(size_t ch);
#endif
#else
    isize_t AddUTF8(size_t ch);
#endif
    isize_t ProcessStringHexEscape(SQChar *dest, isize_t maxdigits);
    isize_t _curtoken;
    SQTable *_keywords;
    bool _reached_eof;
public:
    isize_t _prevtoken;
    isize_t _currentline;
    isize_t _lasttokenline;
    isize_t _currentcolumn;
    const SQChar *_svalue;
    isize_t _nvalue;
    SQFloat _fvalue;
    SQLEXREADFUNC _readf;
    PVOID _up;
    LexChar _currdata;
    SQSharedState *_sharedstate;
    sqvector<SQChar> _longstr;
    CompilerErrorMemb _errfunc;
    void *_errtarget;
    isize_t _decl_type; // mco-mco
};

#endif
