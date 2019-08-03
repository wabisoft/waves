#include <iostream>

#include "wabi/vector2.hpp"

int main(){
	wabi::Vector2 v(0, 0);
	wabi::Vector2 v1(1, 1);
	std::cout << v << std::endl;
	std::cout << v.dot(v1) << std::endl; 
	std::cout << v1.magnitude() << std::endl; 
	std::cout << v1.normalized() << std::endl; 
	std::cout << v1 << std::endl;
	v1.normalize();
	std::cout << v1 << std::endl;
	std::cout << v1.magnitude() << std::endl; 
	return 0;
}
