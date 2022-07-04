#pragma once
#include <Windows.h>
#include <string>
#include <TlHelp32.h>

using std::wstring;
using std::string;
using namespace std;

void getcpuid(unsigned int* CPUInfo, unsigned int InfoType);
void getcpuidex(unsigned int* CPUInfo, unsigned int InfoType, unsigned int ECXValue);
string get_cpuId();
string formatTimeStamp(int64_t timestamp);