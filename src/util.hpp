#pragma once

#include <cmath>
#include <stack>
#include <algorithm>


// Data Structures
template <typename T>
struct Stack : std::stack<T> {
	T rpop() {
		T t = top();
		pop();
		return t;
	}
};

