

#include "sqlitedb.h"

#define BOGUS_NO    999999999

SqliteDb::SqliteDb(int limit, const char* name):_name(name),_db(nullptr),_limit(limit)
{
    if(RunCtx::Pdb){
        LOGE("cannot have 2 db's. Exiting");
        exit(0);
    }
    _maxrecs=std::stoi((const char*)bblconfig(CONFIGF,"dbmax","8000"));       // max records in db
    _dbdel=std::stoi((const char*)bblconfig(CONFIGF,"dbdel","800"));          // how many to delete
    _dbfile = (const char*)bblconfig(CONFIGF,"dbfile","/usr/share/s2w/s2w.db");
    RunCtx::ensureDir(_dbfile);
    if(RunCtx::System=="amd64")
    {
        ::unlink(_dbfile.c_str());
    }
    RunCtx::Pdb = this;
}

SqliteDb::~SqliteDb()
{
    RunCtx::Pdb = 0;
    if(_db)
        sqlite3_close(_db);
    _db = 0;

}


int SqliteDb::open()
{
    int err = sqlite3_open(_dbfile.c_str(), &_db);
    if(err)
    {
        GLOGE("Cannot create database " << _dbfile << ", " << sqlite3_errmsg(_db));
    }
    else
    {
        char *szerr = 0;
        std::string sql =  "CREATE TABLE IF NOT EXISTS NameReading (";
                    sql += "Id integer primary key autoincrement,";
                    sql += "Device         TEXT    NOT NULL,";
                    sql += "ReadingJSON  TEXT    NOT NULL,";
                    sql += "DateTime DATETIME    NOT NULL);";


        GLOGD(sql);
        int rc = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);

        if( rc != SQLITE_OK )
        {
            GLOGE( "SQL error: " << szerr);
            sqlite3_free(szerr);
        }

        sql =  "CREATE TABLE IF NOT EXISTS LastGoodState (";
                    sql += "Key          TEXT    NOT NULL,";
                    sql += "UserValue    TEXT    NOT NULL,";
                    sql += "DateTime DATETIME    NOT NULL);";

        GLOGD(sql);
        rc = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);

        if( rc != SQLITE_OK )
        {
            GLOGE( "SQL error: " << szerr);
            sqlite3_free(szerr);
        }
        sqlite3_close(_db);
        _db = 0;
    }

    return err;
}

int SqliteDb::watch(const char* devDevice)
{
    if(!::strncmp(devDevice, "(n", 2))
    {
        _pdevs.clear();
        for(auto& a: PCTX->_devs)
        {
           _pdevs.push_back(a.second);
        }
        return 0;
    }
    if(PCTX->_devs.find(devDevice)!=PCTX->_devs.end())
    {
        _pdevs.push_back(PCTX->_devs[devDevice]);
        return 0;
    }
    GLOGW("Db cannot watch " << devDevice);
    return -1;
}

