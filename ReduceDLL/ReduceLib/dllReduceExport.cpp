#define __dll__
#include "dllReduceExport.h"

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}

IMPEXP void* CreateReduceClassInstance()
{
    std::cout << "INFO: Accessing DLL Function (CreateReduceClassInstance)" << std::endl;
    Reduce* temp = new Reduce;
    return ((void*)temp);
}