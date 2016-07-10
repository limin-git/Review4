#pragma once


struct SubTime
{
    size_t hour;
    size_t minute;
    size_t second;
    size_t millisecond;
    size_t milliseconds;
};

inline bool operator < ( const SubTime& l, const SubTime& r )
{
    return
        ( l.hour < r.hour ) ||
        ( l.hour == r.hour && l.minute < r.minute ) ||
        ( l.hour == r.hour && l.minute == r.minute && l.second < r.second ) ||
        ( l.hour == r.hour && l.minute == r.minute && l.second == r.second && l.millisecond < r.millisecond )
        ;
}
