#include "check.h"
#include "inventory.h"
#include "merge.h"

#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

bool sameProducts(const std::vector<Product>& left, const std::vector<Product>& right) {
    if (left.size() != right.size()) {
        return false;
    }
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (left[i].id != right[i].id || left[i].quantity != right[i].quantity) {
            return false;
        }
    }
    return true;
}

void checkMerged(const std::vector<Product>& input, const std::vector<Product>& expected) {
    const std::vector<Product> original = input;
    const std::vector<Product> got = mergeProducts(input);
    CHECK(sameProducts(input, original));
    CHECK(sameProducts(got, expected));
}

void emptyInput() {
    checkMerged({}, {});
}

void singleRow() {
    checkMerged({Product{"A", 4}}, {Product{"A", 4}});
}

void keepsOrderWithoutDuplicates() {
    checkMerged({Product{"B", 1}, Product{"A", 2}}, {Product{"B", 1}, Product{"A", 2}});
}

void mergesSample() {
    const std::vector<Product> input{
        Product{"A", 2}, Product{"B", 1}, Product{"A", 3}, Product{"C", 4}, Product{"B", 2}};
    checkMerged(input, {Product{"A", 5}, Product{"B", 3}, Product{"C", 4}});
}

void keepsFirstSeenOrder() {
    const std::vector<Product> input{
        Product{"C", 1}, Product{"A", 2}, Product{"C", 3}, Product{"B", 4}, Product{"A", 5}};
    checkMerged(input, {Product{"C", 4}, Product{"A", 7}, Product{"B", 4}});
}

void zeroIsAllowed() {
    checkMerged({Product{"A", 0}, Product{"B", 0}, Product{"A", 0}},
                {Product{"A", 0}, Product{"B", 0}});
    checkMerged({Product{"A", 0}, Product{"B", 2}, Product{"A", 5}},
                {Product{"A", 5}, Product{"B", 2}});
}

void rejectsNegative() {
    const std::vector<Product> input{Product{"A", 2}, Product{"B", -1}, Product{"A", 3}};
    const std::vector<Product> original = input;
    bool rejected = false;
    try {
        mergeProducts(input);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    CHECK(rejected);
    CHECK(sameProducts(input, original));
}

void rejectsNegativeOnLaterDuplicate() {
    bool rejected = false;
    try {
        mergeProducts({Product{"A", 1}, Product{"A", -1}});
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    CHECK(rejected);
}

void sumsToMax() {
    const int largest = std::numeric_limits<int>::max();
    checkMerged({Product{"A", largest}}, {Product{"A", largest}});
    checkMerged({Product{"A", largest}, Product{"A", 0}}, {Product{"A", largest}});
    checkMerged({Product{"A", largest - 1}, Product{"A", 1}}, {Product{"A", largest}});
}

void rejectsOverflow() {
    const int largest = std::numeric_limits<int>::max();
    bool rejected = false;
    try {
        mergeProducts({Product{"A", largest}, Product{"A", 1}});
    } catch (const std::overflow_error&) {
        rejected = true;
    }
    CHECK(rejected);

    rejected = false;
    try {
        mergeProducts({Product{"A", 1000000000}, Product{"A", 1000000000}, Product{"A", 1000000000}});
    } catch (const std::overflow_error&) {
        rejected = true;
    }
    CHECK(rejected);
}

void importMergesThenAdds() {
    Inventory inventory;
    CHECK(inventory.importProducts(
        {Product{"A", 2}, Product{"B", 1}, Product{"A", 3}, Product{"C", 4}}));

    const auto products = inventory.list();
    CHECK(products.size() == 3);
    CHECK(products[0].id == "A" && products[0].quantity == 5);
    CHECK(products[1].id == "B" && products[1].quantity == 1);
    CHECK(products[2].id == "C" && products[2].quantity == 4);
}

void failedImportLeavesInventory() {
    Inventory inventory;
    CHECK(inventory.add("Z", 3));

    bool rejected = false;
    try {
        inventory.importProducts({Product{"A", 1}, Product{"A", -2}});
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    CHECK(rejected);
    CHECK(!inventory.find("A"));
    const auto products = inventory.list();
    CHECK(products.size() == 1);
    CHECK(products[0].id == "Z" && products[0].quantity == 3);

    CHECK(!inventory.importProducts({Product{"Z", 1}, Product{"B", 4}}));
    CHECK(!inventory.find("B"));
    const auto after = inventory.list();
    CHECK(after.size() == 1);
    CHECK(after[0].id == "Z" && after[0].quantity == 3);
}

}  // namespace

#define RUN(fn)                                                         \
    try {                                                               \
        fn();                                                           \
    } catch (const std::exception& ex) {                                \
        std::cerr << #fn << " threw " << ex.what() << '\n';            \
        ++failureCount();                                               \
    }

int main() {
    RUN(emptyInput);
    RUN(singleRow);
    RUN(keepsOrderWithoutDuplicates);
    RUN(mergesSample);
    RUN(keepsFirstSeenOrder);
    RUN(zeroIsAllowed);
    RUN(rejectsNegative);
    RUN(rejectsNegativeOnLaterDuplicate);
    RUN(sumsToMax);
    RUN(rejectsOverflow);
    RUN(importMergesThenAdds);
    RUN(failedImportLeavesInventory);
    return testsDone();
}
