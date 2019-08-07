#pragma once

#include <iostream>
#include "vector2.hpp"


template <typename Container>
void print_container(Container container) {
	std::cout <<  "{ ";
	for (auto && item : container) {
		std::cout << item << " ";
	}
	std::cout << "}" << std::endl;
}

std::ostream& operator<<(std::ostream& os, Vector2& v) {
	return os << "(" << v[0] << ", " << v[1] << ")";
}

