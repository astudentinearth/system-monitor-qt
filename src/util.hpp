
#include <cstdint>

inline double bytesToMiB(uint64_t bytes) {
    return ((double)bytes / 1024) / 1024;
}


inline double bytesToGiB(uint64_t bytes) {
    return bytesToMiB(bytes) / 1024;
}

