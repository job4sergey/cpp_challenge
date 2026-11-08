#ifndef INVENTORY_H
#define INVENTORY_H

#include "product.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// In-memory inventory. Ids are unique and stored quantities are at least 1.
class Inventory {
public:
    // Rejects an empty id, a quantity below 1, or an id that is already stored.
    bool add(const std::string& id, int quantity);

    // Adds delta to the stored quantity.
    // Rejects an unknown id, a negative result, or a result that does not fit in int.
    // A result of zero removes the product.
    bool adjust(const std::string& id, int delta);

    std::optional<Product> find(const std::string& id) const;
    std::vector<Product> list() const;

    // Merges lines with mergeProducts, then adds each result.
    // If any merged row cannot be added, this inventory is left unchanged.
    // Exceptions from mergeProducts propagate.
    bool importProducts(const std::vector<Product>& lines);

private:
    void removeAt(std::size_t pos);

    std::vector<Product> products_;
    std::unordered_map<std::string, std::size_t> index_;
};

#endif
