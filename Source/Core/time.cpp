#include "Core/time.hpp"


Time Time::milliseconds(s32 milli_seconds) {
	return Time(s64(milli_seconds) * 1000);
}
Time Time::seconds(f32 seconds) {
	seconds = seconds * 1000.0f * 1000.0f;
	return Time(s32(seconds));
}

Time Time::now() {
	return Time(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}

Time& Time::operator-() {
	micro_seconds = -micro_seconds;
	return *this;
}

Time& Time::operator+=(const Time& rhs) {
	micro_seconds += rhs.micro_seconds;
	return *this;
}
Time& Time::operator-=(const Time& rhs) {
	micro_seconds -= rhs.micro_seconds;
	return *this;
}
Time& Time::operator*=(const Time& rhs) {
	micro_seconds *= rhs.micro_seconds.count();
	return *this;
}
Time& Time::operator/=(const Time& rhs) {
	micro_seconds = (rhs.micro_seconds == micro_seconds.zero()) ? micro_seconds.zero() : micro_seconds /= rhs.micro_seconds.count();
	return *this;
}

bool Time::operator==(const Time& rhs) {
	return micro_seconds == rhs.micro_seconds;
}
bool Time::operator!=(const Time& rhs) {
	return !(*this == rhs);
}

bool Time::operator<(const Time& rhs) {
	return micro_seconds < rhs.micro_seconds;
}
bool Time::operator<=(const Time& rhs) {
	return micro_seconds <= rhs.micro_seconds;
}
bool Time::operator>(const Time& rhs) {
	return micro_seconds > rhs.micro_seconds;
}
bool Time::operator>=(const Time& rhs) {
	return micro_seconds >= rhs.micro_seconds;
}

f32 Time::getSeconds() const {
	return f32(micro_seconds.count()) / (1000.f * 1000.f);
}

f32 Time::getMilliseconds() const {
	return f32(micro_seconds.count()) / (1000);
}

f32 Time::getMicroseconds() const {
	return micro_seconds.count();
}

Time operator+(const Time& lhs, const Time& rhs) {
	return Time(lhs) -= rhs;
}

Time operator-(const Time& lhs, const Time& rhs) {
	return Time(lhs) -= rhs;
}

Time operator*(const Time& lhs, const Time& rhs) {
	return Time(lhs) *= rhs;
}

Time operator/(const Time& lhs, const Time& rhs) {
	return Time(lhs) /= rhs;
}
