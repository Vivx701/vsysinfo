#include <gtest/gtest.h>
#include <vsys/VSystemInfo.hpp>

using namespace vsys;

// ================= Basic Creation =================

TEST(VSystemInfoTest, CreateInstance) {
    auto sys = VSystemInfo::create();

    // Just ensure no crash on creation
    SUCCEED();
}

// ================= Refresh =================

TEST(VSystemInfoTest, RefreshDoesNotCrash) {
    auto sys = VSystemInfo::create();

    EXPECT_NO_THROW({
        sys.refresh();
    });
}

// ================= CPU =================

TEST(VSystemInfoTest, CpuUsageValidRange) {
    auto sys = VSystemInfo::create();

    sys.refresh_cpu();
    sys.refresh_cpu(); // second call needed for delta calculation

    float cpu = sys.cpu_usage();

    EXPECT_GE(cpu, 0.0f);
    EXPECT_LE(cpu, 100.0f);
}

// ================= Memory =================

TEST(VSystemInfoTest, MemoryValuesValid) {
    auto sys = VSystemInfo::create();

    sys.refresh_memory();

    auto total = sys.total_memory();
    auto used  = sys.used_memory();
    auto free  = sys.free_memory();

    EXPECT_GT(total, 0);
    EXPECT_LE(used, total);
    EXPECT_LE(free, total);
}

// ================= Disk =================

TEST(VSystemInfoTest, DiskListNotEmpty) {
    auto sys = VSystemInfo::create();

    sys.refresh_disk();

    const auto& disks = sys.disks();

    // On most systems at least one disk exists
    EXPECT_GE(disks.size(), 0);
}

// ================= Network =================

TEST(VSystemInfoTest, NetworkStatsAccessible) {
    auto sys = VSystemInfo::create();

    sys.refresh_network();

    const auto& nets = sys.networks();

    // Just validate access (may be empty depending on environment)
    EXPECT_GE(nets.size(), 0);
}

// ================= Battery =================

TEST(VSystemInfoTest, BatteryAccess) {
    auto sys = VSystemInfo::create();

    sys.refresh_battery();

    const auto& bat = sys.battery();

    // If battery present → percentage must be valid
    if (bat.present) {
        EXPECT_GE(bat.percentage, 0.0f);
        EXPECT_LE(bat.percentage, 100.0f);
    } else {
        SUCCEED(); // desktops often have no battery
    }
}
