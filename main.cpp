#include <SDL.h>
#include <SDL_ttf.h>
#include <ctime>
#include "graphics.h"
#include "logic.h"
#include "defs.h"

// Hàm main - điểm bắt đầu chương trình
int main(int argc, char* argv[]) {
    srand(time(NULL)); // Khởi tạo bộ sinh số ngẫu nhiên

    Graphics graphics;
    if (!graphics.init()) { // Khởi tạo đồ họa, kiểm tra lỗi
        SDL_Log("Failed to initialize graphics!");
        return 1;
    }

    Tictactoe game; // Tạo đối tượng trò chơi caro
    Screen currentScreen = WELCOME; // Màn hình hiện tại: màn hình chào mừng

    // Khởi tạo các nút chọn chế độ chơi
    Button buttons[3] = {
        { {200, 300, 200, 50}, "Player vs Player", false },
        { {200, 400, 200, 50}, "Easy AI", false },
        { {200, 500, 200, 50}, "Hard AI", false }
    };

    // Nút Reset và Menu sau khi trận đấu kết thúc
    Button resetBtn = { {200, 600, 100, 40}, "Reset", false };
    Button menuBtn = { {320, 600, 100, 40}, "Menu", false };

    SDL_Color textColor = TEXT_COLOR; // Màu chữ
    bool running = true; // Cờ điều khiển vòng lặp chính

    // Vòng lặp game
    while (running) {
        SDL_Event e;
        // Xử lý sự kiện
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false; // Người dùng thoát
            }
            else if (e.type == SDL_MOUSEMOTION) {
                SDL_Point mouse = {e.motion.x, e.motion.y};
                // Cập nhật trạng thái hover khi di chuột
                if (currentScreen == WELCOME) {
                    for (auto& btn : buttons) {
                        btn.isHovered = SDL_PointInRect(&mouse, &btn.rect);
                    }
                } else if (game.gameEnded) {
                    resetBtn.isHovered = SDL_PointInRect(&mouse, &resetBtn.rect);
                    menuBtn.isHovered = SDL_PointInRect(&mouse, &menuBtn.rect);
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point mouse = {e.button.x, e.button.y};

                if (currentScreen == WELCOME) {
                    // Chọn chế độ chơi khi click nút
                    for (int i = 0; i < 3; i++) {
                        if (SDL_PointInRect(&mouse, &buttons[i].rect)) {
                            GameMode mode = static_cast<GameMode>(i);
                            game.init(mode); // Khởi tạo game theo chế độ
                            currentScreen = GAME; // Chuyển màn hình
                            break;
                        }
                    }
                }
                else if (currentScreen == GAME) {
                    if (!game.gameEnded) {
                        // Người chơi chọn ô trên bàn cờ
                        int cellSize = BOARD_WIDTH / BOARD_SIZE;
                        if (mouse.x >= BOARD_X && mouse.x < BOARD_X + BOARD_WIDTH &&
                            mouse.y >= BOARD_Y && mouse.y < BOARD_Y + BOARD_HEIGHT) {
                            int col = (mouse.x - BOARD_X) / cellSize;
                            int row = (mouse.y - BOARD_Y) / cellSize;
                            game.move(row, col);

                            // Nếu đến lượt máy AI
                            if (!game.gameEnded && game.mode != PVP && game.nextMove == 'O') {
                                SDL_Delay(500); // Chờ 0.5s cho tự nhiên
                                game.botMove();
                            }
                        }
                    } else {
                        // Sau khi kết thúc trận đấu, click nút reset hoặc về menu
                        if (SDL_PointInRect(&mouse, &resetBtn.rect)) {
                            game.init(game.mode); // Chơi lại
                        } else if (SDL_PointInRect(&mouse, &menuBtn.rect)) {
                            currentScreen = WELCOME; // Quay lại màn hình chào
                        }
                    }
                }
            }
        }

        // Phần render (vẽ màn hình)
        SDL_SetRenderDrawColor(graphics.renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, 255);
        SDL_RenderClear(graphics.renderer);

        graphics.renderBackground(); // Vẽ nền

        if (currentScreen == WELCOME) {
            // Vẽ tiêu đề trò chơi
            SDL_Surface* surface = TTF_RenderText_Blended(graphics.fontLarge, "TIC TAC TOE", textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.renderer, surface);
            SDL_Rect rect = {SCREEN_WIDTH/2 - surface->w/2, 150, surface->w, surface->h};
            SDL_RenderCopy(graphics.renderer, texture, NULL, &rect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            // Vẽ các nút chọn chế độ chơi
            for (auto& btn : buttons) {
                graphics.renderButton(btn);
            }
        }
        else if (currentScreen == GAME) {
            graphics.renderBoard(game); // Vẽ bàn cờ

            if (game.gameEnded) {
                // Vẽ kết quả trận đấu
                std::string message;
                if (game.winner == 'X') message = "Player X wins!";
                else if (game.winner == 'O') message = (game.mode != PVP) ? "AI wins!" : "Player O wins!";
                else message = "Draw!";

                SDL_Surface* surface = TTF_RenderText_Blended(graphics.fontMedium, message.c_str(), textColor);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.renderer, surface);
                SDL_Rect rect = {SCREEN_WIDTH/2 - surface->w/2, 100, surface->w, surface->h};
                SDL_RenderCopy(graphics.renderer, texture, NULL, &rect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);

                // Vẽ các nút reset và menu
                graphics.renderButton(resetBtn);
                graphics.renderButton(menuBtn);
            }
        }

        SDL_RenderPresent(graphics.renderer); // Hiển thị nội dung lên cửa sổ
    }

    graphics.quit(); // Giải phóng tài nguyên
    return 0;
}
