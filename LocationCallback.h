#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <locationapi.h>
#include <sensors.h>
#include <propvarutil.h>
#include <ctime>
#include <chrono>
#include "GeoStruct.h"
#pragma comment( lib, "propsys.lib" )
#pragma comment( lib, "locationapi.lib")

using namespace GeoLocation;

class CLocationEvents :
    public CComObjectRoot,
    public ILocationEvents
{
public:
    CLocationEvents() : m_previousTime(0) {}

    virtual ~CLocationEvents() = default;

    DECLARE_NOT_AGGREGATABLE(CLocationEvents)

    BEGIN_COM_MAP(CLocationEvents)
        COM_INTERFACE_ENTRY(ILocationEvents)
    END_COM_MAP()

    STDMETHOD(OnLocationChanged)(__RPC__in REFIID reportType, __RPC__in_opt ILocationReport* pLocationReport);
    STDMETHOD(OnStatusChanged)(__RPC__in REFIID reportType, LOCATION_REPORT_STATUS status);

private:

    int64_t m_previousTime;

};