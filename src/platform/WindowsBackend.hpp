#pragma once

#include <vsys/Backend.hpp>

namespace vsys {

class WindowsBackend : public IPlatformBackend {
public:
    WindowsBackend() = default;
    ~WindowsBackend() override = default;

    // CPU
    CpuTimes read_total_cpu() override;
    std::vector<CpuTimes> read_per_cpu() override;

    // Memory
    MemoryData read_memory() override;

    // Disk
    std::vector<DiskInfo> read_disks() override;

    // Network
    std::vector<NetworkStats> read_network() override;

    // Battery
    BatteryInfo read_battery() override;
};

} // namespace vsys
