#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace vsys {

/**
 * @brief CPU time statistics for a single CPU or aggregated system.
 *
 * Values represent time spent in different CPU states.
 */
struct CpuTimes {
    uint64_t user = 0;      ///< Time spent in user mode
    uint64_t nice = 0;      ///< Time spent in user mode with low priority
    uint64_t system = 0;    ///< Time spent in kernel mode
    uint64_t idle = 0;      ///< Time spent idle
    uint64_t iowait = 0;    ///< Time waiting for I/O
    uint64_t irq = 0;       ///< Time servicing interrupts
    uint64_t softirq = 0;   ///< Time servicing soft interrupts
};

/**
 * @brief Aggregated CPU usage data.
 *
 * Stores both previous and current snapshots to compute CPU usage.
 */
struct CpuData {
    float total_usage = 0.0f;                 ///< Total CPU usage percentage
    std::vector<float> per_core_usage;        ///< Per-core CPU usage percentages

    CpuTimes prev_total{};                    ///< Previous total CPU snapshot
    CpuTimes curr_total{};                    ///< Current total CPU snapshot

    std::vector<CpuTimes> prev_cores;         ///< Previous per-core snapshots
    std::vector<CpuTimes> curr_cores;         ///< Current per-core snapshots
};

/**
 * @brief Memory usage information.
 */
struct MemoryData {
    uint64_t total = 0;       ///< Total physical memory
    uint64_t available = 0;   ///< Available memory
    uint64_t used = 0;        ///< Used memory
    uint64_t free = 0;        ///< Free memory
};

/**
 * @brief Disk usage information.
 */
struct DiskInfo {
    std::string mount_point;  ///< Mount point path
    uint64_t total = 0;       ///< Total disk size
    uint64_t used = 0;        ///< Used disk space
    uint64_t free = 0;        ///< Free disk space
};

/**
 * @brief Network interface statistics.
 */
struct NetworkStats {
    std::string interface;    ///< Network interface name
    uint64_t rx_bytes = 0;    ///< Received bytes
    uint64_t tx_bytes = 0;    ///< Transmitted bytes
};

/**
 * @brief Battery status information.
 */
struct BatteryInfo {
    bool present = false;          ///< Battery is present or not
    float percentage = 0.0f;       ///< Charge level (0–100%)

    bool charging = false;         ///< Charging state
    bool discharging = false;      ///< Discharging state

    uint64_t energy_now = 0;       ///< Current energy (µWh or mWh)
    uint64_t energy_full = 0;      ///< Full capacity

    int power_now = 0;             ///< Current power draw (µW or mW)

    int time_remaining_sec = -1;   ///< Estimated remaining time (-1 if unknown)
};

} // namespace vsys
