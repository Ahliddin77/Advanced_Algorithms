#include <iostream>
#include <string>

bool bruteForceWildcard(const std::string& text, const std::string& pattern) {
    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; ++i) {
        int j;
        for (j = 0; j < m; ++j) {
            if (pattern[j] != '?' && pattern[j] != text[i + j])
                break;
        }
        if (j == m)
            return true;
    }

    return false;
}

bool sundayWildcard(const std::string& text, const std::string& pattern) {
    int n = text.length();
    int m = pattern.length();

    int i = 0;
    while (i <= n - m) {
        int j;
        for (j = 0; j < m; ++j) {
            if (pattern[j] != '?' && pattern[j] != text[i + j])
                break;
        }
        if (j == m)
            return true;

        // Shift the pattern to the next possible position
        if (i + m < n) {
            char nextChar = text[i + m];
            if (nextChar == '?' || pattern.find(nextChar) == std::string::npos)
                i += m + 1;
            else
                i += m - pattern.rfind(nextChar, m - 1);
        }
        else {
            break;
        }
    }

    return false;
}

int main() {
    std::string text = "The quick brown fox jumps over the lazy dog.";
    std::string pattern1 = "quick*brown";
    std::string pattern2 = "j?mps";
    std::string pattern3 = "l?zy*dog";
    std::string pattern4 = "\\*fox";

    // Brute-force wildcard matching
    bool match1 = bruteForceWildcard(text, pattern1);
    bool match2 = bruteForceWildcard(text, pattern2);
    bool match3 = bruteForceWildcard(text, pattern3);
    bool match4 = bruteForceWildcard(text, pattern4);

    std::cout << "Brute-Force Wildcard Matching Results:\n";
    std::cout << "Pattern 1: " << (match1 ? "Match" : "No Match") << std::endl;
    std::cout << "Pattern 2: " << (match2 ? "Match" : "No Match") << std::endl;
    std::cout << "Pattern 3: " << (match3 ? "Match" : "No Match") << std::endl;
    std::cout << "Pattern 4: " << (match4 ? "Match" : "No Match") << std::endl;

    // Sunday wildcard matching
    bool match5 = sundayWildcard(text, pattern1);
    bool match6 = sundayWildcard(text, pattern2);
    bool match7 = sundayWildcard(text, pattern3);
    bool match8 = sundayWildcard(text, pattern4);

    std::cout << "Sunday Wildcard Matching Results:\n";
    std::cout << "Pattern 1: " << (match5 ? "Match" : "No Match") << std::endl;
    std::cout << "Pattern 2: " << (match6 ? "Match" : "No Match") << std::endl;
    std::cout << "Pattern 3: " << (match7 ? "Match" : "No Match") << std::endl;
    std::cout << "Pattern 4: " << (match8 ? "Match" : "No Match") << std::endl;

    return 0;
}
