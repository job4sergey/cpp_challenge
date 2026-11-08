#ifndef MERGE_H
#define MERGE_H

#include "product.h"

#include <vector>

// Merge rows that share an id by summing their quantities.
// The result keeps the order in which each id first appears.
// An empty input returns an empty vector. The input is not modified.
// A quantity of zero is allowed.
// Throws std::invalid_argument if any quantity is negative.
// Throws std::overflow_error if any sum does not fit in int.
std::vector<Product> mergeProducts(const std::vector<Product>& products);

#endif
