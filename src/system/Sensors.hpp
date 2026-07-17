
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
    uint64_t totalBytes;
    uint64_t usedBytes;
    uint64_t swapBytes;
    uint64_t freeBytes() { return totalBytes - usedBytes; }
    double usedPercent() {
        if(totalBytes == 0) return 0.0;
        return static_cast<double>(usedBytes) * 100 / totalBytes;
    }
    double freePercent() { return 100.0 - freePercent(); }
};

struct CpuTickStats {
    uint64_t active;
    uint64_t idle;
    uint64_t total() { return active + idle; }
    double percentAgainst(CpuTickStats other) {
        uint64_t dTotal = other.total() - total();
        if(dTotal == 0 ) return 0.0;
        uint64_t dActive = other.active - active;
        uint64_t dIdle = other.idle - idle;

        return static_cast<double>(dActive) / dTotal * 100;
    }
};

unsigned long getTotalRAM();

RamStats getRamStats();
std::vector<Stat> getDetailedRamStats();
CpuTickStats getCpuStats();
