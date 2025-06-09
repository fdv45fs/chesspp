#ifndef MINIMAX_H
#define MINIMAX_H

#include <chess.hpp>
#include <limits>
#include <utility>

class Minimax {
public:
    static constexpr int INFINITY_VALUE = 999999;
    static constexpr int NEGATIVE_INFINITY = -999999;
    
    // Constructor
    Minimax();
    
    // Thuật toán minimax chính
    std::pair<chess::Move, int> findBestMove(const chess::Board& board, int depth);
    
    // Lấy số node đã đánh giá
    long long getNodesEvaluated() const { return nodesEvaluated; }
    
    // Reset thống kê
    void resetStats() { nodesEvaluated = 0; }

private:
    long long nodesEvaluated;
    
    // Hàm minimax đệ quy
    int minimax(chess::Board& board, int depth, bool isMaximizing);
    
    // Hàm đánh giá vị trí
    int evaluatePosition(const chess::Board& board);
};

#endif // MINIMAX_H 