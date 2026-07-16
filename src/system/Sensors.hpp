

#pragma once
#include <cstdint>
struct RamStats {
    uint64_t pageSize;
    uint64_t totalPhysicalRam;
    uint64_t activePages;
    uint64_t freePages;
    uint64_t inactivePages;
    uint64_t otherUsedPages;
    uint64_t getAvailableVirtualMemoryBytes();
    uint64_t getUsedRam() {
        return (this->activePages + this->inactivePages + this->otherUsedPages) * pageSize;
    }
};
unsigned long getTotalRAM();

RamStats getRamStats();

