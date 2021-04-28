#pragma once

#include <time.h> //struct timespec、clock_gettime

/**
 * 打点计时器
 */
namespace TickerTapTimer
{

using TimestampType= long;
template<typename Resolution = std::chrono::nanoseconds>
class ChronoClock
{
public:

	static TimestampType Now()
	{
		return std::chrono::duration_cast<Resolution>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}
};

#ifndef _WIN32
class SyscallClock
{
public:
	static TimestampType Now()
	{
		struct timespec t;
		memset(&t, 0x00, sizeof(t));
		clock_gettime(CLOCK_REALTIME, &t);
		return timespec2nsec(t);
	}
	static inline long timespec2nsec(struct timespec& t)
	{
		return t.tv_sec * 1000000000L + t.tv_nsec;
	}
};
#else
using SyscallClock = ChronoClock<>;
#endif

/**
 * 打点
 */
template<typename ClockType = SyscallClock>
class CTicker
{
public:

	CTicker()
			: m_Timestamp(ClockType::Now())
	{

	}
	operator int() const
	{
		return m_Timestamp;
	}
	operator std::string()
	{
		return std::to_string(m_Timestamp);
	}
	std::string Str()
	{
		return std::to_string(m_Timestamp);
	}

	TimestampType& GetTimestamp()
	{
		return m_Timestamp;
	}
	TimestampType operator-(CTicker& Other)
	{
		return m_Timestamp - Other.GetTimestamp();
	}
private:
	TimestampType m_Timestamp; // 时间戳
};

}


