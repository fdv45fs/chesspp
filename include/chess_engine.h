#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <chess.hpp>
#include <memory>
#include <chrono>

class ChessEngine {
public:
    ChessEngine();
    ~ChessEngine();
    
    // Tìm nước đi tốt nhất sử dụng minimax
    chess::Move findBestMoveMinimax(const chess::Board& board, int depth);
    
    // Tìm nước đi tốt nhất sử dụng alpha-beta pruning
    chess::Move findBestMoveAlphaBeta(const chess::Board& board, int depth);
    
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