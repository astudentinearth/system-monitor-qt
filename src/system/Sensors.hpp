
#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Stat {
    std::string label;
    std::string unit;
    double value;
};
std::ostream& operator<<(std::ostream& os, const Stat& stat);
struct RamStats {
  uint64_t pageSize;
  uint64_t totalPhysicalRam;
  uint64_t activePages;
  uint64_t freePages;
  uint64_t inactivePages;
  uint64_t otherUsedPages;
  uint64_t usedPages;
  uint64_t getAvailableVirtualMemoryBytes() {
    return this->pageSize * this->freePages;
  }
  uint64_t getUsedRam() { return (this->usedPages) * pageSize; }
};
unsigned long getTotalRAM();

RamStats getRamStats();
std::vector<Stat> getRamStats2();
