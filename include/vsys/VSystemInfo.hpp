#pragma once

#include <memory>
#include <vector>

#include "Types.hpp"
#include "Backend.hpp"

namespace vsys {

/**
 * @brief Cross-platform system information interface.
 *
 * Provides access to CPU, memory, disk, network, and battery data.
 * Data is cached and updated via explicit refresh calls.
 */
class VSystemInfo {
public:
    static VSystemInfo create();

    // ===== Refresh =====
    void refresh();
    void refresh_cpu();
    void refresh_memory();
    void refresh_disk();
    void refresh_network();
    void refresh_battery();

    // ===== CPU =====
    float cpu_usage() const;
    const std::vector<float>& per_cpu_usage() const;
    const CpuData& cpu() const;

    // ===== Memory =====
    uint64_t total_memory() const;
    uint64_t used_memory() const;
    uint64_t free_memory() const;
    const MemoryData& memory() const;

    // ===== Disk =====
    const std::vector<DiskInfo>& disks() const;

    // ===== Network =====
    const std::vector<NetworkStats>& networks() const;

    // ===== Battery =====
    const BatteryInfo& battery() const;

private:
    VSystemInfo() = default;

    std::unique_ptr<IPlatformBackend> backend;

    CpuData cpu_;
    MemoryData memory_;
    std::vector<DiskInfo> disks_;
    std::vector<NetworkStats> networks_;
    BatteryInfo battery_;
};

} // namespace vsys
