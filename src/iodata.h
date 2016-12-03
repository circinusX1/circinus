#ifndef IODATA_H
#define IODATA_H

#include <vector>
#include <algorithm>
#include "sqwrap.h"
#include "icombase.h"
#include "main.h"
#include "plugins.h"
#include "iodata.h"
class Comm;
typedef Comm* (*pCreCom)(const char*);
typedef void (*pDesCom)(Comm*);

#define MAX_QUEUE   65535

class IoData
{
    size_t _recsz;
public:
    IoData()
    {
        _recsz=MAX_QUEUE;
        _fromdevlen=0;
    }

    void setbuff(int sz)
    {
        _recsz=sz;
    }

    void reset()
    {
        AutoLock l(&_tmut);
        _out.clear();
        _fromdevq.clear();
        _fromdevlen = 0;
    }

    bstring lockDevToSend()
    {
        bstring out;
        AutoLock l(&_tmut);
        if(_out.length())
        {
            out.append(_out.data(), _out.length());
            _out.clear();
        }
        return out;
    }


    void lockDevWrite(cbyte* s, size_t len)
    {
        AutoLock l(&_tmut);
        _out.append(s,len);

        if(_out.length()>(size_t)_recsz){
            LOGERR("truncating output due was not sent");
            _out = _out.substr(_recsz/2);
        }
    }

    void lockDevWrite(const bstring& s)
    {
        lockDevWrite(s.data(), s.length());
    }

    void removefromDevq(const std::vector<size_t>& ids){
        AutoLock l(&_tmut);
        auto a = _fromdevq.begin();
        for(; a!=_fromdevq.end(); ++a)
        {
            if(std::find(ids.begin(), ids.end(), a->ptr()->_id) != ids.end())
            {
                LOGDETAIL("Removing received ID:" << a->ptr()->_id << " " << (const char*)a->ptr()->_data.data());
                _fromdevlen = a->ptr()->_data.length();
                _fromdevq.erase(a);
                break;
            }
        }
    }

    void trim(){
        AutoLock l(&_tmut);
        if(_fromdevq.size()>_recsz)
        {
            LOGDETAIL("trimiming rec Q due overflow")
            _fromdevq.pop_back();
        }
    }

    bstring getAllFromDev()
    {
        AutoLock l(&_tmut);
        bstring out;
        if(_fromdevq.size())
        {
            for(const auto &a : _fromdevq)
            {
                out.append(a->_data.data(), a->_data.length());
            }
            _fromdevlen = 0;
            _fromdevq.clear();
        }
        return out;
    }

    bstring copyAllFromDev()
    {
        AutoLock l(&_tmut);
        bstring out;
        if(_fromdevq.size())
        {
            for(const auto &a : _fromdevq)
            {
                out.append(a->_data.data(), a->_data.length());
            }
        }
        return out;
    }

    size_t  fromDevDataLen()const
    {
        AutoLock    l(&_tmut);
        return   _fromdevlen;
    }

    size_t  dataToDev()const
    {
        AutoLock l(&_tmut);
        return _out.length();
    }

    void accumDevData(const CntPtr<DevMsg>& msg);

private:
    std::deque<CntPtr<DevMsg> > _fromdevq;
    size_t                      _fromdevlen;
    bstring                     _out;
    umutex                      _tmut;
    time_t                      _timein;
};

#endif // IODATA_H

