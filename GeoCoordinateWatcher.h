#pragma once
#include "LocationCallback.h"
#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <locationapi.h>
#include <functional>
#include "GeoStruct.h"

using namespace std;

class GeoCoordinateWatcher
{
public:
    static GeoCoordinateWatcher* GetInstance();
    void onPositionChange(bool isHighAccuracy, function<void(GeoCoordinate)> func);
    function<void(GeoCoordinate)> callback;
    ~GeoCoordinateWatcher();
protected:
    GeoCoordinateWatcher();
private:
    static GeoCoordinateWatcher* _instance;
    class CInitializeATL : public CAtlExeModuleT<CInitializeATL> {};
    CInitializeATL g_InitializeATL;
    HRESULT hr;
    CComPtr<ILocation> spLocation;
    CComObject<CLocationEvents>* pLocationEvents = nullptr;
};

