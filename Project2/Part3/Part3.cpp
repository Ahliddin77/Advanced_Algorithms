#include <iostream>
#include <vector>
#include <chrono>
#include <string>
template <typename Key, typename Value>
class SeparateChainingHashTable {
private:
    std::vector<std::vector<std::pair<Key, Value>>> table;
    size_t size;

public:
    SeparateChainingHashTable(size_t capacity) : table(capacity), size(0) {}

    size_t hash(const Key& key) const {
        return std::hash<Key>()(key) % table.size();
    }

    void insert(const Key& key, const Value& value) {
        size_t index = hash(key);
        table[index].push_back(std::make_pair(key, value));
        size++;
    }

    bool search(const Key& key, Value& value) const {
        size_t index = hash(key);
        for (const auto& entry : table[index]) {
            if (entry.first == key) {
                value = entry.second;
                return true;
            }
        }
        return false;
    }

    float loadFactor() const {
        return static_cast<float>(size) / table.size();
    }
};

template <typename Key, typename Value>
class LinearProbingHashTable {
private:
    std::vector<std::pair<Key, Value>> table;
    size_t size;

public:
    LinearProbingHashTable(size_t capacity) : table(capacity), size(0) {}

    size_t hash(const Key& key, size_t i) const {
        return (std::hash<Key>()(key) + i) % table.size();
    }

    void insert(const Key& key, const Value& value) {
        size_t i = 0;
        size_t index = hash(key, i);
        while (table[index].first != Key{}) {
            i++;
            index = hash(key, i);
        }
        table[index] = std::make_pair(key, value);
        size++;
    }

    bool search(const Key& key, Value& value) const {
        size_t i = 0;
        size_t index = hash(key, i);
        while (table[index].first != Key{}) {
            if (table[index].first == key) {
                value = table[index].second;
                return true;
            }
            i++;
            index = hash(key, i);
        }
        return false;
    }

    float loadFactor() const {
        return static_cast<float>(size) / table.size();
    }
};

template <typename Key, typename Value>
class DoubleHashingHashTable {
private:
    std::vector<std::pair<Key, Value>> table;
    size_t size;

public:
    DoubleHashingHashTable(size_t capacity) : table(capacity), size(0) {}

    size_t hash1(const Key& key) const {
        return std::hash<Key>()(key) % table.size();
    }

    size_t hash2(const Key& key) const {
        return 1 + (std::hash<Key>()(key) % (table.size() - 1));
    }

    size_t hash(const Key& key, size_t i) const {
        return (hash1(key) + i * hash2(key)) % table.size();
    }

    void insert(const Key& key, const Value& value) {
        size_t i = 0;
        size_t index = hash(key, i);
        while (table[index].first != Key{}) {
            i++;
            index = hash(key, i);
        }
        table[index] = std::make_pair(key, value);
        size++;
    }

    bool search(const Key& key, Value& value) const {
        size_t i = 0;
        size_t index = hash(key, i);
        while (table[index].first != Key{}) {
            if (table[index].first == key) {
                value = table[index].second;
                return true;
            }
            i++;
            index = hash(key, i);
        }
        return false;
    }

    float loadFactor() const {
        return static_cast<float>(size) / table.size();
    }
};

int main() {
    const int numElements = 100000;
    const int capacity = 100003;

    SeparateChainingHashTable<int, std::string> separateChainingTable(capacity);
    LinearProbingHashTable<int, std::string> linearProbingTable(capacity);
    DoubleHashingHashTable<int, std::string> doubleHashingTable(capacity);

    auto startTime = std::chrono::steady_clock::now();
    for (int j = 0; j < numElements; j++) {
        separateChainingTable.insert(j, std::to_string(j));
    }
    auto endTime = std::chrono::steady_clock::now();
    auto diffTime = endTime - startTime;
    std::cout << "Separate Chaining: " << std::chrono::duration<double, std::milli>(diffTime).count() << " ms" << std::endl;

    startTime = std::chrono::steady_clock::now();
    for (int j = 0; j < numElements; j++) {
        linearProbingTable.insert(j, std::to_string(j));
    }
    endTime = std::chrono::steady_clock::now();
    diffTime = endTime - startTime;
    std::cout << "Linear Probing: " << std::chrono::duration<double, std::milli>(diffTime).count() << " ms" << std::endl;

    startTime = std::chrono::steady_clock::now();
    for (int j = 0; j < numElements; j++) {
        doubleHashingTable.insert(j, std::to_string(j));
    }
    endTime = std::chrono::steady_clock::now();
    diffTime = endTime - startTime;
    std::cout << "Double Hashing: " << std::chrono::duration<double, std::milli>(diffTime).count() << " ms" << std::endl;

    return 0;
}
