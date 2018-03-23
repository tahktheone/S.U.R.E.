#ifndef SURELOG_H
#define SURELOG_H

#include <CL/cl.h>
#include <iostream>
#include <stdio.h>
#include <cwchar>

class SureLog
{
    public:
        SureLog(const char *prefix);
        SureLog(const SureLog&);
        SureLog *operator=(const SureLog&);
        virtual ~SureLog();
        void AddLine(const char *);
        void AddOCLError(int i_ret, const char *);
        void AddBigText(const char *,const char *);

    protected:
    private:
        char prefix[20];
        void GetOpenCLErrorText(int code,char *result);
        FILE *f;
};

#endif // SURELOG_H
