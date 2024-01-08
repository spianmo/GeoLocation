# GeoLocation
>
> 使用C++ ATL COM在Windows平台上实时获取GPS坐标位置，支持位置监听，与dotNet的System.Device.Location实现方式相同
>
> Use C++ ATL COM to obtain real-time GPS coordinates on the Windows platform, support location monitoring, and implement the same method as dotNet's System.Device.Location

![image](https://user-images.githubusercontent.com/18194268/203357765-1562d806-c739-4b2d-b0e2-c55b6b987813.png)


**Usage:**

```c++
#include <iostream>
#include "GeoCoordinateWatcher.h"

using namespace std;

int main(int argc, char *argv[]) {
    GeoCoordinateWatcher *watcher = GeoCoordinateWatcher::GetInstance();
    watcher->onPositionChange(true, [](const GeoCoordinate &location) -> void {
        cout << location.toJson() << endl;
    });
    system("pause");
    delete watcher;
    return 0;
}
```

**GeoCoordinate Struct:**

```C++
double altitude = 0.0;
double course = 0.0;
std::string formatTimestamp;
double horizontalAccuracy = 0.0;
double latitude = 0.0;
double longitude = 0.0;
double speed = 0.0;
int64_t intervals = 0;
int64_t timestamp = 0;
double verticalAccuracy = 0.0;
```

**About "Access denied to reports" Problem**

["Access denied to reports" · Issue #5 · spianmo/GeoLocation (github.com)](https://github.com/spianmo/GeoLocation/issues/5)

Because your windows does not have the positioning switch turned on, or your application is not allowed to perform GPS positioning, you can force the Windows positioning switch to be turned on by adding a registry. You can complete the addition of the registry in MFC, but please note that UAC permissions are required.

```bat
REG ADD HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\PolicyManager\default\Privacy\LetAppsAccessLocation\ /f /v Value /t REG_DWORD /d 0 >nul
```

Location logs including Windows systems can also be cleared through the registry.

```bat
REG DELETE HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\CapabilityAccessManager\ConsentStore\location\NonPackaged /f >nul
```

A normal business logic for using GPS positioning on Windows is: first determine whether the system contains a GPS sensor. If the current computer does not contain a GPS sensor, it will prompt that the device does not support GPS positioning. Then determine whether the positioning switch of Windows is turned on. If it is not turned on, it will prompt first. First enable GPS permissions or actively increase UAC and then enable the system's positioning function by modifying the registry.

I can give a sample code in C#, which is similar in C++

```c#
    public static void PassLocationPermission()
    {
        RegistryKey localKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, Environment.Is64BitOperatingSystem ? RegistryView.Registry64 : RegistryView.Registry32);
        RegistryKey LetAppsAccessLocation = localKey.OpenSubKey(@"SOFTWARE\Microsoft\PolicyManager\default\Privacy\LetAppsAccessLocation", true);
        LetAppsAccessLocation.SetValue("Value", "1", RegistryValueKind.DWord);
        LetAppsAccessLocation.Close();
    }

    public static void ClearLocationRecord()
    {
        RegistryKey LocationRecord = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\CapabilityAccessManager\ConsentStore\location\", true);
        LocationRecord.DeleteSubKeyTree("NonPackaged", false);
        LocationRecord.Close();

    }
```

It is worth mentioning that this library is the core implementation of GPS positioning in the current C# core library. The implementation of the Geolocation function in chromium79 is also similar. Currently, there is no other good way to obtain positioning on Windows except using ATL COM to interact with the position sensor to obtain the position.

As @Cancy126 mentioned, whether to turn on the positioning switch of Windows will affect whether the positioning function can be used normally. However, there is a problem. Whether to use AGPS has nothing to do with turning on the positioning switch. AGPS is also automatically selected by the system after the Windows positioning switch is turned on. Yes, AGPS will be used when the GPS signal is poor (please refer to Microsoft official documentation[[Location API - Win32 apps | Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/locationapi/windows-location-api-portal)] on this).

Similarly, if the Windows location switch is not turned on, you can actually obtain the approximate geographical location through IP. This part needs to be coded by yourself.
