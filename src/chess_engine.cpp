#include "../include/chess_engine.h"
#include "../include/minimax.h"
#include "../include/alpha_beta.h"
#include <iostream>

ChessEngine::ChessEngine() : searchTimeMs(0.0) {}

ChessEngine::~ChessEngine() {}

SearchResult ChessEngine::findBestMoveMinimax(const chess::Board& board, int depth) {
    std::cout << "\n> Đang tính toán với Minimax..." << std::endl;
    
    startTimer();
    Minimax minimax;
    auto [bestMove, bestEval] = minimax.findBestMove(board, depth);
    stopTimer();
    
    SearchResult result;
    result.move = bestMove;
    result.eval = bestEval;
    result.nodes = minimax.getNodesEvaluated();
    result.time_ms = searchTimeMs;
    // result.prunings is 0 by default

    return result;
}

SearchResult ChessEngine::findBestMoveAlphaBeta(const chess::Board& board, int depth) {
    std::cout << "\n> Đang tính toán với Alpha-Beta Pruning..." << std::endl;
    
    startTimer();
    
    AlphaBeta alphaBeta;
    auto [bestMove, bestEval] = alphaBeta.findBestMove(board, depth);
    
    stopTimer();
    
    SearchResult result;
    result.move = bestMove;
    result.eval = bestEval;
    result.nodes = alphaBeta.getNodesEvaluated();
    result.time_ms = searchTimeMs;
    result.prunings = alphaBeta.getPruningCount();
    
    return result;
}

void ChessEngine::resetStats() {
    // This is now handled within the algorithm classes
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