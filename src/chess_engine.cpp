#include "../include/chess_engine.h"
#include "../include/evaluation.h"
#include "../include/minimax.h"
#include "../include/alpha_beta.h"
#include "../include/ab_tt.h"
#include <iostream>

ChessEngine::ChessEngine() {
    // Khởi tạo Bảng hoán vị với kích thước 64MB
    tt = std::make_unique<TranspositionTable>(64);
}

SearchResult ChessEngine::findBestMoveMinimax(const chess::Board& board, int depth)
{
    current_age++; // Tăng tuổi cho lượt tìm kiếm mới
    Minimax minimax;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto [bestMove, eval] = minimax.findBestMove(board, depth);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    return {bestMove, eval, (double)duration.count(), minimax.getNodesEvaluated(), 0, 0};
}

SearchResult ChessEngine::findBestMoveAlphaBeta(const chess::Board& board, int depth)
{
    current_age++; // Tăng tuổi cho lượt tìm kiếm mới
    AlphaBeta alphaBeta;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto [bestMove, eval] = alphaBeta.findBestMove(board, depth);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    return {bestMove, eval, (double)duration.count(), alphaBeta.getNodesEvaluated(), alphaBeta.getPruningCount(), 0};
}

SearchResult ChessEngine::findBestMoveAlphaBetaTT(const chess::Board& board, int depth) {
    current_age++; // Tăng tuổi cho lượt tìm kiếm mới
    chess::Board boardCopy = board;
    return ::findBestMoveAlphaBetaTT(boardCopy, *tt, depth, current_age);
}
