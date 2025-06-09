#ifndef EVALUATION_H
#define EVALUATION_H

#include <chess.hpp> // Cần cho chess::Board, chess::Piece, chess::Color, chess::PieceType
#include <array>

class Evaluation {
public:
    static int evaluateBoard(const chess::Board& board);

private:
    static void init_tables();

    // PeSTO's values
    static const int mg_value[6];
    static const int eg_value[6];

    static const int mg_pawn_table[64];
    static const int eg_pawn_table[64];
    static const int mg_knight_table[64];
    static const int eg_knight_table[64];
    static const int mg_bishop_table[64];
    static const int eg_bishop_table[64];
    static const int mg_rook_table[64];
    static const int eg_rook_table[64];
    static const int mg_queen_table[64];
    static const int eg_queen_table[64];
    static const int mg_king_table[64];
    static const int eg_king_table[64];
    
    static const int* mg_pesto_table[6];
    static const int* eg_pesto_table[6];

    static const int gamephaseInc[12];

    // Pre-computed tables
    static int mg_table[12][64];
    static int eg_table[12][64];

    static bool tables_initialized;
};

#endif // EVALUATION_H 