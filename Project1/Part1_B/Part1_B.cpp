#include <iostream>
#include <vector>
#include <chrono>
#include <random>

// Binary Sunday pattern matching algorithm
std::vector<int> binarySunday(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    // Preprocessing: calculate the shift table
    std::vector<int> shift(256, m + 1);
    for (int i = 0; i < m; ++i)
        shift[static_cast<int>(pattern[i])] = m - i;

    int i = 0;
    while (i <= n - m) {
        int j;
        for (j = 0; j < m; ++j) {
            if (text[i + j] != pattern[j])
                break;
        }
        if (j == m)
            matches.push_back(i);

        // Shift the pattern to the next possible position
        if (i + m < n)
            i += shift[static_cast<int>(text[i + m])];
        else
            break;
    }

    return matches;
}

// Gusfield Z pattern matching algorithm
std::vector<int> gusfieldZ(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();
    std::string str = pattern + "$" + text;

    std::vector<int> Z(str.length(), 0);
    int l = 0, r = 0;
    for (int i = 1; i < str.length(); ++i) {
        if (i <= r)
            Z[i] = std::min(r - i + 1, Z[i - l]);
        while (i + Z[i] < str.length() && str[i + Z[i]] == str[Z[i]])
            ++Z[i];
        if (i + Z[i] - 1 > r) {
            l = i;
            r = i + Z[i] - 1;
        }
        if (Z[i] == m)
            matches.push_back(i - m - 1);
    }

    return matches;
}

// Knuth-Morris-Pratt (KMP) pattern matching algorithm
std::vector<int> kmp(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    // Preprocessing: calculate the failure function (longest proper suffix)
    std::vector<int> failure(m, 0);
    for (int i = 1, j = 0; i < m; ++i) {
        if (pattern[i] == pattern[j])
            failure[i] = ++j;
        else {
            if (j > 0) {
                j = failure[j - 1];
                --i;  // Stay at the same position in the pattern
            }
            else {
                failure[i] = 0;
            }
        }
    }

    int i = 0;
    while (i <= n - m) {
        int j;
        for (j = 0; j < m; ++j) {
            if (text[i + j] != pattern[j])
                break;
        }
        if (j == m)
            matches.push_back(i);

        // Shift the pattern based on the failure function
        if (j == 0)
            ++i;
        else
            i += j - failure[j - 1];
    }

    return matches;
}

// Rabin-Karp pattern matching algorithm
std::vector<int> rabinKarp(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();
    int d = 256; // Number of characters in the alphabet
    int q = 101; // A prime number

    int h = 1;
    for (int i = 0; i < m - 1; ++i)
        h = (h * d) % q;

    int p = 0; // Hash value for the pattern
    int t = 0; // Hash value for the current substring in the text

    // Calculate the initial hash values
    for (int i = 0; i < m; ++i) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (int i = 0; i <= n - m; ++i) {
        if (p == t) {
            int j;
            for (j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j])
                    break;
            }
            if (j == m)
                matches.push_back(i);
        }
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0)
                t += q;
        }
    }

    return matches;
}

double measureExecutionTime(std::vector<int>(*patternMatchingAlgorithm)(const std::string&, const std::string&), const std::string& text, const std::string& pattern) {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    std::vector<int> matches = patternMatchingAlgorithm(text, pattern);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    return duration.count();
}

int main() {
    std::string text = "Wacky Races";
    std::string pattern = "Races";

    double timeBinarySunday, timeGusfieldZ, timeKMP, timeRabinKarp;

    // Measure the execution time for Binary Sunday
    timeBinarySunday = measureExecutionTime(binarySunday, text, pattern);

    // Measure the execution time for Gusfield Z
    timeGusfieldZ = measureExecutionTime(gusfieldZ, text, pattern);

    // Measure the execution time for KMP
    timeKMP = measureExecutionTime(kmp, text, pattern);

    // Measure the execution time for Rabin-Karp
    timeRabinKarp = measureExecutionTime(rabinKarp, text, pattern);

    // Print the execution times
    std::cout << "Binary Sunday: " << timeBinarySunday << " seconds\n";
    std::cout << "Gusfield Z: " << timeGusfieldZ << " seconds\n";
    std::cout << "KMP: " << timeKMP << " seconds\n";
    std::cout << "Rabin-Karp: " << timeRabinKarp << " seconds\n";

    return 0;
}
