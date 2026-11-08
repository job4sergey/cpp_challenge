#include "inventory.h"

#include <exception>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace {

void error(const std::string& message) {
    std::cout << "error: " << message << '\n';
}

void printProduct(const Product& product) {
    std::cout << product.id << ' ' << product.quantity << '\n';
}

std::optional<int> parseInt(const std::string& text) {
    if (text.empty()) {
        return std::nullopt;
    }
    try {
        std::size_t consumed = 0;
        const int value = std::stoi(text, &consumed);
        if (consumed != text.size()) {
            return std::nullopt;
        }
        return value;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::vector<std::string> tokens(const std::string& line) {
    std::istringstream input(line);
    std::vector<std::string> words;
    std::string word;
    while (input >> word) {
        words.push_back(word);
    }
    return words;
}

void printHelp() {
    std::cout << "add <id> <qty>\n"
              << "adjust <id> <delta>\n"
              << "find <id>\n"
              << "list\n"
              << "import <id> <qty> [<id> <qty>...]\n";
}

void addProduct(Inventory& inventory, const std::vector<std::string>& words) {
    if (words.size() != 3) {
        error("usage: add <id> <qty>");
        return;
    }
    const auto quantity = parseInt(words[2]);
    if (!quantity) {
        error("invalid number");
        return;
    }
    if (!inventory.add(words[1], *quantity)) {
        error("could not add");
        return;
    }
    std::cout << "ok\n";
}

void adjustProduct(Inventory& inventory, const std::vector<std::string>& words) {
    if (words.size() != 3) {
        error("usage: adjust <id> <delta>");
        return;
    }
    const auto delta = parseInt(words[2]);
    if (!delta) {
        error("invalid number");
        return;
    }
    if (!inventory.adjust(words[1], *delta)) {
        error("could not adjust");
        return;
    }
    std::cout << "ok\n";
}

void findProduct(const Inventory& inventory, const std::vector<std::string>& words) {
    if (words.size() != 2) {
        error("usage: find <id>");
        return;
    }
    const auto product = inventory.find(words[1]);
    if (!product) {
        std::cout << "missing " << words[1] << '\n';
        return;
    }
    printProduct(*product);
}

void listProducts(const Inventory& inventory, const std::vector<std::string>& words) {
    if (words.size() != 1) {
        error("usage: list");
        return;
    }
    const std::vector<Product> products = inventory.list();
    if (products.empty()) {
        std::cout << "empty\n";
        return;
    }
    for (const Product& product : products) {
        printProduct(product);
    }
}

void importProducts(Inventory& inventory, const std::vector<std::string>& words) {
    if (words.size() < 3 || words.size() % 2 == 0) {
        error("usage: import <id> <qty> [<id> <qty>...]");
        return;
    }

    std::vector<Product> lines;
    for (std::size_t i = 1; i < words.size(); i += 2) {
        const auto quantity = parseInt(words[i + 1]);
        if (!quantity) {
            error("invalid number");
            return;
        }
        lines.push_back(Product{words[i], *quantity});
    }

    try {
        if (!inventory.importProducts(lines)) {
            error("could not import");
            return;
        }
    } catch (const std::exception& ex) {
        error(ex.what());
        return;
    }
    std::cout << "ok\n";
}

void handle(Inventory& inventory, const std::vector<std::string>& words) {
    if (words.empty()) {
        return;
    }
    const std::string& command = words[0];
    if (command == "help") {
        printHelp();
    } else if (command == "add") {
        addProduct(inventory, words);
    } else if (command == "adjust") {
        adjustProduct(inventory, words);
    } else if (command == "find") {
        findProduct(inventory, words);
    } else if (command == "list") {
        listProducts(inventory, words);
    } else if (command == "import") {
        importProducts(inventory, words);
    } else {
        error("unknown command");
    }
}

}  // namespace

int main() {
    Inventory inventory;
    std::string line;
    while (std::getline(std::cin, line)) {
        handle(inventory, tokens(line));
    }
    return 0;
}
