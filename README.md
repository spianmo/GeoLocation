# GeoLocation
>
> 使用C++ ATL COM在Windows平台上的实时GPS坐标位置获取，支持位置监听，与dotNet的System.Device.Location实现方式相同
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

