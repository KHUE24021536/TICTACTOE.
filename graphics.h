#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <cmath>
#include "defs.h"
#include "logic.h"

// Cấu trúc Button đại diện cho nút bấm giao diện
struct Button {
    SDL_Rect rect;           // Vùng hình chữ nhật của nút
    std::string text;        // Nội dung văn bản hiển thị trên nút
    bool isHovered = false;  // Cờ kiểm tra chuột đang hover vào nút hay không

    // Constructor để khởi tạo Button
    Button(SDL_Rect rect, const std::string& text, bool isHovered = false)
        : rect(rect), text(text), isHovered(isHovered) {}
};

// Lớp Graphics chịu trách nhiệm vẽ toàn bộ giao diện đồ họa
class Graphics {
public:
    SDL_Window* window;           // Cửa sổ SDL
    SDL_Renderer* renderer;       // Bộ sinh hình SDL
    TTF_Font* fontLarge;          // Font chữ lớn
    TTF_Font* fontMedium;         // Font chữ vừa
    SDL_Texture* backgroundTexture; // Texture nền

    // Hàm khởi tạo SDL, cửa sổ, renderer, fonts, ảnh nền
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_Log("SDL không thể khởi tạo! SDL_Error: %s", SDL_GetError());
            return false;
        }

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (!window) {
            SDL_Log("Không thể tạo cửa sổ! SDL_Error: %s", SDL_GetError());
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            SDL_Log("Không thể tạo renderer! SDL_Error: %s", SDL_GetError());
            return false;
        }

        // Khởi tạo SDL_ttf để dùng font chữ
        if (TTF_Init() == -1) {
            SDL_Log("Không thể khởi tạo SDL_ttf! SDL_ttf Error: %s", TTF_GetError());
            return false;
        }

        // Tải font chữ
        fontLarge = TTF_OpenFont("arial.ttf", 48);
        fontMedium = TTF_OpenFont("arial.ttf", 24);
        if (!fontLarge || !fontMedium) {
            SDL_Log("Không thể tải font chữ! SDL_ttf Error: %s", TTF_GetError());
            return false;
        }

        // Khởi tạo SDL_image để xử lý ảnh PNG
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            SDL_Log("Không thể khởi tạo SDL_image! SDL_image Error: %s", IMG_GetError());
            return false;
        }

        // Tải texture ảnh nền
        backgroundTexture = loadTexture("background.png");
        if (!backgroundTexture) {
            SDL_Log("Không thể tải texture background!");
            return false;
        }

        return true;
    }

    // Hàm tải ảnh từ file thành SDL_Texture
    SDL_Texture* loadTexture(const std::string& path) {
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (!loadedSurface) {
            SDL_Log("Không thể tải ảnh %s! SDL_image Error: %s", path.c_str(), IMG_GetError());
            return nullptr;
        }

        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface); // Giải phóng surface sau khi đã tạo texture
        return newTexture;
    }

    // Vẽ ảnh nền ra toàn bộ màn hình
    void renderBackground() {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // Vẽ một Button ra màn hình
    void renderButton(Button& btn) {
        SDL_SetRenderDrawColor(renderer,
            btn.isHovered ? BUTTON_HOVER.r : BUTTON_COLOR.r,
            btn.isHovered ? BUTTON_HOVER.g : BUTTON_COLOR.g,
            btn.isHovered ? BUTTON_HOVER.b : BUTTON_COLOR.b,
            255);
        SDL_RenderFillRect(renderer, &btn.rect);

        // Vẽ chữ trên nút
        SDL_Surface* surface = TTF_RenderText_Blended(fontMedium, btn.text.c_str(), TEXT_COLOR);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect textRect = {
            btn.rect.x + (btn.rect.w - surface->w)/2,
            btn.rect.y + (btn.rect.h - surface->h)/2,
            surface->w, surface->h
        };

        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // Vẽ bàn cờ, ô cờ và X/O
    void renderBoard(Tictactoe& game) {
        // Vẽ nền bàn cờ
        SDL_SetRenderDrawColor(renderer, BOARD_COLOR.r, BOARD_COLOR.g, BOARD_COLOR.b, BOARD_COLOR.a);
        SDL_Rect boardRect = {BOARD_X, BOARD_Y, BOARD_WIDTH, BOARD_HEIGHT};
        SDL_RenderFillRect(renderer, &boardRect);

        // Vẽ các đường kẻ ô
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        int cellSize = BOARD_WIDTH / BOARD_SIZE;
        for (int i = 0; i <= BOARD_SIZE; i++) {
            // Kẻ đường dọc
            SDL_RenderDrawLine(renderer,
                BOARD_X + i * cellSize, BOARD_Y,
                BOARD_X + i * cellSize, BOARD_Y + BOARD_HEIGHT);
            // Kẻ đường ngang
            SDL_RenderDrawLine(renderer,
                BOARD_X, BOARD_Y + i * cellSize,
                BOARD_X + BOARD_WIDTH, BOARD_Y + i * cellSize);
        }

        // Vẽ X hoặc O trong từng ô
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (game.board[i][j] == 'X') {
                    drawX(i, j, cellSize);
                } else if (game.board[i][j] == 'O') {
                    drawO(i, j, cellSize);
                }
            }
        }
    }

    // Vẽ ký tự X trong ô (row, col)
    void drawX(int row, int col, int cellSize) {
        SDL_SetRenderDrawColor(renderer, COLOR_X.r, COLOR_X.g, COLOR_X.b, 255);
        int padding = cellSize / 4;
        int x = BOARD_X + col * cellSize;
        int y = BOARD_Y + row * cellSize;

        SDL_RenderDrawLine(renderer, x + padding, y + padding,
                          x + cellSize - padding, y + cellSize - padding);
        SDL_RenderDrawLine(renderer, x + padding, y + cellSize - padding,
                          x + cellSize - padding, y + padding);
    }

    // Vẽ ký tự O trong ô (row, col)
    void drawO(int row, int col, int cellSize) {
        SDL_SetRenderDrawColor(renderer, COLOR_O.r, COLOR_O.g, COLOR_O.b, 255);
        int radius = cellSize / 3;
        int centerX = BOARD_X + col * cellSize + cellSize / 2;
        int centerY = BOARD_Y + row * cellSize + cellSize / 2;

        // Vẽ hình tròn bằng cách nối các đoạn thẳng
        const int SEGMENTS = 36;
        for (int i = 0; i < SEGMENTS; i++) {
            double angle1 = 2 * M_PI * i / SEGMENTS;
            double angle2 = 2 * M_PI * (i + 1) / SEGMENTS;

            int x1 = centerX + radius * cos(angle1);
            int y1 = centerY + radius * sin(angle1);
            int x2 = centerX + radius * cos(angle2);
            int y2 = centerY + radius * sin(angle2);

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }

    // Giải phóng tài nguyên và thoát SDL
    void quit() {
        SDL_DestroyTexture(backgroundTexture);
        TTF_CloseFont(fontLarge);
        TTF_CloseFont(fontMedium);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }
};

#endif
