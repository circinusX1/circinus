#ifndef YSYSTEM_H
#define YSYSTEM_H

#include <string.h>
#include "../iper.h"


class EXPORT OsSys : public I_IDev
{
public:
    OsSys(const char* dev, const char* name);
    virtual ~OsSys();
    static void   squit(const char* name);
    ALL_VIRTUALS();

    const char* pwd();
    bool  cd(const char* wd);
private:
    
};

#endif // BMP180_H