int SqliteDb::getValue(const char* Device, std::string& outvalues)
{
    int err = sqlite3_open(_dbfile.c_str(), &_db);
    if(err){
        GLOGE("Cannot create database " << _dbfile << ", " << sqlite3_errmsg(_db));
        return -1;
    }
    std::string sql = "SELECT UserValue FROM LastGoodState WHERE Key='Device' ORDER BY id DESC LIMIT 1";
    sqlite3_stmt *statement;
    std::string  s;

    if ( sqlite3_prepare(_db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        sqlite3_column_count(statement);
        sqlite3_step(statement);
        outvalues = (char*)sqlite3_column_text(statement, 0); //ts
        sqlite3_reset(statement);
    }
    sqlite3_close(_db);
    _db=0;
    return 0;
}

int SqliteDb::insertRecord(const char* Device, const char* record)
{
    char* szerr;

    int err = sqlite3_open(_dbfile.c_str(), &_db);
    if(err){
        GLOGE("Cannot open database " << _dbfile << ", " << sqlite3_errmsg(_db));
        return -1;
    }

    time_t rawtime;
    struct tm * timeinfo;
    char buffer[64];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(buffer, sizeof(buffer)-1, "%Y-%m-%d %H:%M:%S", timeinfo);


    std::string sql  = "INSERT INTO NameReading (Device, ReadingJSON, DateTime) VALUES(";
    sql += "'";  sql += Device; sql += "',";
    sql += "'";
    sql += "\"";
    sql += Device;
    sql += "\":\"";
    sql += record;
    sql += "\"',";
    sql += "'";
    sql += buffer;
    sql += "'";
    sql += ");";


    GLOGD(sql)
    err  = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
    if( err != SQLITE_OK )
    {
        GLOGE( "SQL error: "  << szerr);
        sqlite3_free(szerr);
    }
     _userdbs[Device]=true;
    sqlite3_close(_db);
    _db=0;
    return err;
}

int SqliteDb::setValue(const char* name, const char* values)
{
    int err = sqlite3_open(_dbfile.c_str(), &_db);
    if(err){
        GLOGE("Cannot open database " << _dbfile << ", " << sqlite3_errmsg(_db));
        return -1;
    }

    std::string sql = "DELETE FROM LastGoodState WHERE key='";
                sql += name;
                sql += "';";
    char *szerr = 0;

    err  = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
    if( err != SQLITE_OK )
    {
        GLOGE( "SQL error: "  << szerr);
        sqlite3_free(szerr);
        sqlite3_close(_db);
        _db=0;
        return err;
    }
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[64];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(buffer, sizeof(buffer)-1, "%Y-%m-%d %H:%M:%S", timeinfo);


    sql = "INSERT INTO LastGoodState (Key,UserValue,DateTime) VALUES(";
    sql += "'"; sql += name; sql += "',";
    sql += "'"; sql += values;  sql += "',";
    sql += "'"; sql += buffer;  sql += "'";
    sql += ");";

  //  GLOGX(sql);
    err  = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
    if( err != SQLITE_OK )
    {
        GLOGE( "SQL error: "  << szerr);
        sqlite3_free(szerr);
    }
    sqlite3_close(_db);
    _db=0;
    return err;
}

int SqliteDb::update()
{
    int  rv = 0;
    char *szerr = 0;

    if(RunCtx::RunCtx::_diskFree < 300)
    {
        resize();
        return 0;
    }

    assert(_db==0);
    int err = sqlite3_open(_dbfile.c_str(), &_db);
    if(err){
        GLOGE("Cannot create database " << _dbfile << ", " << sqlite3_errmsg(_db));
        return -1;
    }
    sqlite3_busy_timeout(_db, 128);

    time_t rawtime;
    struct tm * timeinfo;
    char buffer[64];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(buffer, sizeof(buffer)-1, "%Y-%m-%d %H:%M:%S", timeinfo);

    std::string sql = "BEGIN;";
    err  = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);

    if( err != SQLITE_OK )
    {
        GLOGE( "SQL error: "  << szerr);
        sqlite3_free(szerr);
        rv = -1;
        sqlite3_close(_db);
        _db = 0;
        return rv;
    }


    for(auto& a : _pdevs)
    {
        IDevice* pd = a;

        std::string dv = pd->getJson();
        if(dv.empty())
        {
            dv = pd->_jstart(pd->name());
            dv += pd->_jsonize("bus", pd->bus()) + ",";
            dv += pd->_jsonize("value", BOGUS_NO);
            dv += pd->_jend();

        }
        sql = "INSERT INTO NameReading (Device,ReadingJSON,DateTime) VALUES(";
        sql += "'";  sql += pd->name(); sql += "',";
        sql += "'"; sql += dv; sql += "',";
        sql += "'"; sql += buffer; sql += "'";
        sql += ");";


     //   GLOGX(sql);
        int err  = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);

        if( err != SQLITE_OK )
        {
            GLOGE( "SQL error: "  << szerr);
            sqlite3_free(szerr);
            rv = -1;
            break;
        }
    }

    sql = "COMMIT;";
    ::msleep(128);
    err  = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
    if( err != SQLITE_OK )
    {
        GLOGE( "SQL error: "  << szerr);
        sqlite3_free(szerr);
        rv = -1;
    }
    sqlite3_close(_db);
    _db=0;
    return rv;
}


/**
 * @brief SqliteDb::querry
 * @param what              mydb/digit   from limi
 *                          mydb/device,device,device/from
 * @param content
 * @return
 */
