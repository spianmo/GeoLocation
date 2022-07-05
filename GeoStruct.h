#pragma once

#include "json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

namespace GeoLocation {
    using nlohmann::json;

    inline json get_untyped(const json &j, const char *property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return {};
    }

    inline json get_untyped(const json &j, const std::string &property) {
        return get_untyped(j, property.data());
    }

    class Info {
    public:
        Info() = default;

        virtual ~Info() = default;

    private:
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

    public:
        [[nodiscard]] const double &getAltitude() const { return altitude; }

        double &getMutableAltitude() { return altitude; }

        void setAltitude(const double &value) { this->altitude = value; }

        [[nodiscard]] const double &getCourse() const { return course; }

        double &getMutableCourse() { return course; }

        void setCourse(const double &value) { this->course = value; }

        [[nodiscard]] const std::string &getFormatTimestamp() const { return formatTimestamp; }

        std::string &getMutableFormatTimestamp() { return formatTimestamp; }

        void setFormatTimestamp(const std::string &value) { this->formatTimestamp = value; }

        [[nodiscard]] const double &getHorizontalAccuracy() const { return horizontalAccuracy; }

        double &getMutableHorizontalAccuracy() { return horizontalAccuracy; }

        void setHorizontalAccuracy(const double &value) { this->horizontalAccuracy = value; }

        [[nodiscard]] const double &getLatitude() const { return latitude; }

        double &getMutableLatitude() { return latitude; }

        void setLatitude(const double &value) { this->latitude = value; }

        [[nodiscard]] const double &getLongitude() const { return longitude; }

        double &getMutableLongitude() { return longitude; }

        void setLongitude(const double &value) { this->longitude = value; }

        [[nodiscard]] const double &getSpeed() const { return speed; }

        double &getMutableSpeed() { return speed; }

        void setSpeed(const double &value) { this->speed = value; }

        [[nodiscard]] const int64_t &getIntervals() const { return intervals; }

        int64_t &getMutableIntervals() { return intervals; }

        void setIntervals(const int64_t &value) { this->intervals = value; }

        [[nodiscard]] const int64_t &getTimestamp() const { return timestamp; }

        int64_t &getMutableTimestamp() { return timestamp; }

        void setTimestamp(const int64_t &value) { this->timestamp = value; }

        [[nodiscard]] const double &getVerticalAccuracy() const { return verticalAccuracy; }

        double &getMutableVerticalAccuracy() { return verticalAccuracy; }

        void setVerticalAccuracy(const double &value) { this->verticalAccuracy = value; }

        [[nodiscard]] json toJsonObject() const {
            json j = json::object();
            j["altitude"] = this->getAltitude();
            j["course"] = this->getCourse();
            j["format_timestamp"] = this->getFormatTimestamp();
            j["horizontal_accuracy"] = this->getHorizontalAccuracy();
            j["latitude"] = this->getLatitude();
            j["longitude"] = this->getLongitude();
            j["speed"] = this->getSpeed();
            j["intervals"] = this->getIntervals();
            j["timestamp"] = this->getTimestamp();
            j["vertical_accuracy"] = this->getVerticalAccuracy();
            return j;
        }

    };

    class GeoCoordinate {
    public:
        GeoCoordinate() = default;

        virtual ~GeoCoordinate() = default;

    private:
        std::string cpuid = getCpuId();
        Info info;
        std::string softwareVer = "V1.0.0";

        static std::string getCpuId() {
            char pCpuId[32] = "";
            int dwBuf[4];
            #if defined(__GNUC__)
                        __cpuid(InfoType, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
            #elif defined(_MSC_VER)
            #if defined(_MSC_VER)
            #if defined(_WIN64)
                        __cpuidex((int *) (void *) (unsigned int *) dwBuf, 1, 0);
            #else
                auto* CPUInfo = (unsigned int *) dwBuf;
                if (nullptr == CPUInfo)
                    throw std::exception("CPUInfo is nullptr");
                _asm {
                    mov edi, CPUInfo;
                    mov eax, 1;
                    mov ecx, 0;
                    cpuid;
                    mov[edi], eax;
                    mov[edi + 4], ebx;
                    mov[edi + 8], ecx;
                    mov[edi + 12], edx;
                }
            #endif
            #endif
            #endif
            sprintf(pCpuId, "%08X", dwBuf[3]);
            sprintf(pCpuId + 8, "%08X", dwBuf[0]);
            return pCpuId;
        }

    public:
        [[nodiscard]] const std::string &getCpuid() const { return cpuid; }

        std::string &getMutableCpuid() { return cpuid; }

        void setCpuid(const std::string &value) { this->cpuid = value; }

        [[nodiscard]] const Info &getInfo() const { return info; }

        Info &getMutableInfo() { return info; }

        void setInfo(const Info &value) { this->info = value; }

        [[nodiscard]] const std::string &getSoftwareVer() const { return softwareVer; }

        std::string &getMutableSoftwareVer() { return softwareVer; }

        void setSoftwareVer(const std::string &value) { this->softwareVer = value; }

        [[nodiscard]] std::string toJson() const {
            json j = json::object();
            j["cpuid"] = this->getCpuid();
            j["info"] = this->getInfo().toJsonObject();
            j["software_ver"] = this->getSoftwareVer();
            return j.dump(4);
        }
    };
}