#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <random>


// Brute-force pattern matching algorithm
std::vector<int> bruteForce(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; ++i) {
        int j;
        for (j = 0; j < m; ++j) {
            if (text[i + j] != pattern[j])
                break;
        }
        if (j == m)
            matches.push_back(i);
    }

    return matches;
}

// Sunday pattern matching algorithm
std::vector<int> sunday(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    // Preprocessing: calculate the shift table
    std::vector<int> shift(256, m + 1);
    for (int i = 0; i < m; ++i)
        shift[pattern[i]] = m - i;

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
            i += shift[text[i + m]];
        else
            break;
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
    int prime = 101; // Prime number for hash calculation

    // Calculate the hash value for pattern and first window of text
    int patternHash = 0;
    int textHash = 0;
    int power = 1;
    for (int i = 0; i < m; ++i) {
        patternHash = (patternHash + pattern[i]) % prime;
        textHash = (textHash + text[i]) % prime;
        if (i != m - 1)
            power = (power * 2) % prime;
    }

    for (int i = 0; i <= n - m; ++i) {
        // Check if the hash values match
        if (patternHash == textHash) {
            int j;
            for (j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j])
                    break;
            }
            if (j == m)
                matches.push_back(i);
        }

        // Calculate the hash value for the next window
        if (i < n - m) {
            textHash = (2 * (textHash - text[i] * power) + text[i + m]) % prime;
            if (textHash < 0)
                textHash += prime;
        }
    }

    return matches;
}

// Gusfield Z algorithm for pattern matching
std::vector<int> gusfieldZ(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    std::string str = pattern + "$" + text;
    int n = str.length();
    int m = pattern.length();
    std::vector<int> z(n, 0);
    int left = 0, right = 0;
    for (int k = 1; k < n; ++k) {
        if (k > right) {
            int j;
            for (j = 0; j < n - k && str[j] == str[j + k]; ++j);
            z[k] = j;
            if (j > 0) {
                left = k;
                right = k + j - 1;
            }
        }
        else {
            int p = k - left;
            int remaining = right - k + 1;
            if (z[p] < remaining) {
                z[k] = z[p];
            }
            else {
                int j;
                for (j = 1; j < remaining && str[right + j] == str[right + j + p]; ++j);
                z[k] = remaining + j - 1;
                left = k;
                right = right + z[k] - 1;
            }
        }

        // Check if a match is found
        if (z[k] == m)
            matches.push_back(k - m - 1);
    }

    return matches;
}
// Finite State Machine (FSM) pattern matching algorithm
std::vector<int> fsm(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    // Build the transition table
    std::vector<std::vector<int>> transition(m + 1, std::vector<int>(256, 0));
    int state = 0;
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j < 256; ++j) {
            if (i < m && j == pattern[i])
                transition[i][j] = i + 1;
            else
                transition[i][j] = transition[state][j];
        }
        if (i < m)
            state = transition[state][pattern[i]];
    }

    // Perform pattern matching
    int currentState = 0;
    for (int i = 0; i < n; ++i) {
        currentState = transition[currentState][text[i]];
        if (currentState == m)
            matches.push_back(i - m + 1);
    }

    return matches;
}
// Measure the execution time of a function
template<typename Func, typename... Args>
std::pair<double, std::vector<int>> measureExecutionTime(Func&& func, Args&&... args) {
    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<int> matches = std::forward<Func>(func)(std::forward<Args>(args)...);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    return { duration.count(), matches };
}


// Generate random pattern of given length
std::string generatePattern(int length) {
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<int> distribution(0, sizeof(alphanum) - 2);
    std::string pattern(length, '\0');
    for (int i = 0; i < length; ++i)
        pattern[i] = alphanum[distribution(gen)];
    return pattern;
}

int main() {
    std::string text = "Mom bought me a new computer";
    std::string smallPattern = "new";
    std::string largePattern = "Mom bought me a new computer. It's a great machine that allows me to do many things.";

    std::cout << "Text Length, Brute-Force, Sunday, KMP, Rabin-Karp, Gusfield Z, FSM" << std::endl;

    for (int i = 1000; i <= 10000; i += 1000) {
        std::string randomText(i, 'a'); // Generate random text of given length

        auto bfResult = measureExecutionTime(bruteForce, randomText, smallPattern);
        auto sunResult = measureExecutionTime(sunday, randomText, smallPattern);
        auto kmpResult = measureExecutionTime(kmp, randomText, smallPattern);
        auto rkResult = measureExecutionTime(rabinKarp, randomText, smallPattern);
        auto gzResult = measureExecutionTime(gusfieldZ, randomText, smallPattern);
        auto fsmResult = measureExecutionTime(fsm, randomText, smallPattern);

        std::cout << i << ", " << bfResult.first << ", " << sunResult.first << ", " << kmpResult.first << ", "
            << rkResult.first << ", " << gzResult.first << ", " << fsmResult.first << std::endl;
    }


    return 0;
}