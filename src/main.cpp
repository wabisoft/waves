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
 	// int blah [10] = {1, 2, 5, 7, 9}; 
	// size_t blah_size = 5;
	// std::cout << "Pre-sort" << std::endl;
	// print_array(blah, blah_size);
 	// insertion_sort(blah, blah_size);
	// std::cout << "Post-sort" << std::endl;
	// print_array(blah, blah_size);
	// sorted_insert(4, blah, blah_size);
	// std::cout << "Insert 4" << std::endl;
	// print_array(blah, blah_size);
	return 0;
}
