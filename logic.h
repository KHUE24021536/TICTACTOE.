#ifndef LOGIC_H
#define LOGIC_H

#include "defs.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Cấu trúc đại diện cho trò chơi Tic Tac Toe
struct Tictactoe {
    char board[BOARD_SIZE][BOARD_SIZE]; // Bảng trò chơi
    char nextMove;                      // Người chơi tiếp theo ('X' hoặc 'O')
    char winner;                        // Người thắng ('X', 'O' hoặc 'D' cho hòa)
    bool gameEnded;                     // Cờ kiểm tra trò chơi đã kết thúc chưa
    GameMode mode;                      // Chế độ chơi: PvP, AI dễ, AI khó

    // Khởi tạo trò chơi mới
    void init(GameMode gameMode) {
        mode = gameMode;
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                board[i][j] = ' ';
        nextMove = 'X';
        winner = ' ';
        gameEnded = false;
    }

    // Thực hiện nước đi của người chơi tại (row, col)
    void move(int row, int col) {
        if (gameEnded || board[row][col] != ' ') return; // Nếu đã kết thúc hoặc ô đã đi, bỏ qua

        board[row][col] = nextMove;

        if (checkWin(row, col)) { // Nếu thắng
            winner = nextMove;
            gameEnded = true;
        } else if (checkDraw()) { // Nếu hòa
            winner = 'D';
            gameEnded = true;
        } else { // Đổi lượt chơi
            nextMove = (nextMove == 'X') ? 'O' : 'X';
        }
    }

    // Kiểm tra người chơi hiện tại có thắng sau nước đi ở (row, col) không
    bool checkWin(int row, int col) {
        // Kiểm tra hàng ngang
        bool win = true;
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[row][j] != nextMove) win = false;
        if (win) return true;

        // Kiểm tra cột dọc
        win = true;
        for (int i = 0; i < BOARD_SIZE; i++)
            if (board[i][col] != nextMove) win = false;
        if (win) return true;

        // Kiểm tra đường chéo chính
        if (row == col) {
            win = true;
            for (int i = 0; i < BOARD_SIZE; i++)
                if (board[i][i] != nextMove) win = false;
            if (win) return true;
        }

        // Kiểm tra đường chéo phụ
        if (row + col == BOARD_SIZE - 1) {
            win = true;
            for (int i = 0; i < BOARD_SIZE; i++)
                if (board[i][BOARD_SIZE-1-i] != nextMove) win = false;
            if (win) return true;
        }

        return false;
    }

    // Kiểm tra bàn cờ đã đầy chưa (hòa)
    bool checkDraw() {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                if (board[i][j] == ' ') return false; // Còn ô trống
        return true; // Không còn ô trống
    }

    // Gọi lượt đi cho AI
    void botMove() {
        if (mode == VS_AI_EASY) {
            easyBotMove(); // AI dễ: chọn ngẫu nhiên ô trống
        } else {
            hardBotMove(); // AI khó: dùng minimax để chọn nước đi tối ưu
        }
    }

private:
    // AI dễ: chọn ngẫu nhiên 1 ô trống để đi
    void easyBotMove() {
        std::vector<std::pair<int,int>> emptyCells;
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                if (board[i][j] == ' ')
                    emptyCells.push_back({i,j});

        if (!emptyCells.empty()) {
            auto move = emptyCells[rand() % emptyCells.size()];
            this->move(move.first, move.second);
        }
    }

    // AI khó: tìm nước đi tốt nhất bằng thuật toán minimax
    void hardBotMove() {
        int bestScore = -1000;
        std::pair<int,int> bestMove = {-1,-1};

        // Duyệt tất cả các ô trống
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O'; // Giả lập nước đi
                    int score = minimax(0, false); // Tính điểm
                    board[i][j] = ' '; // Quay lui

                    if (score > bestScore) { // Cập nhật nước đi tốt nhất
                        bestScore = score;
                        bestMove = {i,j};
                    }
                }
            }
        }

        // Thực hiện nước đi tốt nhất
        if (bestMove.first != -1) {
            this->move(bestMove.first, bestMove.second);
        }
    }

    // Thuật toán Minimax với cắt tỉa Alpha-Beta
    int minimax(int depth, bool isMaximizing, int alpha = -1000, int beta = 1000) {
        if (checkWinFor('O')) return 10 - depth;  // Nếu AI thắng
        if (checkWinFor('X')) return -10 + depth; // Nếu người chơi thắng
        if (checkDraw()) return 0;                // Nếu hòa

        if (isMaximizing) { // Lượt của AI
            int bestScore = -1000;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'O';
                        int score = minimax(depth+1, false, alpha, beta);
                        board[i][j] = ' ';
                        bestScore = std::max(score, bestScore);
                        alpha = std::max(alpha, bestScore);
                        if (beta <= alpha) break; // Cắt tỉa
                    }
                }
            }
            return bestScore;
        } else { // Lượt của người chơi
            int bestScore = 1000;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'X';
                        int score = minimax(depth+1, true, alpha, beta);
                        board[i][j] = ' ';
                        bestScore = std::min(score, bestScore);
                        beta = std::min(beta, bestScore);
                        if (beta <= alpha) break; // Cắt tỉa
                    }
                }
            }
            return bestScore;
        }
    }

    // Kiểm tra xem một người chơi ('X' hoặc 'O') có thắng không
    bool checkWinFor(char player) {
        // Kiểm tra hàng và cột
        for (int i = 0; i < BOARD_SIZE; i++) {
            bool rowWin = true, colWin = true;
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] != player) rowWin = false;
                if (board[j][i] != player) colWin = false;
            }
            if (rowWin || colWin) return true;
        }

        // Kiểm tra hai đường chéo
        bool diag1 = true, diag2 = true;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][i] != player) diag1 = false;
            if (board[i][BOARD_SIZE-1-i] != player) diag2 = false;
        }
        return diag1 || diag2;
    }
};

#endif
