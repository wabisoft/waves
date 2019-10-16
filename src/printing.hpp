#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>

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


std::ostream& operator<<(std::ostream& os, glm::vec2 v);
std::ostream& operator<<(std::ostream& os, AABB aabb);

std::ostream& operator<<(std::ostream& os, sf::Vector2f v);


