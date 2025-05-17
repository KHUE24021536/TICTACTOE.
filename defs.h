#ifndef DEFS_H
#define DEFS_H

#include <SDL.h>

// Kích thước cửa sổ game
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 700;

// Tiêu đề cửa sổ
const char* WINDOW_TITLE = "Tic Tac Toe";

const int BOARD_SIZE = 3;

// Màu nền (Background) - xám nhạt
const SDL_Color BG_COLOR = {245, 245, 245, 255};

// Màu đường kẻ bảng - xanh cô-ban nhạt và có độ trong suốt nhẹ
const SDL_Color BOARD_COLOR = {100, 149, 237, 150};

// Màu cho quân 'X' - đỏ cam đậm
const SDL_Color COLOR_X = {255, 69, 0, 255};

// Màu cho quân 'O' - xanh lá cây đậm
const SDL_Color COLOR_O = {0, 128, 0, 255};

// Màu nút bấm mặc định - xanh dương
const SDL_Color BUTTON_COLOR = {66, 135, 245, 255};

// Màu nút bấm khi di chuột hover - xanh dương sáng hơn
const SDL_Color BUTTON_HOVER = {100, 181, 246, 255};

// Màu chữ - xám đen đậm
//const SDL_Color TEXT_COLOR = {255, 0, 0, 255};
const SDL_Color TEXT_COLOR = {255, 255, 255, 255};

// Tọa độ góc trên bên trái của bảng
const int BOARD_X = 150;
const int BOARD_Y = 200;

// Kích thước bảng (rộng và cao đều 300 pixel)
const int BOARD_WIDTH = 300;
const int BOARD_HEIGHT = 300;

// Các màn hình có thể hiển thị
enum Screen {
    WELCOME, // Màn hình chào mừng
    GAME     // Màn hình chơi game chính
};

// Các chế độ chơi
enum GameMode {
    PVP,          // Người đấu người
    VS_AI_EASY,   // Người đấu AI dễ
    VS_AI_HARD    // Người đấu AI khó
};

#endif
