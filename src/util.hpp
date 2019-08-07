#pragma once

#include <algorithm>

enum EntityType{		
	SEA 		= 1 << 0,
	WAVE 		= 1 << 1,
	PLATFORM 	= 1 << 2,
	ROCK 		= 1 << 3,
	SHIP		= 1 << 4,
};


template<typename T, typename Pred>
inline void insertion_sort(T* t, size_t count, Pred predicate) {
	for(size_t i = 1; i<count; ++i) {
		for(size_t j = i; predicate(t[j], t[j-1]) && j>0; --j) {
			T swap = t[j];
			t[j] = t[j-1];
			t[j-1] = swap;
		}
	}
}

template<typename T>
inline void insertion_sort(T* t, size_t count) {
	return insertion_sort(t, count, [](T a, T b) { return a<b; });
}

template <typename T, typename Pred>
inline void sorted_insert(T t_item, T* t, size_t& count, Pred predicate) {
	for(size_t i = 0; i<count; ++i){
		if(predicate(t[i],t_item))
			continue;
		else {
			T swap;
			swap = t[i];
			t[i] = t_item;
			for (size_t j = i+1; j<count+1; ++j){	
				T swap_swap = swap;
				swap = t[j];
				t[j] = swap_swap;
			}
			++count;
			break;
		}
	}
}

template <typename T>
inline void sorted_insert(T t_item, T* t, size_t& count) {
	return sorted_insert(t_item, t, count, [](T a, T b) { return a<b; });
}
