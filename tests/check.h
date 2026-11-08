#ifndef CHECK_H
#define CHECK_H

#include <iostream>

inline int& failureCount() {
    static int count = 0;
    return count;
}

#define CHECK(condition)                                          \
    do {                                                          \
        if (!(condition)) {                                       \
            std::cerr << __FILE__ << ":" << __LINE__ << ": "     \
                      << #condition << '\n';                      \
            ++failureCount();                                     \
        }                                                         \
    } while (0)

inline int testsDone() {
    if (failureCount() != 0) {
        std::cerr << failureCount() << " check(s) failed\n";
        return 1;
    }
    std::cout << "all checks passed\n";
    return 0;
}

#endif
