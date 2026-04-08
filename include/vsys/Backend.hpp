#pragma once

#include <memory>
#include <vector>

#include "Types.hpp"

namespace vsys {

/**
 * @brief Platform abstraction interface for system information.
 *
 * Each platform (Linux, Windows, etc.) must implement this interface.
 * This layer isolates OS-specific logic from the core library.
 */
class IPlatformBackend {
public:
    virtual ~IPlatformBackend() = default;

    // ================= CPU =================

    /**
     * @brief Read aggregated CPU times.
     * @return Total CPU time snapshot.
     */
    virtual CpuTimes read_total_cpu() = 0;

    /**
     * @brief Read per-core CPU times.
     * @return Vector of CPU time snapshots for each core.
     */
    virtual std::vector<CpuTimes> read_per_cpu() = 0;

    // ================= Memory =================

    /**
     * @brief Read memory statistics.
     * @return MemoryData structure.
     */
    virtual MemoryData read_memory() = 0;

    // ================= Disk =================

    /**
     * @brief Read disk usage information.
     * @return List of disks.
     */
    virtual std::vector<DiskInfo> read_disks() = 0;

    // ================= Network =================

    /**
     * @brief Read network statistics.
     * @return List of network interfaces.
     */
    virtual std::vector<NetworkStats> read_network() = 0;

    // ================= Battery =================

    /**
     * @brief Read battery information.
     * @return BatteryInfo structure.
     */
    virtual BatteryInfo read_battery() = 0;
};


// ================= Factory =================

/**
 * @brief Factory for creating platform-specific backend.
 *
 * This hides OS-specific implementations from the user.
 */
class BackendFactory {
public:
    /**
     * @brief Create backend based on current platform.
     * @return Unique pointer to backend instance.
     */
    static std::unique_ptr<IPlatformBackend> create();
};

} // namespace vsys
