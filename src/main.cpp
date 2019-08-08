#include <iostream>
#include "clock.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "printing.hpp"
#include "stage.hpp"
#include "util.hpp"
#include "vector2.hpp"

int main(){
	Stage stage(13.3f);
	runStage(stage);
 	// int blah [10] =  {};
	// size_t blah_size = 0;
	// std::cout << "Empty" << std::endl;
	// print_array(blah, blah_size);

	// std::cout << "Inserting" << std::endl;
	// sorted_insert(1, blah, blah_size);
	// print_array(blah, blah_size);

	// sorted_insert(2, blah, blah_size);
	// print_array(blah, blah_size);


	// sorted_insert(8, blah, blah_size);
	// print_array(blah, blah_size);

	// sorted_insert(6, blah, blah_size);
	// print_array(blah, blah_size);

	// sorted_insert(3, blah, blah_size);
	// print_array(blah, blah_size);

	// std::cout << "Pre-sort" << std::endl;
	// print_array(blah, blah_size);
 	// insertion_sort(blah, blah_size);
	// std::cout << "Post-sort" << std::endl;
	// print_array(blah, blah_size);
	// sorted_insert(4, blah, blah_size);
	// std::cout << "Insert 4" << std::endl;
	// print_array(blah, blah_size);
	// std::cout << "4 is at index: ";
	// std::cout << find_where(4, blah, blah_size, [](int a, int b) { return a==b; }) << std::endl;
	// std::cout << "10 is at index: ";
	// std::cout << find_where(10, blah, blah_size, [](int a, int b) { return a==b; }) << std::endl;
	// std::cout << "1 is at index: ";
	// std::cout << find_where(1, blah, blah_size, [](int a, int b) { return a==b; }) << std::endl;
	// std::cout << "6 is at index: ";
	// std::cout << find_where(6, blah, blah_size, [](int a, int b) { return a==b; }) << std::endl;
	// std::cout << SIZE_MAX << std::endl;
	return 0;
}
