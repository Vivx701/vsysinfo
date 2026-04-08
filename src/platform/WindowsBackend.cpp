#include "WindowsBackend.hpp"

#ifdef _WIN32

#include <windows.h>
#include <iphlpapi.h>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

#pragma comment(lib, "iphlpapi.lib")

#endif

namespace vsys {

// ================= CPU (Total) =================

CpuTimes WindowsBackend::read_total_cpu() {
#ifdef _WIN32
    FILETIME idleTime, kernelTime, userTime;

    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return {};
    }

    auto to_uint64 = [](const FILETIME& ft) -> uint64_t {
        return (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    };

    uint64_t idle   = to_uint64(idleTime);
    uint64_t kernel = to_uint64(kernelTime);
    uint64_t user   = to_uint64(userTime);

    CpuTimes t{};
    t.idle = idle;
    t.system = kernel - idle; // kernel includes idle
    t.user = user;

    return t;
#else
    return {};
#endif
}

// ================= CPU (Per Core) =================

std::vector<CpuTimes> WindowsBackend::read_per_cpu() {
    // TODO: Implement using PDH or NtQuerySystemInformation
    return {};
}

// ================= Memory =================

MemoryData WindowsBackend::read_memory() {
#ifdef _WIN32
    MEMORYSTATUSEX mem{};
    mem.dwLength = sizeof(mem);

    if (!GlobalMemoryStatusEx(&mem)) {
        return {};
    }

    MemoryData data{};
    data.total = mem.ullTotalPhys;
    data.available = mem.ullAvailPhys;
    data.used = data.total - data.available;
    data.free = mem.ullAvailPhys;

    return data;
#else
    return {};
#endif
}

// ================= Disk =================

std::vector<DiskInfo> WindowsBackend::read_disks() {
#ifdef _WIN32
    std::vector<DiskInfo> disks;

    char buffer[512];
    DWORD len = GetLogicalDriveStringsA(sizeof(buffer), buffer);

    if (len == 0) return disks;

    for (char* drive = buffer; *drive; drive += strlen(drive) + 1) {

        ULARGE_INTEGER freeBytesAvailable, totalBytes, freeBytes;

        if (GetDiskFreeSpaceExA(drive, &freeBytesAvailable, &totalBytes, &freeBytes)) {

            DiskInfo info{};
            info.mount_point = drive;
            info.total = totalBytes.QuadPart;
            info.free = freeBytes.QuadPart;
            info.used = info.total - info.free;

            disks.push_back(info);
        }
    }

    return disks;
#else
    return {};
#endif
}

// ================= Network =================

std::vector<NetworkStats> WindowsBackend::read_network() {
#ifdef _WIN32
    std::vector<NetworkStats> result;

    PMIB_IFTABLE table = nullptr;
    DWORD size = 0;

    if (GetIfTable(nullptr, &size, FALSE) == ERROR_INSUFFICIENT_BUFFER) {

        table = (MIB_IFTABLE*)malloc(size);

        if (table && GetIfTable(table, &size, FALSE) == NO_ERROR) {

            for (DWORD i = 0; i < table->dwNumEntries; ++i) {
                const MIB_IFROW& row = table->table[i];

                NetworkStats stat{};

                struct NetworkStats {
                    std::string name;
                    uint64_t rx_bytes = 0;
                    uint64_t tx_bytes = 0;
                };

                stat.rx_bytes = row.dwInOctets;
                stat.tx_bytes = row.dwOutOctets;

                result.push_back(stat);
            }
        }

        if (table) {
            free(table);
        }
    }

    return result;
#else
    return {};
#endif
}

// ================= Battery =================

BatteryInfo WindowsBackend::read_battery() {
#ifdef _WIN32
    SYSTEM_POWER_STATUS s{};

    if (!GetSystemPowerStatus(&s)) {
        return {};
    }

    BatteryInfo b{};

    // 128 = no system battery
    b.present = (s.BatteryFlag != 128);

    b.percentage = static_cast<float>(s.BatteryLifePercent);

    b.charging = (s.ACLineStatus == 1);
    b.discharging = !b.charging;

    b.time_remaining_sec = s.BatteryLifeTime;

    return b;
#else
    return {};
#endif
}

} // namespace vsys
