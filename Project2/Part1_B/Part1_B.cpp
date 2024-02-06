#include <iostream>
#include <queue>
#include <vector>

// Structure to represent a position in the labyrinth
struct Position {
    int row;
    int col;

    Position(int r = 0, int c = 0) : row(r), col(c) {}
};

// Structure to represent a wizard
struct Wizard {
    Position position;
    int speed;

    Wizard(const Position& pos, int spd) : position(pos), speed(spd) {}
};

// Function to check if a position is within the labyrinth bounds
bool isValidPosition(const Position& pos, int rows, int cols) {
    return pos.row >= 0 && pos.row < rows&& pos.col >= 0 && pos.col < cols;
}

// Function to perform breadth-first search (BFS) to find the shortest path to the exit
int findShortestPath(const std::vector<std::vector<char>>& labyrinth, const Position& start) {
    int rows = labyrinth.size();
    int cols = labyrinth[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<Position, int>> q;

    q.push({ start, 0 });
    visited[start.row][start.col] = true;

    while (!q.empty()) {
        Position currPos = q.front().first;
        int distance = q.front().second;
        q.pop();

        // Check if the current position is the exit
        if (labyrinth[currPos.row][currPos.col] == 'E') {
            return distance;
        }

        // Explore the neighboring positions
        std::vector<Position> neighbors = {
            {currPos.row - 1, currPos.col},  // Up
            {currPos.row + 1, currPos.col},  // Down
            {currPos.row, currPos.col - 1},  // Left
            {currPos.row, currPos.col + 1}   // Right
        };

        for (const auto& neighbor : neighbors) {
            if (isValidPosition(neighbor, rows, cols) && labyrinth[neighbor.row][neighbor.col] != '#' &&
                !visited[neighbor.row][neighbor.col]) {
                q.push({ neighbor, distance + 1 });
                visited[neighbor.row][neighbor.col] = true;
            }
        }
    }

    // If the exit is not reachable, return a large value
    return rows * cols;
}

// Function to predict the winner of the Triwizard Tournament
int predictWinner(const std::vector<std::vector<char>>& labyrinth, const std::vector<Wizard>& wizards) {
    int winner = -1;
    int shortestTime = std::numeric_limits<int>::max();

    for (int i = 0; i < wizards.size(); ++i) {
        const Wizard& wizard = wizards[i];
        int time = findShortestPath(labyrinth, wizard.position);
        int totalTime = time / wizard.speed;  // Calculate total time based on wizard's speed

        if (totalTime < shortestTime) {
            shortestTime = totalTime;
            winner = i;
        }
    }

    return winner;
}

int main() {
    // Example labyrinth
    std::vector<std::vector<char>> labyrinth = {
        {'S', '.', '.', '.', '#', '.', '.'},
        {'.', '#', '#', '.', '#', '#', '.'},
        {'.', '.', '#', '.', '.', '.', '.'},
        {'.', '#', '#', '#', '#', '#', '.'},
        {'.', '.', '.', '.', '.', '#', 'E'}
    };

    // Example wizards and their speeds
    std::vector<Wizard> wizards = {
        {{0, 0}, 2},  // Wizard 1: Starting position (0, 0) with speed 2
        {{2, 2}, 3},  // Wizard 2: Starting position (2, 2) with speed 3
        {{3, 1}, 1}   // Wizard 3: Starting position (3, 1) with speed 1
    };

    int winner = predictWinner(labyrinth, wizards);

    if (winner != -1) {
        std::cout << "Wizard " << (winner + 1) << " wins the Triwizard Tournament!" << std::endl;
    }
    else {
        std::cout << "No winner. The exit is not reachable for any wizard." << std::endl;
    }

    return 0;
}
