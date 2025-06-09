#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <chess.hpp>
#include <memory>
#include <chrono>
#include <string>
#include <utility>

// A structure to hold all results from a search
struct SearchResult {
    chess::Move move = chess::Move::NO_MOVE;
    int eval = 0;
    long long nodes = 0;
    double time_ms = 0.0;
    long long prunings = 0; // Only for alpha-beta, 0 otherwise
};

class ChessEngine {
public:
    ChessEngine();
    ~ChessEngine();
    
    // Tìm nước đi tốt nhất sử dụng minimax
    SearchResult findBestMoveMinimax(const chess::Board& board, int depth);
    
    // Tìm nước đi tốt nhất sử dụng alpha-beta pruning
    SearchResult findBestMoveAlphaBeta(const chess::Board& board, int depth);
    
    // Thiết lập độ sâu tìm kiếm
    void setSearchDepth(int depth) { searchDepth = depth; }
    
    // Lấy thống kê
    long long getNodesEvaluated() const { return nodesEvaluated; }
    double getSearchTime() const { return searchTimeMs; }
    
    void resetStats();

private:
    int searchDepth;
    long long nodesEvaluated;
    double searchTimeMs;
    std::chrono::high_resolution_clock::time_point searchStartTime;
    
    void startTimer();
    void stopTimer();
};

#endif // CHESS_ENGINE_H 