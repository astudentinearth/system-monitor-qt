
#include "Sensors.hpp"
#include <cstddef>
#include <cstdint>

#if defined(__APPLE__) || defined(__linux__)

#include <unistd.h>

uint64_t getPageSize() {
    return sysconf(_SC_PAGE_SIZE);
}

#else
uint64_t getPageSize() {
    return 0;
}
#endif

#ifdef __APPLE__

#include <sys/sysctl.h>
#include <sys/types.h>
#include <mach/vm_types.h>
#include <mach/vm_statistics.h>
#include <mach/message.h>
#include <mach/mach.h>
#include <mach/mach_syscalls.h>

unsigned long getTotalRAM() {
  unsigned long memsize = 0;
  size_t length = sizeof(memsize);

  if (sysctlbyname("hw.memsize", &memsize, &length, nullptr, 0) != 0) {
    return 0;
  }
  return memsize;
}

vm_statistics64_data_t getVmStats_mac() {
    vm_statistics64_data_t data;
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

    if(KERN_SUCCESS != host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&data, &count)) {
        return data;
    }
    return data;
}

#else
unsigned long getTotalRAM() { return 0; }
#endif

RamStats getRamStats() {
    RamStats stats;
    stats.pageSize = getPageSize();
    stats.totalPhysicalRam = getTotalRAM();

    #ifdef __APPLE__
    auto vmstats = getVmStats_mac();   
    stats.activePages = vmstats.active_count;
    stats.inactivePages = vmstats.inactive_count;
    stats.freePages = vmstats.free_count + vmstats.purgeable_count;
    stats.otherUsedPages = vmstats.wire_count + vmstats.compressor_page_count;
#endif

    return stats;
}

uint64_t RamStats::getAvailableVirtualMemoryBytes() {
    return this->pageSize * this->freePages;
}

