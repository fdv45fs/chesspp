#ifndef EVALUATION_H
#define EVALUATION_H

#include <chess.hpp> // Cần cho chess::Board, chess::Piece, chess::Color, chess::PieceType

class Evaluation {
public:
    // Giá trị cơ bản cho các quân cờ (có thể điều chỉnh)
    static constexpr int PAWN_VALUE = 100;
    static constexpr int KNIGHT_VALUE = 300;
    static constexpr int BISHOP_VALUE = 310; // Thường Tượng được đánh giá cao hơn Mã một chút
    static constexpr int ROOK_VALUE = 500;
    static constexpr int QUEEN_VALUE = 900;
    // Vua không có giá trị material trong cách tính này vì không thể bị bắt.

    /**
     * @brief Đánh giá bàn cờ một cách đơn giản bằng cách tính tổng giá trị quân cờ.
     * 
     * @param board Đối tượng bàn cờ hiện tại.
     * @return int Điểm số từ góc nhìn của quân Trắng (điểm Trắng - điểm Đen).
     *             Giá trị dương nghĩa là Trắng có lợi thế material.
     *             Giá trị âm nghĩa là Đen có lợi thế material.
     */
    static int evaluateBoardSimple(const chess::Board& board);

private:
    // Hàm trợ giúp để lấy giá trị vật chất của một loại quân cờ
    static int getPieceMaterialValue(chess::PieceType pieceType);
};

#endif // EVALUATION_H 