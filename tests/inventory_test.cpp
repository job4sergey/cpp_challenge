#include "check.h"
#include "inventory.h"

#include <limits>
#include <string>

namespace {

void requireProduct(const Inventory& inventory, const std::string& id, int quantity) {
    const auto product = inventory.find(id);
    CHECK(product.has_value());
    CHECK(product && product->id == id);
    CHECK(product && product->quantity == quantity);
}

void emptyList() {
    const Inventory inventory;
    CHECK(inventory.list().empty());
    CHECK(!inventory.find("missing"));
}

void addAndFind() {
    Inventory inventory;
    CHECK(inventory.add("apple", 5));
    requireProduct(inventory, "apple", 5);
}

void listKeepsInsertionOrder() {
    Inventory inventory;
    CHECK(inventory.add("pear", 1));
    CHECK(inventory.add("apple", 2));
    CHECK(inventory.add("plum", 3));

    const auto products = inventory.list();
    CHECK(products.size() == 3);
    CHECK(products[0].id == "pear" && products[0].quantity == 1);
    CHECK(products[1].id == "apple" && products[1].quantity == 2);
    CHECK(products[2].id == "plum" && products[2].quantity == 3);
}

void listAndFindReturnCopies() {
    Inventory inventory;
    CHECK(inventory.add("apple", 2));

    auto listed = inventory.list();
    listed[0].quantity = 9;
    auto found = inventory.find("apple");
    CHECK(found.has_value());
    found->quantity = 8;

    requireProduct(inventory, "apple", 2);
}

void rejectBadAdds() {
    Inventory inventory;
    CHECK(!inventory.add("", 1));
    CHECK(!inventory.add("apple", 0));
    CHECK(!inventory.add("apple", -3));
    CHECK(inventory.list().empty());

    CHECK(inventory.add("apple", 2));
    CHECK(!inventory.add("apple", 4));
    requireProduct(inventory, "apple", 2);
}

void adjustUpAndDown() {
    Inventory inventory;
    CHECK(inventory.add("apple", 5));
    CHECK(inventory.add("pear", 4));
    CHECK(inventory.adjust("apple", 2));
    CHECK(inventory.adjust("apple", -3));
    requireProduct(inventory, "apple", 4);
    requireProduct(inventory, "pear", 4);

    const auto products = inventory.list();
    CHECK(products.size() == 2);
    CHECK(products[0].id == "apple" && products[0].quantity == 4);
    CHECK(products[1].id == "pear" && products[1].quantity == 4);
}

void rejectBadAdjustments() {
    Inventory inventory;
    CHECK(!inventory.adjust("missing", 1));
    CHECK(inventory.add("apple", 3));
    CHECK(!inventory.adjust("apple", -4));
    requireProduct(inventory, "apple", 3);
    CHECK(inventory.list().size() == 1);
}

void rejectAdjustOverflow() {
    Inventory inventory;
    const int largest = std::numeric_limits<int>::max();
    CHECK(inventory.add("apple", largest));
    CHECK(!inventory.adjust("apple", 1));
    requireProduct(inventory, "apple", largest);
    CHECK(inventory.adjust("apple", -1));
    requireProduct(inventory, "apple", largest - 1);
}

void removeOnlyProduct() {
    Inventory inventory;
    CHECK(inventory.add("apple", 2));
    CHECK(inventory.adjust("apple", -2));
    CHECK(!inventory.find("apple"));
    CHECK(inventory.list().empty());
}

void removeLastOfTwo() {
    Inventory inventory;
    CHECK(inventory.add("apple", 2));
    CHECK(inventory.add("pear", 5));
    CHECK(inventory.adjust("pear", -5));

    requireProduct(inventory, "apple", 2);
    CHECK(!inventory.find("pear"));
    const auto products = inventory.list();
    CHECK(products.size() == 1);
    CHECK(products[0].id == "apple" && products[0].quantity == 2);
}

void addAfterRemovingLast() {
    Inventory inventory;
    CHECK(inventory.add("apple", 1));
    CHECK(inventory.adjust("apple", -1));
    CHECK(inventory.add("pear", 6));
    CHECK(inventory.add("plum", 7));
    CHECK(inventory.adjust("plum", -7));

    requireProduct(inventory, "pear", 6);
    CHECK(!inventory.find("plum"));
    CHECK(!inventory.find("apple"));
    const auto products = inventory.list();
    CHECK(products.size() == 1);
    CHECK(products[0].id == "pear");
}

}  // namespace

int main() {
    emptyList();
    addAndFind();
    listKeepsInsertionOrder();
    listAndFindReturnCopies();
    rejectBadAdds();
    adjustUpAndDown();
    rejectBadAdjustments();
    rejectAdjustOverflow();
    removeOnlyProduct();
    removeLastOfTwo();
    addAfterRemovingLast();
    return testsDone();
}
