//
// Created by 屋顶上的小丑 on 2023/4/4.
//
#include <iostream>
#include "map.hpp"

template<typename T>
void print_type(T x) {
    if (sjtu::my_type_traits<T>::iterator_assignable::value)
        std::cout << "the iterator of key: " << x->first << " is assignable!\n";
    else
        std::cout << "the iterator of key: " << x->first << " isn't assignable!\n";
}

template<typename T, typename U>
void compare_types(T x, U y) {
    if (sjtu::my_is_same<T, U>::value)
        std::cout << "the iterator of key: " << x->first << " and " << "the iterator of key: " << y->first
                  << " have the same type.\n";
    else
        std::cout << "the iterator of key: " << x->first << " and " << "the iterator of key: " << y->first
                  << " have different type.\n";
}

int main() {
    sjtu::map<int, long long> map1;
    sjtu::map<long long, int> map2;
    for (int i = 0; i < 10; i++) {
        map1[i] = i;
        map2[i] = i;
    }
    auto it1 = map1.begin();
    auto it2 = --map2.cend();
    print_type(it1);
    print_type(it2);
    compare_types(it1, it2);
    compare_types(it1, --map1.end());
    return 0;
}