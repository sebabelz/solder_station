#ifndef SOLDER_STATION_CHANNEL_H
#define SOLDER_STATION_CHANNEL_H
#include <cstdint>
#include <ostream>


enum class Channel: uint8_t
{
    Inactive = 0,
    Zero = 1,
    One = 2,
    Two = 4,
    Three = 8,
};

Channel operator|(Channel lhs, Channel rhs);
std::ostream &operator<<(std::ostream &os, const Channel &ch);

#endif //SOLDER_STATION_CHANNEL_H
