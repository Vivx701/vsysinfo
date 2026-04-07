#include <vsys/VSystemInfo.hpp>

namespace vsys {

// ================= Factory =================

VSystemInfo VSystemInfo::create() {
    VSystemInfo sys;
    sys.backend = BackendFactory::create();
    return sys;
}

// ================= Refresh =================

void VSystemInfo::refresh() {
    refresh_cpu();
    refresh_memory();
    refresh_disk();
    refresh_network();
    refresh_battery();
}

void VSystemInfo::refresh_cpu() {
    if (!backend) return;

    cpu_.prev_total = cpu_.curr_total;
    cpu_.curr_total = backend->read_total_cpu();

    auto prev = cpu_.prev_total;
    auto curr = cpu_.curr_total;

    uint64_t prev_idle = prev.idle + prev.iowait;
    uint64_t curr_idle = curr.idle + curr.iowait;

    uint64_t prev_total = prev.user + prev.nice + prev.system +
                          prev.idle + prev.iowait + prev.irq + prev.softirq;

    uint64_t curr_total = curr.user + curr.nice + curr.system +
                          curr.idle + curr.iowait + curr.irq + curr.softirq;

    uint64_t delta_total = curr_total - prev_total;
    uint64_t delta_idle  = curr_idle - prev_idle;

    if (delta_total == 0) {
        cpu_.total_usage = 0.0f;
        return;
    }

    cpu_.total_usage = 100.0f * (1.0f - (float)delta_idle / delta_total);

    // ===== Per-core =====
    cpu_.prev_cores = cpu_.curr_cores;
    cpu_.curr_cores = backend->read_per_cpu();

    cpu_.per_core_usage.clear();

    size_t count = cpu_.curr_cores.size();
    cpu_.per_core_usage.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        if (i >= cpu_.prev_cores.size()) {
            cpu_.per_core_usage.push_back(0.0f);
            continue;
        }

        const auto& p = cpu_.prev_cores[i];
        const auto& c = cpu_.curr_cores[i];

        uint64_t p_idle = p.idle + p.iowait;
        uint64_t c_idle = c.idle + c.iowait;

        uint64_t p_total = p.user + p.nice + p.system +
                           p.idle + p.iowait + p.irq + p.softirq;

        uint64_t c_total = c.user + c.nice + c.system +
                           c.idle + c.iowait + c.irq + c.softirq;

        uint64_t d_total = c_total - p_total;
        uint64_t d_idle  = c_idle - p_idle;

        if (d_total == 0) {
            cpu_.per_core_usage.push_back(0.0f);
        } else {
            float usage = 100.0f * (1.0f - (float)d_idle / d_total);
            cpu_.per_core_usage.push_back(usage);
        }
    }
}

void VSystemInfo::refresh_memory() {
    if (!backend) return;
    memory_ = backend->read_memory();
}

void VSystemInfo::refresh_disk() {
    if (!backend) return;
    disks_ = backend->read_disks();
}

void VSystemInfo::refresh_network() {
    if (!backend) return;
    networks_ = backend->read_network();
}

void VSystemInfo::refresh_battery() {
    if (!backend) return;
    battery_ = backend->read_battery();
}

// ================= CPU =================

float VSystemInfo::cpu_usage() const {
    return cpu_.total_usage;
}

const std::vector<float>& VSystemInfo::per_cpu_usage() const {
    return cpu_.per_core_usage;
}

const CpuData& VSystemInfo::cpu() const {
    return cpu_;
}

// ================= Memory =================

uint64_t VSystemInfo::total_memory() const {
    return memory_.total;
}

uint64_t VSystemInfo::used_memory() const {
    return memory_.used;
}

uint64_t VSystemInfo::free_memory() const {
    return memory_.free;
}

const MemoryData& VSystemInfo::memory() const {
    return memory_;
}

// ================= Disk =================

const std::vector<DiskInfo>& VSystemInfo::disks() const {
    return disks_;
}

// ================= Network =================

const std::vector<NetworkStats>& VSystemInfo::networks() const {
    return networks_;
}

// ================= Battery =================

const BatteryInfo& VSystemInfo::battery() const {
    return battery_;
}

} // namespace vsys
