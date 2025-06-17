#ifndef AB_TT_H
#define AB_TT_H

#include "../include/chess_engine.h"
#include "../include/transposition_table.h"

// Hàm bao bọc (wrapper) để khởi động tìm kiếm Alpha-Beta với TTable
SearchResult findBestMoveAlphaBetaTT(chess::Board& board, TranspositionTable& tt, int depth, int age);

#endif // AB_TT_H 