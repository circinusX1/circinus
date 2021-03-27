//
// SqratMemberMethods: Member Methods
//

//
// Copyright (c) 2009 Brandon Jones
// Copyright 2011 Li-Cheng (Andy) Tai
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#if !defined(_SCRAT_THREAD_H_)
#define _SCRAT_THREAD_H_

#include <squirrel.h>
#include <mutex>
#include <condition_variable>
#include "sq_import.h"
#ifdef WITH_MAIN_MUTEX
#include "osthread.h"

struct  BaseSqEnvi
{
    std::mutex              mutex_;
    std::condition_variable condvar_;
    std::atomic<bool>       ready_;
    std::atomic<bool>       snap_;
    void let_thread_go(){
        snap_  = true;
        ready_ = false;
    }
    bool snapped()const{return ready_.load()==false;}
    void notify(){
        ready_ = true;
        condvar_.notify_all();
    }
};

#define LOCK_SCOPE(x_)    const std::unique_lock<std::mutex> x_(__bsqenv->mutex_);

class  sq_env;
extern sq_env*		__sqenv;          // squirrel interpreter enviroment object
extern BaseSqEnvi*	__bsqenv;

#define APP_LOCK(_x)     const std::lock_guard<std::mutex> x_(__bsqenv->mutex_);
#else
#define APP_LOCK(x)
#endif // WITH_MAIN_MUTEX


#endif
