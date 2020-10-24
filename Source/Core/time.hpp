#pragma once

#include "Core/std_types.hpp"

#include <chrono>


class Time
{
private:
	std::chrono::microseconds micro_seconds;

public:
	static Time milliseconds(s32 milliSeconds);
	static Time seconds(f32 seconds);

	static Time now();

	constexpr Time() noexcept : micro_seconds(0) {}
	explicit constexpr Time(int micro_seconds) noexcept : micro_seconds(micro_seconds) {}
	explicit constexpr Time(std::chrono::microseconds micro_seconds) noexcept : micro_seconds(micro_seconds) {}

	Time& operator-();
	Time& operator+=(const Time& rhs);
	Time& operator-=(const Time& rhs);
	Time& operator*=(const Time& rhs);
	Time& operator/=(const Time& rhs);

	bool operator==(const Time& rhs);
	bool operator!=(const Time& rhs);
	bool operator<(const Time& rhs);
	bool operator<=(const Time& rhs);
	bool operator>(const Time& rhs);
	bool operator>=(const Time& rhs);

	f32 getSeconds() const;
	f32 getMilliseconds() const;
	f32 getMicroseconds() const;
};

Time operator+(const Time& lhs, const Time& rhs);
Time operator-(const Time& lhs, const Time& rhs);
Time operator*(const Time& lhs, const Time& rhs);
Time operator/(const Time& lhs, const Time& rhs);
