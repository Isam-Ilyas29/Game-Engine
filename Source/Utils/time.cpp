#include "time.h"



Time Time::setMilliseconds(int milli_seconds) {
	return Time(int(milli_seconds) * 1000);
}
Time Time::setSeconds(float seconds) {
	seconds = seconds * 1000.0f * 1000.0f;
	return Time(int(seconds));
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

float Time::getSeconds() const {
	return float(micro_seconds.count()) / (1000.f * 1000.f);
}

float Time::getMilliseconds() const {
	return float(micro_seconds.count()) / (1000);
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
