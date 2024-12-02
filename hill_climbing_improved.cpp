#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <csignal>

typedef std::vector<int> ChessBoard; // Index is the row, value is the column

int calculate_hueristic(ChessBoard board);
int minimum_remaining_values(ChessBoard board);
int randomNum(int min, int max);
void signalHandler(int signum);

const int BOARD_SIZE = 8;
ChessBoard board(BOARD_SIZE);
int restarts = 0;
int h = 0;
int mrv = 0;

int main() {
    struct sigaction sigact;
    srand(time(0));

    int MAX_MOVES = 1000;

    // Signal handler
    sigact.sa_handler = signalHandler;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;

    sigaction(SIGINT, &sigact, NULL);

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
    h = calculate_hueristic(board);
    mrv = minimum_remaining_values(board);

    std::cout << "Initial hueristic: " << h << std::endl << std::endl;
    std::cout << "Initial MRV: " << mrv << std::endl;

    // Hill Climbing
    while(h != 0 && mrv != BOARD_SIZE-1) {
        ChessBoard temp_board = board;
        int temp_h = h;
        int temp_mrv = mrv;

        //std::cout << "Restart " << restarts << std::endl;

        for (int j = 0; j < MAX_MOVES; j++) {
            // Randomly move a queen
            int row = randomNum(0, BOARD_SIZE - 1);
            int col = randomNum(0, BOARD_SIZE - 1);

            temp_board.at(row) = col;

            // Calculate the hueristics
            int new_h = calculate_hueristic(temp_board);
            int new_mrv = minimum_remaining_values(temp_board);

            // Check if the new hueristic is better
            if (new_h < temp_h || (new_h == temp_h && (temp_mrv > 0 && new_mrv < temp_mrv))) {
                board = temp_board;
                h = new_h;
                mrv = new_mrv;
                std::cout << "Move " << j << " hueristic: " << h << std::endl;
                std::cout << "Move " << j << " MRV: " << mrv << std::endl;
                break;
            }
        }

        restarts++;
    }

    std::cout << "Restarts: " << restarts << std::endl;

    // Print the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << "(" << i << "," << board.at(i) << ") ";
    }

    std::cout << std::endl;

    // Write to file
    std::ofstream file("improved_output.txt");

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
 * @brief Calculates the number of open spaces in the rows
 * 
 * @param board 
 * @return int 
 */
int minimum_remaining_values(ChessBoard board) {
    int mrv = BOARD_SIZE;
    
    // Loop over each column and check how many queens are attacking it
    for (int i = 0; i < board.size(); i++) {
        int count = mrv;

        for (int j = 0; j < board.size(); j++) {
            if (board.at(i) == board.at(j) || abs(board.at(i) - board.at(j)) == abs(i - j)) {
                count--;
            }
        }

        if (count < mrv) {
            mrv = count;
        }
    }

    return mrv;
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

/**
 * @brief Catches the interrupt signal and prints the board
 * 
 * @param signum 
 */
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
    std::cout << "Restarts: " << restarts << std::endl;

    // Print the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << "(" << i << "," << board.at(i) << ") ";
    }
    std::cout << std::endl;

    // Print Hueristic
    std::cout << "Hueristic: " << h << std::endl;
    std::cout << "MRV: " << mrv << std::endl;

    exit(signum);
}