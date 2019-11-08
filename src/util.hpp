#pragma once

#include <cmath>
#include <stack>
#include <algorithm>


// template <typename T>
// T rpop(std::stack<T> stk) {
// 	T t = stk.top();
// 	stk.pop();
// 	return t;
// }

// Data Structures
template <typename T>
struct Stack : std::stack<T, std::vector<T>> {
	T rpop() {
		T t = this->top();
		this->pop();
		return t;
	}
};

