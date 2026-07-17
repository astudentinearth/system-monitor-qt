#ifdef __APPLE__

#include "../util.hpp"
#include "Sensors.hpp"
#include <cstddef>
#include <cstdint>
#include <mach/mach.h>
#include <mach/mach_syscalls.h>
#include <mach/message.h>
#include <mach/vm_statistics.h>
#include <mach/vm_types.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <unistd.h>

uint64_t getPageSize() { return sysconf(_SC_PAGE_SIZE); }

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

  if (KERN_SUCCESS != host_statistics64(mach_host_self(), HOST_VM_INFO64,
                                        (host_info64_t)&data, &count)) {
    return data;
  }
  return data;
}

RamStats getRamStats() {
  RamStats stats;
  stats.pageSize = getPageSize();
  stats.totalPhysicalRam = getTotalRAM();

  auto vmstats = getVmStats_mac();
  stats.activePages = vmstats.active_count;
  stats.inactivePages = vmstats.inactive_count;
  stats.freePages = vmstats.free_count + vmstats.purgeable_count;
  stats.otherUsedPages = vmstats.wire_count + vmstats.compressor_page_count;
  stats.usedPages = (vmstats.internal_page_count - vmstats.purgeable_count) +
                    vmstats.wire_count + vmstats.compressor_page_count;

  return stats;
}

uint64_t getUsedPageCount(vm_statistics64_data_t vmstats) {
  return (vmstats.internal_page_count - vmstats.purgeable_count) +
         vmstats.wire_count + vmstats.compressor_page_count;
}

std::vector<Stat> getRamStats2() {
  auto vmstats = getVmStats_mac();
  auto ram = getTotalRAM();
  auto pagesize = getPageSize();

  return {{"Total physical RAM", "GiB", bytesToGiB(ram)},
          {"Page size", "bytes", static_cast<double>(pagesize)},
          {"Total used memory", "GiB",
           bytesToGiB(getUsedPageCount(vmstats) * pagesize)},
          {"-- App memory", "GiB",
           bytesToGiB((vmstats.internal_page_count - vmstats.purgeable_count) *
                      pagesize)},
          {"-- Wired memory", "GiB", bytesToGiB(vmstats.wire_count * pagesize)},
          {"-- Compressed memory", "GiB",
           bytesToGiB(vmstats.compressor_page_count * pagesize)}};
}

#endif
