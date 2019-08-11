#pragma once

#include <cmath>
#include <algorithm>
#include "constants.hpp"


template<typename T, typename Pred>
inline void insertion_sort(T* t, const size_t count, Pred predicate) {
	for(size_t i = 1; i<count; ++i) {
		for(size_t j = i; predicate(t[j], t[j-1]) && j>0; --j) {
			T swap = t[j];
			t[j] = t[j-1];
			t[j-1] = swap;
		}
	}
}

template<typename T>
inline void insertion_sort(T* t, const size_t count) {
	return insertion_sort(t, count, [](T a, T b) { return a<b; });
}

template <typename T, typename Pred>
inline void sorted_insert(T t_item, T* t, size_t& count, Pred predicate) {
	if (count == 0) {
		t[0] = t_item;
		++count;
		return;
	}
	for(size_t i = 0; i<count; ++i){
		if(predicate(t[i],t_item)) {
			if (i == count - 1) { // at the end of array
				t[i+1] = t_item;
				++count;
				return;
			}
			continue;
		} else {
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

template <typename T, typename BinaryPred>
inline size_t find_where(const T t_item, const T* const t, const size_t count, BinaryPred predicate) {
	for (size_t i = 0; i<count; ++i) {
		if(predicate(t[i], t_item)) { 
			return i;
		}
	}
	return SIZE_MAX;
}


template <typename T>
inline size_t find_where(const T t_item, const T* const t, const size_t count) {
	return find_where(t_item, t, count, [](T a, T b) { return a==b; });
}

template <typename T, typename UnaryPred>
inline size_t find_where(const T* const t, const size_t count, UnaryPred predicate) {
	for (size_t i = 0; i<count; ++i) {
		if(predicate(t[i])) { 
			return i;
		}
	}
	return SIZE_MAX;
}

template <typename T, typename U, typename Prop>
inline size_t binary_find_where(U search, const T* const t, const size_t count, Prop prop) {	
	size_t left = 0;
	size_t right = count-1;
	while (left <= right) {
		size_t middle = (size_t)std::floor((left + right) / 2);
		U propRes = prop(t[middle]);
		if (propRes < search) {
			left = middle + 1;
		} else if (propRes > search) {
			right = middle - 1;
		} else {
			return middle;
		}
	}
	return SIZE_MAX;
}
