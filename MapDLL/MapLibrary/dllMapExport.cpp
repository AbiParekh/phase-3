#define __dll__
#include "dllMapExport.h"


int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}

IMPEXP void* CreateMapClassInstance()
{
    std::cout   << __func__ <<  " INFO: Accessing DLL Function (CreateMapClassInstance)" << std::endl;
    Map* temp = new Map;
    return ((void*)temp);
}