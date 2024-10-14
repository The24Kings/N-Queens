#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <cstdlib>

typedef std::vector<int> ChessBoard; // Index is the row, value is the column

int calculate_hueristic(ChessBoard board);
int minimum_remaining_values(ChessBoard board);
int randomNum(int min, int max);

const int BOARD_SIZE = 8;

int main() {
    ChessBoard board(BOARD_SIZE);
    srand(time(0));

    int MAX_MOVES = 1000;
    int MAX_RESTARTS = 100;

    // Initialize the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        board.at(i) = randomNum(0, BOARD_SIZE - 1);
    }

    // Print the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << "(" << i << "," << board.at(i) << ") ";
    }

    std::cout << std::endl;

    // Calculate the hueristic
    int h = calculate_hueristic(board);

    std::cout << "Initial hueristic: " << h << std::endl << std::endl;

    // Hill Climbing
    for (int i = 0; i < MAX_RESTARTS; i++) {
        ChessBoard temp_board = board;
        int temp_h = h;

        //std::cout << "Restart " << i << std::endl;

        for (int j = 0; j < MAX_MOVES; j++) {
            // Randomly move a queen
            int row = randomNum(0, BOARD_SIZE - 1);
            int col = randomNum(0, BOARD_SIZE - 1);

            temp_board.at(row) = col;

            // Calculate the hueristic
            int new_h = calculate_hueristic(temp_board);

            // Check if the new hueristic is better
            if (new_h < temp_h) {
                board = temp_board;
                h = new_h;
                std::cout << "Move " << j << " hueristic: " << h << std::endl;
                break;
            }
        }

        if (h == 0) {
            break;
        }
    }

    // Print the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << "(" << i << "," << board.at(i) << ") ";
    }

    std::cout << std::endl;
    std::cout << "Final hueristic: " << h << std::endl;

    // Write to file
    std::ofstream file("output.txt");

    if (!file.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return 1;
    }

    file << "Board Size: " << BOARD_SIZE << std::endl;
    file << "\n[";
    for (int i = 0; i < BOARD_SIZE; i++) {
        file << board.at(i) << ", ";
    }
    file << "]" << std::endl;

    file << std::endl;
    file << "Final hueristic: " << h << std::endl;

    file.close();

    return 0;
}

/**
 * @brief Calculates how many queens are attacking each other on columns and diagonals
 * 
 * @param board 
 * @return int 
 */
int calculate_hueristic(ChessBoard board) {
    int h = 0;

    for (int i = 0; i < board.size(); i++) {
        for (int j = i + 1; j < board.size(); j++) {
            if (board.at(i) == board.at(j) || abs(board.at(i) - board.at(j)) == abs(i - j)) { // Checks if the queens are in the same row or diagonal
                h++;
            }
        }
    }

    return h;
}

/**
 * @brief Calculates the minimum remaining values for the board
 * 
 * @param board 
 * @return int 
 */
int minimum_remaining_values(ChessBoard board) {
    int min = 0;
    int min_h = calculate_hueristic(board);
    ChessBoard temp_board = board;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board.size(); j++) {
            temp_board.at(i) = j;
            int h = calculate_hueristic(temp_board);

            if (h < min_h) {
                min = j;
                min_h = h;
            }
        }
    }

    return min;
}

/**
 * @brief Calculates a random number between min and max
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int randomNum(int min, int max) {
    return rand() % (max - min + 1) + min;
}
