#include <iostream>
#include <vector>
#include <unordered_map>

// Function to check if a guest can be seated at a table without conflicts
bool isValidSeat(int guest, int table, const std::vector<std::vector<int>>& dislikes, const std::vector<int>& seating) {
    for (int i = 0; i < seating.size(); ++i) {
        if (seating[i] == guest && dislikes[i][table] == 1) {
            return false;
        }
    }
    return true;
}

// Recursive DFS function to find a valid sitting arrangement
bool dfs(const std::vector<std::vector<int>>& dislikes, std::vector<int>& seating, int table) {
    if (table == seating.size()) {
        return true;  // Base case: all tables have been filled
    }

    for (int guest = 0; guest < dislikes.size(); ++guest) {
        if (isValidSeat(guest, table, dislikes, seating)) {
            seating[table] = guest;

            if (dfs(dislikes, seating, table + 1)) {
                return true;  // Found a valid arrangement
            }

            seating[table] = -1;  // Backtrack
        }
    }

    return false;  // No valid arrangement found
}

// Function to set up a sitting scheme for Aunt's Namesday party
std::vector<int> setSittingScheme(const std::vector<std::vector<int>>& dislikes, int numTables) {
    std::vector<int> seating(numTables, -1);  // Initialize seating arrangement

    if (dfs(dislikes, seating, 0)) {
        return seating;  // Return the valid seating arrangement
    }
    else {
        return {};  // Return an empty vector if no valid arrangement is possible
    }
}

int main() {
    // Example input: invited guests and their dislikes
    std::vector<std::vector<int>> dislikes = {
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 1},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 1},
        {0, 1, 0, 1, 0}
    };

    int numTables = 2;  // Number of tables

    std::vector<int> seating = setSittingScheme(dislikes, numTables);

    if (!seating.empty()) {
        // Display the seating arrangement
        for (int table = 0; table < seating.size(); ++table) {
            std::cout << "Table " << table + 1 << ": Guest " << seating[table] + 1 << std::endl;
        }
    }
    else {
        std::cout << "No valid sitting arrangement possible." << std::endl;
    }

    return 0;
}
