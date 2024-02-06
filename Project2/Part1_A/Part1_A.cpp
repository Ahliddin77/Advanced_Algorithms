#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <chrono>

// Function to load the dictionary from a file
std::unordered_set<std::string> loadDictionary(const std::string& dictionaryFile) {
    std::unordered_set<std::string> dictionary;
    std::ifstream file(dictionaryFile);
    std::string word;

    if (file.is_open()) {
        while (std::getline(file, word)) {
            dictionary.insert(word);
        }
        file.close();
    }

    return dictionary;
}

// Function to check if a word is spelled correctly using a linear list
bool isSpelledCorrectly_LinearList(const std::string& word, const std::unordered_set<std::string>& dictionary) {
    return dictionary.find(word) != dictionary.end();
}

// Function to check if a word is spelled correctly using a binary search tree (BBST)
bool isSpelledCorrectly_BBST(const std::string& word, const std::set<std::string>& dictionary) {
    return dictionary.find(word) != dictionary.end();
}

// Function to check if a word is spelled correctly using a trie
bool isSpelledCorrectly_Trie(const std::string& word, const std::unordered_set<std::string>& dictionary) {
    return dictionary.find(word) != dictionary.end();
}

// Function to check if a word is spelled correctly using a hash map
bool isSpelledCorrectly_HashMap(const std::string& word, const std::unordered_set<std::string>& dictionary) {
    return dictionary.find(word) != dictionary.end();
}

int main() {
    std::string dictionaryFile = "english_dictionary.txt";
    std::string textFile = "input_text.txt";

    // Load the dictionary
    std::unordered_set<std::string> dictionary = loadDictionary(dictionaryFile);
    std::set<std::string> dictionary_BBST(dictionary.begin(), dictionary.end());
    std::unordered_set<std::string> dictionary_Trie(dictionary.begin(), dictionary.end());
    std::unordered_set<std::string> dictionary_HashMap(dictionary.begin(), dictionary.end());

    // Spell check the text file using different approaches
    std::ifstream file(textFile);
    std::string word;
    std::chrono::steady_clock::time_point startTime, endTime;
    double elapsedTime;

    if (file.is_open()) {
        // Spell check using a linear list
        startTime = std::chrono::steady_clock::now();
        while (file >> word) {
            if (!isSpelledCorrectly_LinearList(word, dictionary)) {
                std::cout << "Misspelled word (Linear List): " << word << std::endl;
            }
        }
        endTime = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
        std::cout << "Linear List Spell Checking Time: " << elapsedTime << " seconds" << std::endl;

        // Rewind the file
        file.clear();
        file.seekg(0, std::ios::beg);

        // Spell check using a binary search tree (BBST)
        startTime = std::chrono::steady_clock::now();
        while (file >> word) {
            if (!isSpelledCorrectly_BBST(word, dictionary_BBST)) {
                std::cout << "Misspelled word (BBST): " << word << std::endl;
            }
        }
        endTime = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
        std::cout << "BBST Spell Checking Time: " << elapsedTime << " seconds" << std::endl;

        // Rewind the file
        file.clear();
        file.seekg(0, std::ios::beg);

        // Spell check using a trie
        startTime = std::chrono::steady_clock::now();
        while (file >> word) {
            if (!isSpelledCorrectly_Trie(word, dictionary_Trie)) {
                std::cout << "Misspelled word (Trie): " << word << std::endl;
            }
        }
        endTime = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
        std::cout << "Trie Spell Checking Time: " << elapsedTime << " seconds" << std::endl;

        // Rewind the file
        file.clear();
        file.seekg(0, std::ios::beg);

        // Spell check using a hash map
        startTime = std::chrono::steady_clock::now();
        while (file >> word) {
            if (!isSpelledCorrectly_HashMap(word, dictionary_HashMap)) {
                std::cout << "Misspelled word (Hash Map): " << word << std::endl;
            }
        }
        endTime = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
        std::cout << "Hash Map Spell Checking Time: " << elapsedTime << " seconds" << std::endl;

        file.close();
    }

    return 0;
}

