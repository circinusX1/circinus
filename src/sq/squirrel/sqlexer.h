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
    int Lex(int inst_tok=0);
    const SQChar *Tok2Str(int tok);
private:
    int GetIDType(const SQChar *s,int len);
    int ReadString(int ndelim,bool verbatim);
    int ReadNumber(int tok=0);
    void LexBlockComment();
    void LexLineComment();
    int ReadID();
    void Next();
#ifdef SQUNICODE
#if WCHAR_SIZE == 2
    int AddUTF16(size_t ch);
#endif
#else
    int AddUTF8(size_t ch);
#endif
    int ProcessStringHexEscape(SQChar *dest, int maxdigits);
    int _curtoken;
    SQTable *_keywords;
    bool _reached_eof;
public:
    int _prevtoken;
    int _currentline;
    int _lasttokenline;
    int _currentcolumn;
    const SQChar *_svalue;
    int _nvalue;
    SQFloat _fvalue;
    SQLEXREADFUNC _readf;
    PVOID _up;
    LexChar _currdata;
    SQSharedState *_sharedstate;
    sqvector<SQChar> _longstr;
    CompilerErrorMemb _errfunc;
    void *_errtarget;
    int _decl_type; // mco-mco
};

#endif
