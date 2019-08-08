#pragma once

#include <iostream>
#include "prelude.hpp"


template <typename Container>
void print_container(Container container) {
	std::cout <<  "{ ";
	for (auto && item : container) {
		std::cout << item << " ";
	}
	std::cout << "}" << std::endl;
}

template <typename T>
void print_array(T*t, size_t count) {
	std::cout <<  "{ ";
	for (size_t i = 0; i< count; ++i)
 		std::cout << t[i] << " ";
	std::cout << "}" << std::endl;
}


std::ostream& operator<<(std::ostream& os, const Vector2& v);
std::ostream& operator<<(std::ostream& os, const AABB& aabb);


