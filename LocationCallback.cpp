#include "LocationCallback.h"
#include "GeoStruct.h"
#include "GeoCoordinateWatcher.h"

PCWSTR GUIDToString(REFGUID guid, PWSTR psz, UINT cch)
{
    StringFromGUID2(guid, psz, cch);
    return psz;
}

const double kKnotsToMetresPerSecondConversionFactor = 463.0 / 900.0;

void ConvertKnotsToMetresPerSecond(double* knots) {
    *knots *= kKnotsToMetresPerSecondConversionFactor;
}

static int64_t GetUnixTime()
{
    int64_t times = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return times;
}

STDMETHODIMP CLocationEvents::OnLocationChanged(__RPC__in REFIID reportType, __RPC__in_opt ILocationReport* pLocationReport)
{
    if (IID_ILatLongReport == reportType)
    {
        CComPtr<ILatLongReport> spLatLongReport;

        if ((SUCCEEDED(pLocationReport->QueryInterface(IID_PPV_ARGS(&spLatLongReport)))) && (NULL != spLatLongReport.p))
        {
            wchar_t szGUID[64];
            //wprintf(L"\nReportType: %s", GUIDToString(IID_ILatLongReport, szGUID, ARRAYSIZE(szGUID)));

            SYSTEMTIME systemTime;
            if (SUCCEEDED(spLatLongReport->GetTimestamp(&systemTime)))
            {
                //wprintf(L"\nTimestamp: %d\n", systemtime_to_time_t(systemTime));
                ULONGLONG currentTime = 0, diffTime = 0;

                if (TRUE == SystemTimeToFileTime(&systemTime, (FILETIME*)&currentTime))
                {
                    diffTime = (currentTime > m_previousTime) ? (currentTime - m_previousTime) : 0;
                }

                /*wprintf(L"Timestamp: YY:%u, MM:%u, DD:%u, HH:%u, MM:%u, SS:%u, MS:%u [%I64u]\n",
                    systemTime.wYear,
                    systemTime.wMonth,
                    systemTime.wDay,
                    systemTime.wHour,
                    systemTime.wMinute,
                    systemTime.wSecond,
                    systemTime.wMilliseconds,
                    diffTime / 10000);*/

                m_previousTime = currentTime;
            }

            DOUBLE latitude = 0, longitude = 0, altitude = 0, errorRadius = 0, altitudeError = 0;

            if (SUCCEEDED(spLatLongReport->GetLatitude(&latitude)))
            {
                //wprintf(L"Latitude: %f\n", latitude);
            }

            if (SUCCEEDED(spLatLongReport->GetLongitude(&longitude)))
            {
                //wprintf(L"Longitude: %f\n", longitude);
            }

            if (SUCCEEDED(spLatLongReport->GetAltitude(&altitude)))
            {
                //wprintf(L"Altitude: %f\n", altitude);
            }
            else
            {
                //wprintf(L"Altitude: Not available.\n");
            }

            if (SUCCEEDED(spLatLongReport->GetErrorRadius(&errorRadius)))
            {
                //wprintf(L"HorizontalAccuracy: %f\n", errorRadius);
            }

            if (SUCCEEDED(spLatLongReport->GetAltitudeError(&altitudeError)))
            {
                //wprintf(L"VerticalAccuracy: %f\n", altitudeError);
            }
            else
            {
                //wprintf(L"VerticalAccuracy: Not available.\n");
            }

            PROPVARIANT heading;
            double course;
            PropVariantInit(&heading);
            if (SUCCEEDED(spLatLongReport->GetValue(SENSOR_DATA_TYPE_TRUE_HEADING_DEGREES, &heading))) {
                PropVariantToDouble(heading, &course);
                //wprintf(L"Course: %f\n", course);
            }

            PROPVARIANT speed;
            double speed_;
            PropVariantInit(&speed);
            if (SUCCEEDED(spLatLongReport->GetValue(SENSOR_DATA_TYPE_SPEED_KNOTS, &speed))) {
                PropVariantToDouble(speed, &speed_);
                ConvertKnotsToMetresPerSecond(&speed_);
                //wprintf(L"Speed: %f\n", speed_);
            }
            GeoCoordinate geoCoordinate;
            Info info;
            info.setLatitude(latitude);
            info.setLongitude(longitude);
            info.setAltitude(altitude);
            info.setHorizontalAccuracy(errorRadius);
            info.setVerticalAccuracy(altitudeError);
            info.setSpeed(speed_);
            info.setCourse(course);
            //取13位时间戳
            info.setTimestamp(GetUnixTime());
            info.setFormatTimestamp(formatTimeStamp(time(nullptr)));
            geoCoordinate.setInfo(info);
            //printf("%s\n", info.toJson().c_str());
            GeoCoordinateWatcher* watcher = GeoCoordinateWatcher::GetInstance();
            watcher->callback(geoCoordinate);
        }
    }

    return S_OK;
}


// This is called when the status of a report type changes.
// The LOCATION_REPORT_STATUS enumeration is defined in LocApi.h in the SDK
STDMETHODIMP CLocationEvents::OnStatusChanged(__RPC__in REFIID reportType, LOCATION_REPORT_STATUS status)
{
    if (IID_ILatLongReport == reportType)
    {
        switch (status)
        {
        case REPORT_NOT_SUPPORTED:
            wprintf(L"\nNo devices detected.\n");
            break;
        case REPORT_ERROR:
            wprintf(L"\nReport error.\n");
            break;
        case REPORT_ACCESS_DENIED:
            wprintf(L"\nAccess denied to reports.\n");
            break;
        case REPORT_INITIALIZING:
            wprintf(L"\nReport is initializing.\n");
            break;
        case REPORT_RUNNING:
            wprintf(L"\nRunning.\n");
            break;
        }
    }
    else if (IID_ICivicAddressReport == reportType)
    {
    }

    return S_OK;
}