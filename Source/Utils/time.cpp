#include "time.hpp"


Time Time::setMilliseconds(s32 milli_seconds) {
	return Time(s64(milli_seconds) * 1000);
}
Time Time::setSeconds(f32 seconds) {
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

Time& Time::operator+=(Time const& rhs) {
	micro_seconds += rhs.micro_seconds;
	return *this;
}
Time& Time::operator-=(Time const& rhs) {
	micro_seconds -= rhs.micro_seconds;
	return *this;
}
Time& Time::operator*=(Time const& rhs) {
	micro_seconds *= rhs.micro_seconds.count();
	return *this;
}
Time& Time::operator/=(Time const& rhs) {
	micro_seconds = (rhs.micro_seconds == micro_seconds.zero()) ? micro_seconds.zero() : micro_seconds /= rhs.micro_seconds.count();
	return *this;
}

bool Time::operator==(Time const& rhs) {
	return micro_seconds == rhs.micro_seconds;
}
bool Time::operator!=(Time const& rhs) {
	return !(*this == rhs);
}

bool Time::operator<(Time const& rhs) {
	return micro_seconds < rhs.micro_seconds;
}
bool Time::operator<=(Time const& rhs) {
	return micro_seconds <= rhs.micro_seconds;
}
bool Time::operator>(Time const& rhs) {
	return micro_seconds > rhs.micro_seconds;
}
bool Time::operator>=(Time const& rhs) {
	return micro_seconds >= rhs.micro_seconds;
}

f32 Time::getSeconds() const {
	return f32(micro_seconds.count()) / (1000.f * 1000.f);
}

f32 Time::getMilliseconds() const {
	return f32(micro_seconds.count()) / (1000);
}

float Time::getMicroseconds() const {
	return micro_seconds.count();
}

Time operator+(Time const& lhs, Time const& rhs) {
	return Time(lhs) -= rhs;
}

Time operator-(Time const& lhs, Time const& rhs) {
	return Time(lhs) -= rhs;
}

Time operator*(Time const& lhs, Time const& rhs) {
	return Time(lhs) *= rhs;
}

Time operator/(Time const& lhs, Time const& rhs) {
	return Time(lhs) /= rhs;
}
