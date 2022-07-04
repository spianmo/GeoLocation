#pragma once

#include "json.hpp"
#include "util.h"

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
        __int64 timestamp = 0;
        double verticalAccuracy = 0.0;

    public:
        const double &getAltitude() const { return altitude; }

        double &getMutableAltitude() { return altitude; }

        void setAltitude(const double &value) { this->altitude = value; }

        const double &getCourse() const { return course; }

        double &getMutableCourse() { return course; }

        void setCourse(const double &value) { this->course = value; }

        const std::string &getFormatTimestamp() const { return formatTimestamp; }

        std::string &getMutableFormatTimestamp() { return formatTimestamp; }

        void setFormatTimestamp(const std::string &value) { this->formatTimestamp = value; }

        const double &getHorizontalAccuracy() const { return horizontalAccuracy; }

        double &getMutableHorizontalAccuracy() { return horizontalAccuracy; }

        void setHorizontalAccuracy(const double &value) { this->horizontalAccuracy = value; }

        const double &getLatitude() const { return latitude; }

        double &getMutableLatitude() { return latitude; }

        void setLatitude(const double &value) { this->latitude = value; }

        const double &getLongitude() const { return longitude; }

        double &getMutableLongitude() { return longitude; }

        void setLongitude(const double &value) { this->longitude = value; }

        const double &getSpeed() const { return speed; }

        double &getMutableSpeed() { return speed; }

        void setSpeed(const double &value) { this->speed = value; }

        const __int64 &getTimestamp() const { return timestamp; }

        __int64 &getMutableTimestamp() { return timestamp; }

        void setTimestamp(const __int64 &value) { this->timestamp = value; }

        const double &getVerticalAccuracy() const { return verticalAccuracy; }

        double &getMutableVerticalAccuracy() { return verticalAccuracy; }

        void setVerticalAccuracy(const double &value) { this->verticalAccuracy = value; }

        json toJsonObject() const {
            json j = json::object();
            j["altitude"] = this->getAltitude();
            j["course"] = this->getCourse();
            j["format_timestamp"] = this->getFormatTimestamp();
            j["horizontal_accuracy"] = this->getHorizontalAccuracy();
            j["latitude"] = this->getLatitude();
            j["longitude"] = this->getLongitude();
            j["speed"] = this->getSpeed();
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
        std::string cpuid = get_cpuId();
        Info info;
        std::string softwareVer = "V1.0.0";

    public:
        const std::string &getCpuid() const { return cpuid; }

        std::string &getMutableCpuid() { return cpuid; }

        void setCpuid(const std::string &value) { this->cpuid = value; }

        const Info &getInfo() const { return info; }

        Info &getMutableInfo() { return info; }

        void setInfo(const Info &value) { this->info = value; }

        const std::string &getSoftwareVer() const { return softwareVer; }

        std::string &getMutableSoftwareVer() { return softwareVer; }

        void setSoftwareVer(const std::string &value) { this->softwareVer = value; }

        std::string toJson() const {
            json j = json::object();
            j["cpuid"] = this->getCpuid();
            j["info"] = this->getInfo().toJsonObject();
            j["software_ver"] = this->getSoftwareVer();
            return j.dump(4);
        }
    };
}