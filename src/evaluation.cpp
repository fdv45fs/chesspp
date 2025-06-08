#include "../include/evaluation.h"

// Hàm trợ giúp để lấy giá trị vật chất của một loại quân cờ
int Evaluation::getPieceMaterialValue(chess::PieceType pieceType) {
    if (pieceType == chess::PieceType::PAWN)   return PAWN_VALUE;
    else if (pieceType == chess::PieceType::KNIGHT) return KNIGHT_VALUE;
    else if (pieceType == chess::PieceType::BISHOP) return BISHOP_VALUE;
    else if (pieceType == chess::PieceType::ROOK)   return ROOK_VALUE;
    else if (pieceType == chess::PieceType::QUEEN)  return QUEEN_VALUE;
    else if (pieceType == chess::PieceType::KING)   return 0; // Vua không tính điểm material
    // else if (pieceType == chess::PieceType::NONE) return 0; // Đã được bao gồm trong else cuối
    else return 0; // Cho chess::PieceType::NONE hoặc các trường hợp không xác định
}

int Evaluation::evaluateBoardSimple(const chess::Board& board) {
    int whiteMaterial = 0;
    int blackMaterial = 0;

    // Duyệt qua tất cả các ô trên bàn cờ
    // Cách duyệt ô cờ có thể khác nhau tùy theo thư viện chess.hpp
    // Giả sử thư viện cung cấp cách duyệt qua tất cả các ô từ SQ_A1 đến SQ_H8
    // Hoặc có thể dùng board.pieces(color, pieceType) để lấy danh sách quân cờ

    // Cách 1: Duyệt qua từng ô (0-63)
    for (int i = 0; i < 64; ++i) {
        chess::Square sq = static_cast<chess::Square>(i); // Cần kiểm tra tính hợp lệ của static_cast này với chess.hpp
        chess::Piece piece = board.at(sq);

        if (piece.type() != chess::PieceType::NONE) { // Chỉ xét nếu ô có quân cờ
            int pieceValue = getPieceMaterialValue(piece.type());
            if (piece.color() == chess::Color::WHITE) {
                whiteMaterial += pieceValue;
            } else {
                blackMaterial += pieceValue;
            }
        }
    }

    // Cách 2 (thường hiệu quả hơn nếu thư viện hỗ trợ): Duyệt qua các bitboard của từng loại quân
    // Ví dụ (cần API cụ thể của chess.hpp):
    // for (chess::PieceType pt : {chess::PieceType::PAWN, chess::PieceType::KNIGHT, chess::PieceType::BISHOP, chess::PieceType::ROOK, chess::PieceType::QUEEN}) {
    //     chess::Bitboard white_pieces = board.pieces(chess::Color::WHITE, pt);
    //     chess::Bitboard black_pieces = board.pieces(chess::Color::BLACK, pt);
    //     whiteMaterial += static_cast<int>(white_pieces.count()) * getPieceMaterialValue(pt);
    //     blackMaterial += static_cast<int>(black_pieces.count()) * getPieceMaterialValue(pt);
    // }

    return whiteMaterial - blackMaterial;
} 