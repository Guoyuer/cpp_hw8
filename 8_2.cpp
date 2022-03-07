//
// Created by Tmp on 3/6/2022.
//

#include <iostream>
#include <functional>
#include "8_2.h"

int times(int a, int b) {
    return a * b;
}

int main() {
    MPCS51044::function<int(int, int)> fn = times;
    std::cout << fn(10, 20);

    return 0;
}