#include <cstring>
#include <mach/host_info.h>
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

/** === RAM Sensors === */

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
  std::memset(&data, 0, sizeof(data));
  mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

  if (KERN_SUCCESS != host_statistics64(mach_host_self(), HOST_VM_INFO64,
                                        (host_info64_t)&data, &count)) {
    return data;
  }
  return data;
}

uint64_t getUsedSwap() {
  struct xsw_usage swap;
  size_t size = sizeof(swap);

  if (sysctlbyname("vm.swapusage", &swap, &size, nullptr, 0) == 0) {
    return swap.xsu_used;
  }
  return 0;
}

RamStats getRamStats() {
  RamStats stats;
  stats.totalBytes = getTotalRAM();
  auto pagesize = getPageSize();

  auto vmstats = getVmStats_mac();
  stats.usedBytes = ((vmstats.internal_page_count - vmstats.purgeable_count) +
                     vmstats.wire_count + vmstats.compressor_page_count) *
                    pagesize;

  stats.swapBytes = getUsedSwap();

  return stats;
}

uint64_t getUsedPageCount(vm_statistics64_data_t vmstats) {
  return (vmstats.internal_page_count - vmstats.purgeable_count) +
         vmstats.wire_count + vmstats.compressor_page_count;
}

std::vector<Stat> getDetailedRamStats() {
  auto vmstats = getVmStats_mac();
  auto ram = getTotalRAM();
  auto pagesize = getPageSize();
  auto swap = getUsedSwap();

  return {{"Total physical RAM", "GiB", bytesToGiB(ram)},
          {"Page size", "bytes", static_cast<double>(pagesize)},
          {"Total used memory", "GiB",
           bytesToGiB(getUsedPageCount(vmstats) * pagesize)},
          {"-- App memory", "GiB",
           bytesToGiB((vmstats.internal_page_count - vmstats.purgeable_count) *
                      pagesize)},
          {"-- Wired memory", "GiB", bytesToGiB(vmstats.wire_count * pagesize)},
          {"-- Compressed memory", "GiB",
           bytesToGiB(vmstats.compressor_page_count * pagesize)},
          {"Swapped memory", "GiB", bytesToGiB(getUsedSwap())}};
}

/** === CPU sensors === */

bool getCpuTicks(uint64_t &user, uint64_t &system, uint64_t &idle,
                 uint64_t &nice) {

  host_cpu_load_info_data_t cpuInfo;
  std::memset(&cpuInfo, 0, sizeof(cpuInfo));

  mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;

  if (KERN_SUCCESS != host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO,
                                      reinterpret_cast<host_info_t>(&cpuInfo),
                                      &count)) {
    return false;
  }

  user = cpuInfo.cpu_ticks[CPU_STATE_USER];
  system = cpuInfo.cpu_ticks[CPU_STATE_SYSTEM];
  idle = cpuInfo.cpu_ticks[CPU_STATE_IDLE];
  nice = cpuInfo.cpu_ticks[CPU_STATE_NICE];

  return true;
}

CpuTickStats getCpuStats() {
    uint64_t user, system, idle, nice;
    if(!getCpuTicks(user, system, idle, nice)) {return {.active = 0, .idle = 0};}
    return {
        .active = user + system + nice,
        .idle = idle
    };
}

#endif
