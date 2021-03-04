#include "Channel.h"

Channel operator|(Channel lhs, Channel rhs)
{
    return static_cast<Channel> (static_cast<std::underlying_type<Channel>::type>(lhs) |
                                 static_cast<std::underlying_type<Channel>::type>(rhs));
}

std::ostream &operator<<(std::ostream &os, const Channel &ch)
{
    int number;
    switch (ch)
    {
        case Channel::Zero :
            number = 0;
            break;
        case Channel::One :
            number = 1;
            break;
        case Channel::Two :
            number = 2;
            break;
        case Channel::Three :
            number = 3;
            break;
        default:
            number = -1;
            break;
    }
    os << number;
    return os;
}



