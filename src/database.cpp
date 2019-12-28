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

#include <ctime>
#include <sys/stat.h>
#include <dlfcn.h>
#include "dlconfig.h"
#include "dcore.h"
#include "database.h"
#include "apis.h"

Database::Database(int limit, int clean, const char* name):_name(name),_db(nullptr),_limit(limit)
{
    // RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD
    std::string libr = ::execute("find /usr -name libsqlite*.so");
    if(libr.empty()) {

        LOGE("cannot find any sqlite library installed. Disabling the db feature");
        return;
    }
    libr.pop_back();
    _sohandler = ::dlopen(libr.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
    if(_sohandler==nullptr) {
        LOGE("cannot load " << libr <<":"<< dlerror() <<", disabling db feature");
        return;
    }
    fp_sqlite3_open         =  (p_sqlite3_open)::dlsym(_sohandler, "sqlite3_open");
    fp_sqlite3_close        =  (p_sqlite3_close)::dlsym(_sohandler, "sqlite3_close");
    fp_sqlite3_busy_timeout =  (p_sqlite3_busy_timeout)::dlsym(_sohandler, "sqlite3_busy_timeout");
    fp_sqlite3_exec         =  (p_sqlite3_exec)::dlsym(_sohandler, "sqlite3_exec");
    fp_sqlite3_free         =  (p_sqlite3_free)::dlsym(_sohandler, "sqlite3_free");
    fp_sqlite3_errmsg       =  (p_sqlite3_errmsg)::dlsym(_sohandler, "sqlite3_errmsg");
    fp_sqlite3_column_count =  (p_sqlite3_column_count)::dlsym(_sohandler, "sqlite3_column_count");
    fp_sqlite3_step         =  (p_sqlite3_step)::dlsym(_sohandler, "sqlite3_step");
    fp_sqlite3_column_text  =  (p_sqlite3_column_text)::dlsym(_sohandler, "sqlite3_column_text");
    fp_sqlite3_reset        =  (p_sqlite3_reset)::dlsym(_sohandler, "sqlite3_reset");
    fp_sqlite3_prepare      =  (p_sqlite3_prepare)::dlsym(_sohandler, "sqlite3_prepare");
    _maxrecs=limit;
    _dbdel=clean;
    LOGI(" -Database: " << _name <<
         " would have " << _maxrecs <<
         " records, resizes at :" << _dbdel);
    time ( &_rawtime );
    _dbage = time(0);
    _present = (fp_sqlite3_prepare != nullptr);
    Pdb = this;
}

Database::~Database()
{
    Pdb = 0;
    if(_db && fp_sqlite3_close)
        fp_sqlite3_close(_db);
    _db = 0;
    if(_sohandler)
        ::dlclose(_sohandler);
}

int Database::reset()
{
    return 0;
}

int Database::ctx_it(const char* nouse)
{
    if(!_present){ LOGW("db is disabled ") return false; }
    for(const auto& d: App->_devs)
    {
        if(d.second->get_data().length())
        {
            _dbable.push_back(d.second);
        }
    }
    if(_dbable.size()==0)
    {
        LOGW( __PRETTY_FUNCTION__ << "empty");
        return -1;
    }
    int err = fp_sqlite3_open(_name.c_str(), &_db);
    if(err!=SQLITE_OK)
    {
        LOGE( __PRETTY_FUNCTION__ <<" open " << _name << ", " << fp_sqlite3_errmsg(_db));
        return -1;
    }
    fp_sqlite3_busy_timeout(_db, 512);_toast(); fp_sqlite3_busy_timeout(_db, 512);
    for(auto& dev: _dbable)
    {
        char *szerr = nullptr;
        std::string sql =  "CREATE TABLE IF NOT EXISTS '";
        sql += dev->dev_key();
        sql += "' ("; sql += "RID integer primary key autoincrement,";
        for(size_t i=0;i<dev->get_data().length();i++)
        {
            sql += std::to_string(i); sql += " TEXT NOT NULL,";
        }
        sql += "DTIM DATETIME NOT NULL);";
        int rc = fp_sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
        if( rc != SQLITE_OK )
        {
            LOGE(  __PRETTY_FUNCTION__ << " SQL ERR: " << szerr);
            fp_sqlite3_free(szerr);
            err=-1;
            break;
        }
    }
    fp_sqlite3_close(_db);
    _db = 0;
    return err;
}


int Database::update()
{
    if(!_present){ LOGW("DB OFF ") return -1; }
    int    rv = 0;
    char  *szerr = 0;
    bool  hadvals=false;
    (void)hadvals;

    assert(_db==0);
    int err = fp_sqlite3_open(_name.c_str(), &_db);
    if(err){
        LOGE( __PRETTY_FUNCTION__ <<" CREATE " << _name << ", " << fp_sqlite3_errmsg(_db));
        return -1;
    }
    Clk.update();
    std::string sql,svls;
    std::string buffer = Clk.datetime();
    fp_sqlite3_busy_timeout(_db, 512);
    sql = "BEGIN;";
    err  = fp_sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
    if( err != SQLITE_OK )
    {
        LOGE(  __PRETTY_FUNCTION__ << " ERR: "  << szerr);
        fp_sqlite3_free(szerr);
        rv = -1;
        return rv;
    }

    for(auto& dev: _dbable)
    {
        svls.clear();
        sql = "INSERT INTO '"; sql += dev->dev_key(); sql += "' (";svls = "(";
        for(size_t i=0;i<dev->get_data().length();i++)
        {
            sql += "'"; svls += "'"; sql += std::to_string(i);
            svls += (const char*)dev->get_data().c_chars();
            sql += "'";  svls += "'";
            if(i<dev->get_data().length()-1)
            {
                sql+=","; svls+=",";
            }
        }
        sql+=",DTIM) VALUES"; sql+=svls; sql+=",'"; sql+=buffer; sql+= "');";
        err  = fp_sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
        if( err != SQLITE_OK )
        {
            LOGE( "SQL ERR: "  << szerr);
            fp_sqlite3_free(szerr);
            rv = -1;
            break;
        }
    }
    if(rv!=-1)
    {
        sql = "COMMIT;";
        ::msleep(2);
        err  = fp_sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
        if( err != SQLITE_OK )
        {
            LOGE(  __PRETTY_FUNCTION__ <<" SQL ERR: "  << szerr);
            fp_sqlite3_free(szerr);
            rv = -1;
        }
    }
    fp_sqlite3_close(_db);
    _db=0;
    return rv;
}

int Database::disk_sz()
{
    if(HddRoom < MIN_DRIVE_MEGS)
    {
        _resize();
        return 0;
    }
    return 1;
}

void Database::_toast()
{
    if(!_present){ LOGW("db is disabled ") return; }
    char *szerr = nullptr;
    std::string sql = "SELECT name FROM sqlite_master WHERE type='table';";
    sqlite3_stmt *statement;
    strarray_t drops;

    if ( fp_sqlite3_prepare(_db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        fp_sqlite3_column_count(statement);
        int res = 0;
        while ( 1 )
        {
            res = fp_sqlite3_step(statement);
            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                break;
            }
            if ( res == SQLITE_ROW )
            {
                std::string s = (char*)fp_sqlite3_column_text(statement, 0); //Name name
                if(s[0]=='_')
                {
                    LOGI(" a private table" << s);
                }
            }
        }
        fp_sqlite3_reset(statement);
    }

    for(const auto t: drops )
    {
        sql = "DROP TABLE '" + t + "';";
        int rc = fp_sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
        if( rc != SQLITE_OK )
        {
            LOGE(  __PRETTY_FUNCTION__ << " SQL ERR: " << szerr);
            fp_sqlite3_free(szerr);
            break;
        }
    }
}

int Database::save(const char* name)
{
    return 0;
}

int Database::_resize()
{
    if(!_present){ LOGW("DB OFF ") return -1; }
    char* szerr;
    int err = fp_sqlite3_open(_name.c_str(), &_db);
    if(err){
        LOGE("OPEN FAILS " << _name << ", " << fp_sqlite3_errmsg(_db));
        return -1;
    }
    LOGW("RESIZING: " << HddRoom << "Megas");
    // flush 60days to 45 days
    fp_sqlite3_busy_timeout(_db, 1024);
    for(auto& dev: _dbable)
    {
        std::string sql ;
        std::string  s;

        if(dev->dev_key()==nullptr)
            continue;
        sql = "DELETE FROM '"; sql += dev->dev_key();
        sql +="' WHERE RID IN (SELECT RID from '";
        sql += dev->dev_key(); sql += "' ORDER BY RID ASC LIMIT " + std::to_string(this->_dbdel);
        sql += ");"; std::cout << sql << "\n";
        int rc = fp_sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
        if( rc != SQLITE_OK )
        {
            LOGE( __PRETTY_FUNCTION__ <<" SQL ERR: " << szerr);
            fp_sqlite3_free(szerr);
        }
    }
    fp_sqlite3_close(_db);
    _db = 0;
    return 0;
}

