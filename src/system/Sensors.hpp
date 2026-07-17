
#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Stat {
  std::string label;
  std::string unit;
  double value;
};

std::ostream &operator<<(std::ostream &os, const Stat &stat);

struct RamStats {
  uint64_t totalBytes;
  uint64_t usedBytes;
  uint64_t swapBytes;
  uint64_t freeBytes() { return totalBytes - usedBytes; }
  double usedPercent() {
    if (totalBytes == 0)
      return 0.0;
    return static_cast<double>(usedBytes) * 100 / totalBytes;
  }
  double freePercent() { return 100.0 - freePercent(); }
};

struct CpuTickStats {
  uint64_t active() { return user + system + nice; }
  uint64_t user;
  uint64_t system;
  uint64_t nice;
  uint64_t idle;
  uint64_t total() { return active() + idle; }

  double percentAgainst(CpuTickStats other) {
    uint64_t dTotal = other.total() - total();
    if (dTotal == 0)
      return 0.0;
    uint64_t dActive = other.active() - active();
    return percent_(dActive, dTotal);
  }

  double userPercent(CpuTickStats other) {
    uint64_t dTotal = other.total() - total();
    if (dTotal == 0)
      return 0.0;
    uint64_t dUser = other.user - user;
    return percent_(dUser, dTotal);
  }

  double sysPercent(CpuTickStats other) {
    uint64_t dTotal = other.total() - total();
    if (dTotal == 0)
      return 0.0;
    uint64_t dSystem = other.system - system;
    return percent_(dSystem, dTotal);
  }

  double nicePercent(CpuTickStats other) {
    uint64_t dTotal = other.total() - total();
    if (dTotal == 0)
      return 0.0;
    uint64_t dNice = other.nice - nice;
    return percent_(dNice, dTotal);
  }

  std::vector<Stat> statAgainst(CpuTickStats other) {
    return {{"CPU usage", "", percentAgainst(other)},
            {"    %user", "", userPercent(other)},
            {"    %system", "", sysPercent(other)},
            {"    %nice", "", nicePercent(other)},
            {"    %idle", "", 100 - percentAgainst(other)}};
  }

private:
  double percent_(uint64_t dTarget, uint64_t dTotal) {
    return static_cast<double>(dTarget) / dTotal * 100;
  }
};

unsigned long getTotalRAM();

RamStats getRamStats();
std::vector<Stat> getDetailedRamStats();
CpuTickStats getCpuStats();
