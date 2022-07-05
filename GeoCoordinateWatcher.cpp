#include "GeoCoordinateWatcher.h"

IID REPORT_TYPES[] = { IID_ILatLongReport };
GeoCoordinateWatcher* GeoCoordinateWatcher::_instance;

GeoCoordinateWatcher::GeoCoordinateWatcher()
{
    _instance = this;
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);;
    if (SUCCEEDED(hr))
    {
        hr = spLocation.CoCreateInstance(CLSID_Location);
    }
}

GeoCoordinateWatcher::~GeoCoordinateWatcher()
{
    if (SUCCEEDED(hr))
    {
        for (auto index : REPORT_TYPES)
        {
            spLocation->UnregisterForReport(index);
        }
    }

    if (nullptr != pLocationEvents)
    {
        pLocationEvents->Release();
        pLocationEvents = nullptr;
    }
}

GeoCoordinateWatcher* GeoCoordinateWatcher::GetInstance()
{
    if (_instance == nullptr)
        _instance = new GeoCoordinateWatcher();
    return _instance;
}

void GeoCoordinateWatcher::onPositionChange(bool isHighAccuracy, std::function<void(GeoCoordinate)> func)
{
    this->callback = std::move(func);
    if (SUCCEEDED(hr))
    {
        hr = CComObject<CLocationEvents>::CreateInstance(&pLocationEvents);
        if (nullptr != pLocationEvents)
        {
            pLocationEvents->AddRef();
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = spLocation->RequestPermissions(nullptr, REPORT_TYPES, ARRAYSIZE(REPORT_TYPES), FALSE);
        if (FAILED(hr))
        {
            wprintf(L"Warning: Unable to request permissions.\n");
        }

        for (auto index : REPORT_TYPES)
        {
            spLocation->SetDesiredAccuracy(IID_ILatLongReport, isHighAccuracy ? LOCATION_DESIRED_ACCURACY_HIGH : LOCATION_DESIRED_ACCURACY_DEFAULT);
            hr = spLocation->RegisterForReport(pLocationEvents, index, 0);
        }
    }
}
