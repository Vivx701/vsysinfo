#include <vsys/Backend.hpp>

#ifdef _WIN32
    #include "../platform/WindowsBackend.hpp"
#else
    #include "../platform/LinuxBackend.hpp"
#endif

namespace vsys {

std::unique_ptr<IPlatformBackend> BackendFactory::create() {

#ifdef _WIN32
    return std::make_unique<WindowsBackend>();

#elif defined(__linux__)
    return std::make_unique<LinuxBackend>();

#else
    // Unsupported platform
    return nullptr;

#endif

}

} // namespace vsys
