#include <Windows.h>
#include <cstdio>
#include <ctime>
#include "util.h"

using namespace std;


string get_cpuId() {
    char pCpuId[32] = "";
    int dwBuf[4];
    getcpuid((unsigned int*)dwBuf, 1);
    sprintf(pCpuId, "%08X", dwBuf[3]);
    sprintf(pCpuId + 8, "%08X", dwBuf[0]);
    return pCpuId;
}


void getcpuid(unsigned int* CPUInfo, unsigned int InfoType) {
#if defined(__GNUC__)// GCC  
    __cpuid(InfoType, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#elif defined(_MSC_VER)// MSVC  
    getcpuidex(CPUInfo, InfoType, 0);
#endif
}

void getcpuidex(unsigned int* CPUInfo, unsigned int InfoType, unsigned int ECXValue) {
#if defined(_MSC_VER) // MSVC  
#if defined(_WIN64) // 64位下不支持内联汇编. 1600: VS2010, 据说VC2008 SP1之后才支持__cpuidex.  
    __cpuidex((int*)(void*)CPUInfo, (int)InfoType, (int)ECXValue);
#else  
    if (NULL == CPUInfo)
        return;
    _asm {
        // load. 读取参数到寄存器.  
        mov edi, CPUInfo;
        mov eax, InfoType;
        mov ecx, ECXValue;
        // CPUID  
        cpuid;
        // save. 将寄存器保存到CPUInfo  
        mov[edi], eax;
        mov[edi + 4], ebx;
        mov[edi + 8], ecx;
        mov[edi + 12], edx;
    }
#endif  
#endif  
}


string formatTimeStamp(int64_t timestamp) {
    time_t t = timestamp;
    struct tm *tm = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
    return buf;
}