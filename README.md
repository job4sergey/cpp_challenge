# Inventory interview

An in-memory inventory. A product has an id and a quantity. Ids are unique. Add a product, change a quantity, look one up, or list products in insertion order. New quantities are at least 1. Zero removes a product; quantities never go negative. Unknown products cannot be updated. `import` merges lines that share an id, then adds them.

## Build and test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Pipe commands to `./build/inventory`: `add <id> <qty>`, `adjust <id> <delta>`, `find <id>`, `list`, `import <id> <qty> [<id> <qty>...]`.

## Exercises

### 1. Debug — 20 minutes

One logic error remains. Tests pass. Bringing a quantity to zero removes that product, but a later find can return a different product and a later adjust can change the wrong row. Pipe this to `./build/inventory`. Banana should end at 4 and cherry at 4. Fix the cause and run them again.

```text
add apple 2
add banana 3
add cherry 4
adjust apple -2
find banana
find cherry
adjust banana 1
find banana
find cherry
list
```

### 2. Review — 15 minutes

Review `exercises/review.cpp` as a pull request. The comment at the top is the intended behavior. Identify each problem, explain its impact, and suggest a fix. Say which issues should block merging. A rewrite is not required.

### 3. Implement — 25 minutes

Implement `mergeProducts` in `src/merge.cpp` (`include/merge.h`). `Inventory::importProducts` already calls it. Sum quantities that share an id, keep first-seen order, and do not change the input. Empty input returns an empty vector. Zero is allowed. Throw `std::invalid_argument` for a negative quantity and `std::overflow_error` if a sum does not fit in `int`. Use about 15 minutes to implement and 10 minutes to test:

```bash
cmake --build build --target merge_test
./build/merge_test
```
