#pragma once

class NonMovable {
public:
	NonMovable() = default;
	virtual ~NonMovable() = default;

	NonMovable(const NonMovable&) = delete;
	NonMovable& operator=(const NonMovable&) = delete;

	NonMovable(NonMovable&&) = delete;
	NonMovable& operator=(NonMovable&&) = delete;
};