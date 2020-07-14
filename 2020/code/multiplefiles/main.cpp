#include <iostream>
#include "./lib/math.h"
#include "./definition/error_code.h"

int main() {
    int a{0}, b{0}, c{0};
    std::cin >> a >> b >> c;
    int sum(0);
    int ret_val = safeAdd(sum, a, b, c);
    std::cout << sum << std::endl;

    return ret_val;
}