#pragma once

#include <cmath>
#include <stack>
#include <algorithm>

// Data Structures
template <typename T>
struct Stack : std::stack<T, std::vector<T>> {
	T rpop() {
		T t = this->top();
		this->pop();
		return t;
	}
};

