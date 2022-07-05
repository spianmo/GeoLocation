#include <iostream>
#include "GeoCoordinateWatcher.h"

using namespace std;

int main(int argc, char *argv[]) {
    system("chcp 65001");
    GeoCoordinateWatcher *watcher = GeoCoordinateWatcher::GetInstance();
    watcher->onPositionChange(true, [](const GeoCoordinate &location) -> void {
        cout << location.toJson() << endl;
    });
    system("pause");
    delete watcher;
    return 0;
}
