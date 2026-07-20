#include <string>
#ifdef __linux__
#include "../util.hpp"
#include "Sensors.hpp"
#include <cstdint>
#include <fstream>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

class LinuxSysinfo {
public:
  struct sysinfo m_sysinfo;

  LinuxSysinfo *reload() {
    struct sysinfo info{};
    sysinfo(&info);
    m_sysinfo = info;
    return this;
  }

  explicit LinuxSysinfo() { reload(); }

  uint64_t totalRam() { return m_sysinfo.totalram; }
  uint64_t usedRam() { return totalRam() - m_sysinfo.freeram; }
  uint64_t usedSwap() { return m_sysinfo.totalswap - m_sysinfo.freeswap; }
};

/** === RAM sensors === */

uint64_t getPageSize() { return sysconf(_SC_PAGE_SIZE); }

unsigned long getTotalRAM() {
  struct sysinfo info{};
  sysinfo(&info);

  return info.totalram;
}

RamStats getRamStats() {
  LinuxSysinfo info;
  return {.totalBytes = info.totalRam(),
          .usedBytes = info.usedRam(),
          .swapBytes = info.usedSwap()};
}

std::vector<Stat> getDetailedRamStats() {
  auto ram = getTotalRAM();
  LinuxSysinfo info;

  auto pagesize = getPageSize();

  return {{"Total physical RAM", "GiB", bytesToGiB(ram)},
          {"Page size", "bytes", static_cast<double>(pagesize)},
          {"Total used memory", "GiB", bytesToGiB(info.usedRam())},
          {"Swapped memory", "GiB", bytesToGiB(info.usedSwap())}};
}

/** === CPU SENSORS === */

/** @brief Reads the first line of /proc/stat to retrieve CPU tick information.
 *  @return the first line of said file. */
std::string readCpuTickInfo() {
    std::ifstream procStat("/proc/stat");
    std::string line;
    std::getline(procStat, line);
    procStat.close();
    return line;
}

CpuTickStats getCpuStats() {
    CpuTickStats stats{};
    std::string statsStr = readCpuTickInfo();
    std::vector<std::string> tokens;
    std::istringstream stream(statsStr);
    std::string lbl;
    stream >> lbl >> stats.user >> stats.nice >> stats.system >> stats.idle;
    return stats;
}

#endif
