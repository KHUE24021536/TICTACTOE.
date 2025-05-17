TICTACTOE – Game Đánh Cờ Caro Kinh Điển
Bài tập lớn môn Lập trình nâng cao (INT2215_7)

* Họ và tên: Nguyễn Minh Khuê
* Mã sinh viên: 24021536
* Lớp: K69CS4 - UET - VNU
* Tên dự án: TicTacToe

---

💡 Ý tưởng

Trò chơi "Tic Tac Toe" (cờ caro 3x3) là một trong những game đơn giản nhưng rất phổ biến, phù hợp để áp dụng các kiến thức về đồ họa, xử lý sự kiện, AI và cấu trúc chương trình. Dự án này được xây dựng lại với:

* Giao diện đồ họa trực quan bằng thư viện SDL2.
* Tích hợp 3 chế độ chơi: Người với người, Người với AI dễ, Người với AI khó (Minimax).
* Hỗ trợ tính năng phát hiện thắng/thua/hòa, đổi lượt người chơi, và reset trò chơi.

---

🕹️ Mô tả trò chơi & Hướng dẫn

# Lối chơi:

* Hai người chơi lần lượt đánh dấu 'X' hoặc 'O' trên bảng 3x3.
* Người thắng là người đầu tiên có 3 dấu liền nhau theo hàng, cột hoặc đường chéo.
* Nếu bàn cờ đầy mà không ai thắng thì là hòa.

# Chế độ:

* PvP – Người chơi vs Người chơi.
* VS AI (EASY) – Chơi với máy đánh ngẫu nhiên.
* VS AI (HARD) – Chơi với máy sử dụng thuật toán Minimax có cắt tỉa Alpha-Beta.

---

 🛠️ Các kỹ thuật sử dụng

* ✅ Thư viện SDL2.0 để xử lý đồ họa, âm thanh và sự kiện.
* ✅ Xử lý người dùng: bắt sự kiện chuột, phím và chọn ô hợp lệ.
* ✅ Render bảng, chữ, nút, và hiệu ứng trực quan.
* ✅ Tổ chức chương trình rõ ràng theo module: `logic.h`, `defs.h`, `graphics.cpp`, `main.cpp`.
* ✅ Minimax + Alpha-Beta Pruning để làm AI chơi "khó".
* ✅ Random move cho AI dễ.
* ✅ Phân biệt màu sắc rõ ràng cho từng người chơi ('X' và 'O').
* ✅ Dễ dàng mở rộng, chỉnh sửa và nâng cấp.

---

# 🔗 Nguồn tham khảo

* SDL2 Documentation: tài liệu thầy gửi.
* Kiến thức về Minimax và Alpha-Beta Pruning: ChatGpt.


