#pragma once

#include "Core/std_types.hpp"

#include <chrono>


class Time
{
private:
	std::chrono::microseconds micro_seconds;

public:
	static Time setMilliseconds(s32 milliSeconds);
	static Time setSeconds(f32 seconds);

	static Time now();

	constexpr Time() noexcept : micro_seconds(0) {}
	explicit constexpr Time(int micro_seconds) noexcept : micro_seconds(micro_seconds) {}
	explicit constexpr Time(std::chrono::microseconds micro_seconds) noexcept : micro_seconds(micro_seconds) {}

	Time& operator-();
	Time& operator+=(Time const& rhs);
	Time& operator-=(Time const& rhs);
	Time& operator*=(Time const& rhs);
	Time& operator/=(Time const& rhs);

	bool operator==(Time const& rhs);
	bool operator!=(Time const& rhs);
	bool operator<(Time const& rhs);
	bool operator<=(Time const& rhs);
	bool operator>(Time const& rhs);
	bool operator>=(Time const& rhs);

	f32 getSeconds() const;
	f32 getMilliseconds() const;
	f32 getMicroseconds() const;
};

Time operator+(Time const& lhs, Time const& rhs);
Time operator-(Time const& lhs, Time const& rhs);
Time operator*(Time const& lhs, Time const& rhs);
Time operator/(Time const& lhs, Time const& rhs);
