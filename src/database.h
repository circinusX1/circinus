/*
Copyright (c) 2014-2016 comarius.DPT All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/comarius. The name of the
https://github.com/comarius may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/


#ifndef SQLITE_H
#define SQLITE_H

#include "main.h"
#include <vector>
#include <sqlite3.h>

#ifndef SQLITE_API
# define SQLITE_API
#endif
/**
 * @brief The Database class
 * database class.
 */
class Database
{
    typedef SQLITE_API int (*p_sqlite3_open)(
      const char *filename,   /* Database filename (UTF-8) */
      sqlite3 **ppDb          /* OUT: SQLite db handle */
    );
    typedef SQLITE_API int (*p_sqlite3_close)(sqlite3*);
    typedef SQLITE_API int (*p_sqlite3_busy_timeout)(sqlite3*, int ms);
    typedef SQLITE_API int (*p_sqlite3_prepare)(
      sqlite3 *db,            /* Database handle */
      const char *zSql,       /* SQL statement, UTF-8 encoded */
      int nByte,              /* Maximum length of zSql in bytes. */
      sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
      const char **pzTail     /* OUT: Pointer to unused portion of zSql */
    );
    typedef SQLITE_API int (*p_sqlite3_exec)(
      sqlite3*,                                  /* An open database */
      const char *sql,                           /* SQL to be evaluated */
      int (*callback)(void*,int,char**,char**),  /* Callback function */
      void *,                                    /* 1st argument to callback */
      char **errmsg                              /* Error msg written here */
    );
    typedef SQLITE_API void (*p_sqlite3_free)(void*);
    typedef SQLITE_API const char *(*p_sqlite3_errmsg)(sqlite3*);
    typedef SQLITE_API int (*p_sqlite3_column_count)(sqlite3_stmt *pStmt);
    typedef SQLITE_API int (*p_sqlite3_step)(sqlite3_stmt*);
    typedef SQLITE_API const unsigned char *(*p_sqlite3_column_text)(sqlite3_stmt*, int iCol);
    typedef SQLITE_API int (*p_sqlite3_reset)(sqlite3_stmt *pStmt);

public:
    Database(int limit, int clean, const char* name);
    virtual ~Database();

    int watch(const char* devname);
    int update();
    int ctx_it(const char* na);
    int save(const char* name);
    int reset();
    int disk_sz();
    //int add_table(Sqrat::Function& f);
    void ex_fld(const char* fld);
    const char* filename()const{return _name.c_str();}
    const std::string& name()const{return _name;}


    static void squit(SqEnvi& e){
        Sqrat::Class<Database> cls(e.theVM(), _SC("DB"));
        cls.Ctor<int, int, const char*>();
        cls.Functor(_SC("ctx_it"), &Database::ctx_it);
        cls.Functor(_SC("update"), &Database::update);
        cls.Functor(_SC("save"), &Database::save);
        cls.Functor(_SC("reset"), &Database::reset);
        cls.Functor(_SC("to_space"), &Database::disk_sz);
        Sqrat::RootTable().Bind(_SC("DB"), cls);
    }

private:
    int _resize();
    void _toast();

private:
    std::vector<I_IDev*> _dbable;
    int          _columns = 0;
    int          _maxrecs=72000;
    int          _preverror=0;
    int          _dbdel=7200;
    int          _resizecheck=0;
    time_t       _rawtime;
    size_t       _dbage;
    std::string  _name;
    sqlite3     *_db = nullptr;
    int          _limit;
    bool         _present = false;
    p_sqlite3_open           fp_sqlite3_open;
    p_sqlite3_close          fp_sqlite3_close;
    p_sqlite3_busy_timeout   fp_sqlite3_busy_timeout;
    p_sqlite3_exec           fp_sqlite3_exec;
    p_sqlite3_free           fp_sqlite3_free;
    p_sqlite3_errmsg         fp_sqlite3_errmsg;
    p_sqlite3_column_count   fp_sqlite3_column_count;
    p_sqlite3_prepare        fp_sqlite3_prepare;
    p_sqlite3_step           fp_sqlite3_step;
    p_sqlite3_column_text    fp_sqlite3_column_text;
    p_sqlite3_reset          fp_sqlite3_reset;
    void                     *_sohandler;
};




#endif // SQLITE_H
