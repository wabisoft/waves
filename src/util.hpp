#pragma once

#include <cmath>
#include <stack>
#include <algorithm>
#include "constants.hpp"


/**************
 * Algorithms *
 **************/
template <typename T, typename Iter, typename Pred>
void insertion_sort(Iter begin, Iter end, Pred predicate) {
	for (auto it = begin; it != end; ++it) {
		std::rotate(std::upper_bound(begin, it, *it, predicate), it, std::next(it));
	}
}

// template <typename T, typename It, typename Pred>
template <typename T, typename Pred>
typename std::vector<T>::iterator sorted_insert(std::vector<T>& v, T item, Pred predicate) {
#ifdef _DEBUG
	auto it = std::upper_bound<std::vector<T>::iterator, T>(v.begin(), v.end(), item, predicate);
#endif
	return v.insert(
#ifdef _DEBUG
		it,
#else
		std::upper_bound<std::vector<T>::iterator, T>(v.begin(), v.end(), item, predicate),
#endif
		item
	);
}

template <typename T>
typename std::vector<T>::iterator sorted_insert(std::vector<T>& v, T item) {
	return sorted_insert<T>(v, item, [] (const T& a, const T& b) { return a < b; });
}

template<typename T, typename Pred>
inline void insertion_sort(T* t, const int count, Pred predicate) {
	for(int i = 1; i<count; ++i) {
		for(int j = i; predicate(t[j], t[j-1]) && j>0; --j) {
			T swap = t[j];
			t[j] = t[j-1];
			t[j-1] = swap;
		}
	}
}

template<typename T>
inline void insertion_sort(T* t, const int count) {
	return insertion_sort(t, count, [](T a, T b) { return a<b; });
}

// template <typename T, typename Pred>
// inline void sorted_insert(T t_item, T* t, int & count, Pred predicate) {
// 	if (count == 0) {
// 		t[0] = t_item;
// 		++count;
// 		return;
// 	}
// 	for(int i = 0; i<count; ++i){
// 		if(predicate(t[i],t_item)) {
// 			if (i == count - 1) { // at the end of array
// 				t[i+1] = t_item;
// 				++count;
// 				return;
// 			}
// 			continue;
// 		} else {
// 			T swap;
// 			swap = t[i];
// 			t[i] = t_item;
// 			for (int j = i+1; j<count+1; ++j){
// 				T swap_swap = swap;
// 				swap = t[j];
// 				t[j] = swap_swap;
// 			}
// 			++count;
// 			break;
// 		}
// 	}
// }

// template <typename T>
// inline void sorted_insert(T t_item, T* t, int & count) {
// 	return sorted_insert(t_item, t, count, [](T a, T b) { return a<b; });
// }

template <typename T, typename BinaryPred>
inline int find_where(const T t_item, const T* const t, const int count, BinaryPred predicate) {
	for (int i = 0; i<count; ++i) {
		if(predicate(t[i], t_item)) {
			return i;
		}
	}
	return -1;
}


template <typename T>
inline int find_where(const T t_item, const T* const t, const int count) {
	return find_where(t_item, t, count, [](T a, T b) { return a==b; });
}

template <typename T, typename UnaryPred>
inline int find_where(const T* const t, const int count, UnaryPred predicate) {
	for (int i = 0; i<count; ++i) {
		if(predicate(t[i])) {
			return i;
		}
	}
	return -1;
}

template <typename T, typename U, typename Prop>
inline int binary_find_where(U search, const T* const t, const int count, Prop prop) {
	int left = 0;
	int right = count-1;
	while (left <= right) {
		int middle = (int)std::floor((left + right) / 2);
		U propRes = prop(t[middle]);
		if (propRes < search) {
			left = middle + 1;
		} else if (propRes > search) {
			right = middle - 1;
		} else {
			return middle;
		}
	}
	return -1;
}


// Data Structures
template <typename T>
struct Stack : std::stack<T> {
	T rpop() {
		T t = top();
		pop();
		return t;
	}
};

