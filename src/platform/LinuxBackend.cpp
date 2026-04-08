#include "LinuxBackend.hpp"

#ifndef _WIN32

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <sys/statvfs.h>

#endif

namespace vsys {

// ================= CPU (TOTAL) =================

CpuTimes LinuxBackend::read_total_cpu() {
#ifndef _WIN32
    std::ifstream file("/proc/stat");
    if (!file.is_open()) return {};

    std::string line;
    std::getline(file, line);

    std::istringstream ss(line);

    std::string label;
    CpuTimes t{};

    ss >> label; // "cpu"

    ss >> t.user >> t.nice >> t.system >> t.idle
       >> t.iowait >> t.irq >> t.softirq;

    return t;
#else
    return {};
#endif
}

// ================= CPU (PER CORE) =================

std::vector<CpuTimes> LinuxBackend::read_per_cpu() {
#ifndef _WIN32
    std::ifstream file("/proc/stat");
    if (!file.is_open()) return {};

    std::vector<CpuTimes> result;
    std::string line;

    while (std::getline(file, line)) {
        if (line.rfind("cpu", 0) != 0) continue;

        // skip total cpu line
        if (line.substr(0, 4) == "cpu ") continue;

        // only cpu0, cpu1...
        if (line.size() > 3 && std::isdigit(line[3])) {

            std::istringstream ss(line);

            std::string label;
            CpuTimes t{};

            ss >> label;
            ss >> t.user >> t.nice >> t.system >> t.idle
               >> t.iowait >> t.irq >> t.softirq;

            result.push_back(t);
        }
    }

    return result;
#else
    return {};
#endif
}

// ================= MEMORY =================

MemoryData LinuxBackend::read_memory() {
#ifndef _WIN32
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) return {};

    MemoryData mem{};
    std::string key;
    uint64_t value;
    std::string unit;

    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") {
            mem.total = value * 1024;
        } else if (key == "MemAvailable:") {
            mem.available = value * 1024;
        } else if (key == "MemFree:") {
            mem.free = value * 1024;
        }
    }

    mem.used = mem.total - mem.available;

    return mem;
#else
    return {};
#endif
}

// ================= DISK =================

std::vector<DiskInfo> LinuxBackend::read_disks() {
#ifndef _WIN32
    std::vector<DiskInfo> disks;

    struct statvfs stat{};
    if (statvfs("/", &stat) == 0) {
        DiskInfo d{};
        d.mount_point = "/";

        uint64_t total = stat.f_blocks * stat.f_frsize;
        uint64_t free  = stat.f_bfree * stat.f_frsize;

        d.total = total;
        d.free = free;
        d.used = total - free;

        disks.push_back(d);
    }

    return disks;
#else
    return {};
#endif
}

// ================= NETWORK =================

std::vector<NetworkStats> LinuxBackend::read_network() {
#ifndef _WIN32
    std::ifstream file("/proc/net/dev");
    if (!file.is_open()) return {};

    std::vector<NetworkStats> result;
    std::string line;

    // skip headers
    std::getline(file, line);
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);

        std::string iface;
        uint64_t rx = 0;
        uint64_t tx = 0;

        ss >> iface;

        // remove ':' from interface name
        iface = iface.substr(0, iface.find(':'));

        ss >> rx;

        // skip unused fields
        for (int i = 0; i < 7; ++i) {
            ss >> line;
        }

        ss >> tx;

        NetworkStats stat{};
        stat.interface = iface;
        stat.rx_bytes = rx;
        stat.tx_bytes = tx;

        result.push_back(stat);
    }

    return result;
#else
    return {};
#endif
}

// ================= BATTERY =================

BatteryInfo LinuxBackend::read_battery() {
#ifndef _WIN32
    BatteryInfo b{};

    std::ifstream cap("/sys/class/power_supply/BAT0/capacity");
    std::ifstream status("/sys/class/power_supply/BAT0/status");

    if (!cap.is_open() || !status.is_open()) {
        b.present = false;
        return b;
    }

    b.present = true;

    cap >> b.percentage;

    std::string stat;
    status >> stat;

    b.charging = (stat == "Charging");
    b.discharging = (stat == "Discharging");

    return b;
#else
    return {};
#endif
}

} // namespace vsys
