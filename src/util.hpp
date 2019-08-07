#pragma once

#include <algorithm>

enum EntityType{		
	SEA 		= 1 << 0,
	WAVE 		= 1 << 1,
	PLATFORM 	= 1 << 2,
	ROCK 		= 1 << 3,
	SHIP		= 1 << 4,
};



// No predicate
template <typename Iter>
inline void insertion_sort(Iter begin, Iter end){
	for (auto it= begin; it != end; ++it){
		std::rotate(std::upper_bound(begin, it, *it), it, std::next(it));
	}
}

// With predicate
template <typename Iter, typename Predicate>
inline void insertion_sort(Iter begin, Iter end, Predicate predicate){
	for (auto it= begin; it != end; ++it){
		std::rotate(std::upper_bound(begin, it, *it, predicate), it, std::next(it));
	}
}

// No predicate
template<typename T, typename Container>
inline void sorted_insert(Container& container, T t){
	container.insert(std::upper_bound(container.begin(), container.end(), t), t);
}

// With predicate
template<typename T, typename Container, typename Predicate>
inline void sorted_insert(Container& container, T t, Predicate predicate){
	container.insert(std::upper_bound(container.begin(), container.end(), t, predicate), t);
}


template<typename T, typename Pred>
inline void insertion_sort(T* t, size_t count, Pred predicate) {
	for(size_t i = 1; i<count; ++i) {
		for(size_t j = i; predicate(t[j-1], t[j]) && j>0; --j) {
			T swap = t[j];
			t[j] = t[j-1];
			t[j-1] = swap;
		}
	}
}

template<typename T>
inline void insertion_sort(T* t, size_t count) {
	return insertion_sort(t, count, [](T a, T b) { return a>b; });
}
