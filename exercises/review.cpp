#include "product.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Receive a delivery into warehouse stock.
//
// Each line adds its quantity to the stock for that id.
// Repeated ids in one delivery accumulate.
// Reject a line when its id is empty, its quantity is negative,
// or the addition would overflow int. Append each rejected id to
// errors and continue with the later lines. Leave stock unchanged
// for a rejected line. Return how many lines were applied.

namespace {

bool isRejected(const Product& line, std::vector<std::string>& errors) {
    if (line.id.empty() || line.quantity < 0) {
        errors.push_back(line.id);
        return true;
    }
    return false;
}

void writeSummary(
    const std::vector<std::unordered_map<std::string, int>::iterator>& touched) {
    for (const auto entry : touched) {
        std::cout << entry->first << '=' << entry->second << '\n';
    }
}

}  // namespace

int receiveDelivery(std::unordered_map<std::string, int>& stock,
                    const std::vector<Product>& delivery,
                    std::vector<std::string>& errors) {
    int applied = 0;
    std::vector<std::unordered_map<std::string, int>::iterator> touched;

    for (const Product& line : delivery) {
        int& current = stock[line.id];
        if (isRejected(line, errors)) {
            return applied;
        }

        current += line.quantity;
        touched.push_back(stock.find(line.id));
        ++applied;
    }

    writeSummary(touched);
    return applied;
}
