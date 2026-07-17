
#include "util.hpp"
#include <sys/ioctl.h> 
#include <unistd.h>

uint16_t getTermWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

