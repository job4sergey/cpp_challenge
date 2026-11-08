#include "inventory.h"

#include "merge.h"

#include <limits>
#include <utility>

bool Inventory::add(const std::string& id, int quantity) {
    if (id.empty() || quantity <= 0) {
        return false;
    }
    if (index_.find(id) != index_.end()) {
        return false;
    }

    products_.push_back(Product{id, quantity});
    index_.emplace(id, products_.size() - 1);
    return true;
}

bool Inventory::adjust(const std::string& id, int delta) {
    const auto it = index_.find(id);
    if (it == index_.end()) {
        return false;
    }

    const std::size_t pos = it->second;
    if (pos >= products_.size()) {
        return false;
    }

    const long long updated =
        static_cast<long long>(products_[pos].quantity) + delta;
    if (updated < 0 || updated > std::numeric_limits<int>::max()) {
        return false;
    }
    if (updated == 0) {
        removeAt(pos);
        return true;
    }

    products_[pos].quantity = static_cast<int>(updated);
    return true;
}

std::optional<Product> Inventory::find(const std::string& id) const {
    const auto it = index_.find(id);
    if (it == index_.end()) {
        return std::nullopt;
    }
    if (it->second >= products_.size()) {
        return std::nullopt;
    }
    return products_[it->second];
}

std::vector<Product> Inventory::list() const {
    return products_;
}

bool Inventory::importProducts(const std::vector<Product>& lines) {
    const std::vector<Product> merged = mergeProducts(lines);
    Inventory updated = *this;
    for (const Product& product : merged) {
        if (!updated.add(product.id, product.quantity)) {
            return false;
        }
    }
    *this = std::move(updated);
    return true;
}

void Inventory::removeAt(std::size_t pos) {
    const std::string id = products_[pos].id;
    products_.erase(products_.begin() + static_cast<std::ptrdiff_t>(pos));
    index_.erase(id);
}
