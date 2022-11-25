#include "LocationCallback.h"
#include "GeoCoordinateWatcher.h"

const double kKnotsToMetresPerSecondConversionFactor = 463.0 / 900.0;

void ConvertKnotsToMetresPerSecond(double *knots) {
    *knots *= kKnotsToMetresPerSecondConversionFactor;
}

static int64_t GetUnixTime() {
    int64_t times = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return times;
}

std::string formatTimeStamp(int64_t timestamp) {
    time_t t = timestamp;
    struct tm *tm = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
    return buf;
}

STDMETHODIMP CLocationEvents::OnLocationChanged(__RPC__in REFIID reportType, __RPC__in_opt ILocationReport *pLocationReport) {
    if (IID_ILatLongReport == reportType) {
        CComPtr<ILatLongReport> spLatLongReport;

        if ((SUCCEEDED(pLocationReport->QueryInterface(IID_PPV_ARGS(&spLatLongReport)))) && (nullptr != spLatLongReport.p)) {

            SYSTEMTIME systemTime;
            int64_t currentTime = 0, diffTime = 0;
            if (SUCCEEDED(spLatLongReport->GetTimestamp(&systemTime))) {
                if (TRUE == SystemTimeToFileTime(&systemTime, (FILETIME *) &currentTime)) {
                    diffTime = (currentTime > m_previousTime) ? (currentTime - m_previousTime) : 0;
                    if(m_previousTime == 0) diffTime = 0;
                }
                m_previousTime = currentTime;
            }

            DOUBLE latitude = 0, longitude = 0, altitude = 0, errorRadius = 0, altitudeError = 0;

            spLatLongReport->GetLatitude(&latitude);
            spLatLongReport->GetLongitude(&longitude);
            spLatLongReport->GetAltitude(&altitude);
            spLatLongReport->GetErrorRadius(&errorRadius);
            spLatLongReport->GetAltitudeError(&altitudeError);

            PROPVARIANT heading;
            double course;
            PropVariantInit(&heading);
            if (SUCCEEDED(spLatLongReport->GetValue(SENSOR_DATA_TYPE_TRUE_HEADING_DEGREES, &heading))) {
                PropVariantToDouble(heading, &course);
            }

            PROPVARIANT speed;
            double _speed;
            PropVariantInit(&speed);
            if (SUCCEEDED(spLatLongReport->GetValue(SENSOR_DATA_TYPE_SPEED_KNOTS, &speed))) {
                PropVariantToDouble(speed, &_speed);
                ConvertKnotsToMetresPerSecond(&_speed);
            }
            GeoCoordinate geoCoordinate;
            Info info;
            info.setLatitude(latitude);
            info.setLongitude(longitude);
            info.setAltitude(altitude);
            info.setHorizontalAccuracy(errorRadius);
            info.setVerticalAccuracy(altitudeError);
            info.setSpeed(_speed);
            info.setCourse(course);
            info.setIntervals(diffTime);
            info.setTimestamp(GetUnixTime());
            info.setFormatTimestamp(formatTimeStamp(time(nullptr)));
            geoCoordinate.setInfo(info);
            GeoCoordinateWatcher *watcher = GeoCoordinateWatcher::GetInstance();
            watcher->callback(geoCoordinate);
        }
    }
    return S_OK;
}

STDMETHODIMP CLocationEvents::OnStatusChanged(__RPC__in REFIID reportType, LOCATION_REPORT_STATUS status) {
    if (IID_ILatLongReport == reportType) {
        switch (status) {
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
    return S_OK;
}