int SqliteDb::querry(const char* what, std::string& content)
{
    std::string swhat = what;
    std::vector<std::string> params;
    std::string timer[2];
    std::string sql = "SELECT * FROM NameReading ";
    std::string crit;

    bblconfig::split(swhat, '&', params);
    if(params.size()>1)
    {
        if(params[1]=="reset")
        {
            this->reset();
            content = "{\"err\":0}";
            return 0;
        }
    }
    std::vector<std::string> kv;
    std::vector<std::string> rg;

    std::string whereas;
    size_t pc = 0;
    int to = _limit, frm=-1;
    size_t els = params.size();
    if(els)
    {
        for(auto& a : params)
        {
            if(pc==0){
                ++pc;
                continue;
            }
            if(!std::isdigit(a[0]))
            {

                std::string aa = a + ",";
                bblconfig::split(a,',',rg);
                size_t      els = rg.size();
                size_t      el = 0;
                for(const auto& ae : rg)
                {
                    whereas += " Device";
                    whereas += "='";
                    whereas += ae;
                    whereas += "'";
                    if(el < els-1)
                    {
                        whereas += " OR ";
                    }
                    ++el;
                }
            }
            else
            {
                std::string aa = a + ",";
                bblconfig::split(a,',',rg);
                //size_t      els = rg.size();
                frm = std::stoi(rg[0]);
                if(rg.size()>1 && !rg[1].empty())
                    to = std::stoi(rg[1]);
            }
        }
    }
    if(!whereas.empty())
    {
        crit += " WHERE ";
        crit += whereas;
    }

    crit += " ORDER BY Id DESC LIMIT ";
    if(frm==-1)frm=0;
    crit += std::to_string(frm);
    crit += ",";
    crit += std::to_string(to);
    sql += crit + ";";

    GLOGX(sql);
    int err = sqlite3_open(_dbfile.c_str(), &_db);
    if(err){
        GLOGE("Cannot open database " << _dbfile << ", " << sqlite3_errmsg(_db));
        return -1;
    }

    sqlite3_stmt *statement;
    std::string  s;

//    int recs = 0;
    if ( sqlite3_prepare(_db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        std::string dname;
        sqlite3_column_count(statement);
        int res = 0;
        content.append("{\"recs\":[");
        int records = to;
        while ( 1 )
        {
            res = sqlite3_step(statement);
            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                break;
            }

            if ( res == SQLITE_ROW )
            {
                content.append("{");
                    s = (char*)sqlite3_column_text(statement, 1); //Name name
                    dname = s;
                    content.append("\"");
                    content.append(s);
                    content.append("\":{");

                    s = (char*)sqlite3_column_text(statement, 3); //ts
                    content.append("\"time\":\"");
                    content.append(s);
                    content.append("\"");

                    s = (char*)sqlite3_column_text(statement, 2); //Name values
                    if(!s.empty()){
                        content.append(",");
                        content.append(s);
                    }
                    content.append("}");
                content.append("},");
                --records;
            }

        }
        sqlite3_reset(statement);
        if(records==0)
            content.append("{\"next\":1} ] }");
        content.append("{\"next\":0} ] }");
    }
    else
    {
        GLOGE("Cannot select  " << sqlite3_errmsg(_db));

    }
    GLOGX(content);
    sqlite3_close(_db);
    _db = 0;
    return 0;
}


int SqliteDb::save(const char* name)
{
    return 0;
}

int SqliteDb::resize()
{
    char* szerr;
    int err = sqlite3_open(_dbfile.c_str(), &_db);
    if(err){
        GLOGE("Cannot create database " << _dbfile << ", " << sqlite3_errmsg(_db));
        return -1;
    }
    GLOGW("deleting older records, due HDD left free space " << RunCtx::_diskFree << "...");
    // flush 60days to 45 days
    std::string sql = "SELECT COUNT(*) FROM NameReading;";
    sqlite3_stmt *statement;
    std::string  s;

    if ( sqlite3_prepare(_db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        sqlite3_column_count(statement);
        sqlite3_step(statement);
        s = (char*)sqlite3_column_text(statement, 0); //ts
        int recs = std::stoi(s);
        sqlite3_reset(statement);


        if(recs > _maxrecs)
        {
            sql = "DELETE FROM NameReading WHERE Id IN (SELECT Id FROM NameReading ORDER BY Id  LIMIT ";
            sql += std::to_string(_dbdel);
            sql += ";";
            int rc = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);
            if( rc != SQLITE_OK )
            {
                GLOGE( "SQL error: " << szerr);
                sqlite3_free(szerr);
            }
        }
    }
    sqlite3_close(_db);
    _db = 0;
    return 0;
}

int SqliteDb::reset()
{
    char *szerr = 0;
    int err = sqlite3_open(_dbfile.c_str(), &_db);
    if(err){
        GLOGE("Cannot create database " << _dbfile << ", " << sqlite3_errmsg(_db));
        return err;
    }
    std::string sql = "DELETE FROM NameReading;";
    GLOGD(sql);
    err  = sqlite3_exec(_db, sql.c_str(), 0, 0, &szerr);

    if( err != SQLITE_OK )
    {
        GLOGE( "SQL error: "  << szerr);
        sqlite3_free(szerr);
    }
    sqlite3_close(_db);
    _db = 0;
    return err;
}

void SqliteDb::fillUserFields(std::string& us)
{
    for(const auto& e : _userdbs)
    {
        us.append("\"");
        us.append(e.first);
        us.append("\"");
        us.append(",");
    }

}
