#include "../include/chess_engine.h"
#include "../include/minimax.h"
#include "../include/alpha_beta.h"
#include <iostream>

ChessEngine::ChessEngine() : searchDepth(4), nodesEvaluated(0), searchTimeMs(0.0) {}

ChessEngine::~ChessEngine() {}

chess::Move ChessEngine::findBestMoveMinimax(const chess::Board& board, int depth) {
    std::cout << "\n=== SỬ DỤNG THUẬT TOÁN MINIMAX ===" << std::endl;
    
    startTimer();
    
    Minimax minimax;
    chess::Move bestMove = minimax.findBestMove(board, depth);
    
    stopTimer();
    
    nodesEvaluated = minimax.getNodesEvaluated();
    
    std::cout << "Thời gian tìm kiếm: " << searchTimeMs << " ms" << std::endl;
    std::cout << "================================\n" << std::endl;
    
    return bestMove;
}

chess::Move ChessEngine::findBestMoveAlphaBeta(const chess::Board& board, int depth) {
    std::cout << "\n=== SỬ DỤNG THUẬT TOÁN ALPHA-BETA PRUNING ===" << std::endl;
    
    startTimer();
    
    AlphaBeta alphaBeta;
    chess::Move bestMove = alphaBeta.findBestMove(board, depth);
    
    stopTimer();
    
    nodesEvaluated = alphaBeta.getNodesEvaluated();
    
    std::cout << "Thời gian tìm kiếm: " << searchTimeMs << " ms" << std::endl;
    std::cout << "Hiệu quả cắt tỉa: " << alphaBeta.getPruningCount() << " lần" << std::endl;
    std::cout << "============================================\n" << std::endl;
    
    return bestMove;
}

void ChessEngine::resetStats() {
    nodesEvaluated = 0;
    searchTimeMs = 0.0;
}

void ChessEngine::startTimer() {
    searchStartTime = std::chrono::high_resolution_clock::now();
}

void ChessEngine::stopTimer() {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - searchStartTime);
    searchTimeMs = duration.count() / 1000.0; // Chuyển đổi sang milliseconds
} 