

#pragma once
#include <cstdint>
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
