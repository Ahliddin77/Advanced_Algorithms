#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

const int PRIME = 31;

bool compareCorner(const vector<vector<int>>& picture, int x1, int y1, int x2, int y2, int K) {
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            if (picture[x1 + i][y1 + j] != picture[x2 + i][y2 + j])
                return false;
        }
    }
    return true;
}

bool checkDuplicateCorner(const vector<vector<int>>& picture, int M, int N, int K) {
    if (M <= K || N <= K)
        return false;

    // Calculate the hash of the top-right corner
    int cornerHash = 0;
    int power = 1;
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            cornerHash += picture[i][N - K + j] * power;
            power *= PRIME;
        }
    }

    // Store the hash of each K by K corner in a hash map
    unordered_map<int, vector<pair<int, int>>> cornerHashes;

    // Initialize the first hash value for the first row
    int firstRowHash = cornerHash;
    cornerHashes[firstRowHash].push_back(make_pair(0, N - K));

    // Calculate the rolling hash for each subsequent row
    for (int i = 1; i <= M - K; ++i) {
        // Remove the contribution of the previous row
        firstRowHash -= picture[i - 1][N - K] * power;

        // Shift the hash to the left and add the new row's contribution
        firstRowHash *= PRIME;
        firstRowHash += picture[i + K - 1][N - K];

        // Store the corner hash with its position
        cornerHashes[firstRowHash].push_back(make_pair(i, N - K));
    }

    // Check for matches of the top-right corner in the picture
    for (int i = K; i <= M - K; ++i) {
        int cornerHash = 0;
        int power = 1;
        for (int j = 0; j < K; ++j) {
            cornerHash += picture[i][N - K + j] * power;
            power *= PRIME;
        }

        // Check if the corner hash matches any stored hash
        if (cornerHashes.count(cornerHash) > 0) {
            const vector<pair<int, int>>& matchingCorners = cornerHashes[cornerHash];
            for (const auto& corner : matchingCorners) {
                if (compareCorner(picture, corner.first, corner.second, i, N - K, K))
                    return true;
            }
        }
    }

    return false;
}

int main() {
    vector<vector<int>> picture = {
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 1, 2, 3},
        {4, 5, 6, 7, 8, 9},
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 1, 2, 3},
        {4, 5, 6, 7, 8, 9}
    };

    int M = picture.size();
    int N = picture[0].size();
    int K = 3;

    bool duplicateCorner = checkDuplicateCorner(picture, M, N, K);

    std::cout << "Duplicate corner: " << (duplicateCorner ? "Yes" : "No") << std::endl;

    return 0;
}
