#include "SureLog.h"
#include <time.h>

SureLog::SureLog(const char *i_prefix)
{
    char fname[100];
    sprintf(prefix,"%s",i_prefix);
    sprintf(fname,"./%s.log",prefix);
    f = fopen(fname,"w");
    AddLine("Старт");
}

SureLog::~SureLog()
{
    AddLine("Финиш");
    fclose(f);
}

void SureLog::AddLine(const char *text)
{
    char StrTime[100];
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    strftime(StrTime,100,"%d.%m.%Y %H:%M:%S",timeinfo);
    fprintf(f,"%s %s\n",StrTime,text);
};

void SureLog::AddOCLError(int i_ret, const char *text)
{
    char cl_err[100];
    char line[300];
    GetOpenCLErrorText(i_ret,cl_err);
    sprintf(line,"%s: %s",text,cl_err);
    AddLine(line);
};

void SureLog::AddBigText(const char *comment,const char *text)
{
    AddLine(comment);
    fprintf(f,"%s\n",text);
};

void SureLog::GetOpenCLErrorText(int i_ret,char *result)
{
    char cl_err[50];
    switch(i_ret)
    {
        case  CL_SUCCESS: sprintf(cl_err,"CL_SUCCESS");break;
        case  CL_DEVICE_NOT_FOUND: sprintf(cl_err,"CL_DEVICE_NOT_FOUND");break;
        case  CL_DEVICE_NOT_AVAILABLE: sprintf(cl_err,"CL_DEVICE_NOT_AVAILABLE");break;
        case  CL_COMPILER_NOT_AVAILABLE: sprintf(cl_err,"CL_COMPILER_NOT_AVAILABLE");break;
        case  CL_MEM_OBJECT_ALLOCATION_FAILURE: sprintf(cl_err,"CL_MEM_OBJECT_ALLOCATION_FAILURE");break;
        case  CL_OUT_OF_RESOURCES: sprintf(cl_err,"CL_OUT_OF_RESOURCES");break;
        case  CL_OUT_OF_HOST_MEMORY: sprintf(cl_err,"CL_OUT_OF_HOST_MEMORY");break;
        case  CL_PROFILING_INFO_NOT_AVAILABLE: sprintf(cl_err,"CL_PROFILING_INFO_NOT_AVAILABLE");break;
        case  CL_MEM_COPY_OVERLAP: sprintf(cl_err,"CL_MEM_COPY_OVERLAP");break;
        case  CL_IMAGE_FORMAT_MISMATCH: sprintf(cl_err,"CL_IMAGE_FORMAT_MISMATCH");break;
        case  CL_IMAGE_FORMAT_NOT_SUPPORTED: sprintf(cl_err,"CL_IMAGE_FORMAT_NOT_SUPPORTED");break;
        case  CL_BUILD_PROGRAM_FAILURE: sprintf(cl_err,"CL_BUILD_PROGRAM_FAILURE");break;
        case  CL_MAP_FAILURE: sprintf(cl_err,"CL_MAP_FAILURE");break;
        case  CL_MISALIGNED_SUB_BUFFER_OFFSET: sprintf(cl_err,"CL_MISALIGNED_SUB_BUFFER_OFFSET");break;
        case  CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: sprintf(cl_err,"CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST");break;
        case  CL_COMPILE_PROGRAM_FAILURE: sprintf(cl_err,"CL_COMPILE_PROGRAM_FAILURE");break;
        case  CL_LINKER_NOT_AVAILABLE: sprintf(cl_err,"CL_LINKER_NOT_AVAILABLE");break;
        case  CL_LINK_PROGRAM_FAILURE: sprintf(cl_err,"CL_LINK_PROGRAM_FAILURE");break;
        case  CL_DEVICE_PARTITION_FAILED: sprintf(cl_err,"CL_DEVICE_PARTITION_FAILED");break;
        case  CL_KERNEL_ARG_INFO_NOT_AVAILABLE: sprintf(cl_err,"CL_KERNEL_ARG_INFO_NOT_AVAILABLE");break;
        case  CL_INVALID_VALUE: sprintf(cl_err,"CL_INVALID_VALUE");break;
        case  CL_INVALID_DEVICE_TYPE: sprintf(cl_err,"CL_INVALID_DEVICE_TYPE");break;
        case  CL_INVALID_PLATFORM: sprintf(cl_err,"CL_INVALID_PLATFORM");break;
        case  CL_INVALID_DEVICE: sprintf(cl_err,"CL_INVALID_DEVICE");break;
        case  CL_INVALID_CONTEXT: sprintf(cl_err,"CL_INVALID_CONTEXT");break;
        case  CL_INVALID_QUEUE_PROPERTIES: sprintf(cl_err,"CL_INVALID_QUEUE_PROPERTIES");break;
        case  CL_INVALID_COMMAND_QUEUE: sprintf(cl_err,"CL_INVALID_COMMAND_QUEUE");break;
        case  CL_INVALID_HOST_PTR: sprintf(cl_err,"CL_INVALID_HOST_PTR");break;
        case  CL_INVALID_MEM_OBJECT: sprintf(cl_err,"CL_INVALID_MEM_OBJECT");break;
        case  CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: sprintf(cl_err,"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR");break;
        case  CL_INVALID_IMAGE_SIZE: sprintf(cl_err,"CL_INVALID_IMAGE_SIZE");break;
        case  CL_INVALID_SAMPLER: sprintf(cl_err,"CL_INVALID_SAMPLER");break;
        case  CL_INVALID_BINARY: sprintf(cl_err,"CL_INVALID_BINARY");break;
        case  CL_INVALID_BUILD_OPTIONS: sprintf(cl_err,"CL_INVALID_BUILD_OPTIONS");break;
        case  CL_INVALID_PROGRAM: sprintf(cl_err,"CL_INVALID_PROGRAM");break;
        case  CL_INVALID_PROGRAM_EXECUTABLE: sprintf(cl_err,"CL_INVALID_PROGRAM_EXECUTABLE");break;
        case  CL_INVALID_KERNEL_NAME: sprintf(cl_err,"CL_INVALID_KERNEL_NAME");break;
        case  CL_INVALID_KERNEL_DEFINITION: sprintf(cl_err,"CL_INVALID_KERNEL_DEFINITION");break;
        case  CL_INVALID_KERNEL: sprintf(cl_err,"CL_INVALID_KERNEL");break;
        case  CL_INVALID_ARG_INDEX: sprintf(cl_err,"CL_INVALID_ARG_INDEX");break;
        case  CL_INVALID_ARG_VALUE: sprintf(cl_err,"CL_INVALID_ARG_VALUE");break;
        case  CL_INVALID_ARG_SIZE: sprintf(cl_err,"CL_INVALID_ARG_SIZE");break;
        case  CL_INVALID_KERNEL_ARGS: sprintf(cl_err,"CL_INVALID_KERNEL_ARGS");break;
        case  CL_INVALID_WORK_DIMENSION: sprintf(cl_err,"CL_INVALID_WORK_DIMENSION");break;
        case  CL_INVALID_WORK_GROUP_SIZE: sprintf(cl_err,"CL_INVALID_WORK_GROUP_SIZE");break;
        case  CL_INVALID_WORK_ITEM_SIZE: sprintf(cl_err,"CL_INVALID_WORK_ITEM_SIZE");break;
        case  CL_INVALID_GLOBAL_OFFSET: sprintf(cl_err,"CL_INVALID_GLOBAL_OFFSET");break;
        case  CL_INVALID_EVENT_WAIT_LIST: sprintf(cl_err,"CL_INVALID_EVENT_WAIT_LIST");break;
        case  CL_INVALID_EVENT: sprintf(cl_err,"CL_INVALID_EVENT");break;
        case  CL_INVALID_OPERATION: sprintf(cl_err,"CL_INVALID_OPERATION");break;
        case  CL_INVALID_GL_OBJECT: sprintf(cl_err,"CL_INVALID_GL_OBJECT");break;
        case  CL_INVALID_BUFFER_SIZE: sprintf(cl_err,"CL_INVALID_BUFFER_SIZE");break;
        case  CL_INVALID_MIP_LEVEL: sprintf(cl_err,"CL_INVALID_MIP_LEVEL");break;
        case  CL_INVALID_GLOBAL_WORK_SIZE: sprintf(cl_err,"CL_INVALID_GLOBAL_WORK_SIZE");break;
        case  CL_INVALID_PROPERTY: sprintf(cl_err,"CL_INVALID_PROPERTY");break;
        case  CL_INVALID_IMAGE_DESCRIPTOR: sprintf(cl_err,"CL_INVALID_IMAGE_DESCRIPTOR");break;
        case  CL_INVALID_COMPILER_OPTIONS: sprintf(cl_err,"CL_INVALID_COMPILER_OPTIONS");break;
        case  CL_INVALID_LINKER_OPTIONS: sprintf(cl_err,"CL_INVALID_LINKER_OPTIONS");break;
        case  CL_INVALID_DEVICE_PARTITION_COUNT: sprintf(cl_err,"CL_INVALID_DEVICE_PARTITION_COUNT");break;
        default : sprintf(cl_err,"Unknown error");break;
    };
    sprintf(result,"Ошибка Opencl №%i (%s)",i_ret,cl_err);
};
