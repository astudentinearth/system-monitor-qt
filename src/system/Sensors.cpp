
#include <ostream>
#include "Sensors.hpp"
std::ostream& operator<<(std::ostream& os, const Stat& stat) {
    os << stat.label << " " << stat.value << " " << stat.unit;
    return os;
}
