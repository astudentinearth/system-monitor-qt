#ifdef __linux__ 

#include <cstddef>
#include <cstdint>


#include <unistd.h>

uint64_t getPageSize() {
    return sysconf(_SC_PAGE_SIZE);
}


#endif
