cmake_minimum_required(VERSION 3.16)

project(vsysinfo VERSION 0.1 LANGUAGES CXX)

# ================= C++ Standard =================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ================= Options =================
option(BUILD_TESTS "Build tests" ON)

# ================= Library =================
add_library(vsysinfo SHARED
    src/core/VSystemInfo.cpp
    src/core/BackendFactory.cpp

    src/platform/WindowsBackend.cpp
    src/platform/LinuxBackend.cpp
)

target_include_directories(vsysinfo
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Platform-specific linking
if(WIN32)
    set_target_properties(vsysinfo PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_link_libraries(vsysinfo PRIVATE iphlpapi)
endif()

# ================= GoogleTest =================
if(BUILD_TESTS)

    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )

    # Prevent override of compiler flags on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    # ================= Test Executable =================
    add_executable(vsys_tests
        tests/test_vsysteminfo.cpp
    )

    target_link_libraries(vsys_tests
        PRIVATE
        vsysinfo
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(vsys_tests)

endif()
