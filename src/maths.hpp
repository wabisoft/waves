#pragma once

#include <cmath>


template <typename T>
int sign(T t) {
	return t / std::abs(t);
}
