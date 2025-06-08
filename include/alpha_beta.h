#ifndef ALPHA_BETA_H
#define ALPHA_BETA_H

#include <chess.hpp>
#include <limits>

class AlphaBeta {
public:
    static constexpr int INFINITY_VALUE = 999999;
    static constexpr int NEGATIVE_INFINITY = -999999;
    
    // Constructor
    AlphaBeta();
    
    // Thuật toán alpha-beta pruning chính
    chess::Move findBestMove(const chess::Board& board, int depth);
    
    // Lấy số node đã đánh giá
    long long getNodesEvaluated() const { return nodesEvaluated; }
    
    // Lấy số lần cắt tỉa
    long long getPruningCount() const { return pruningCount; }
    
    // Reset thống kê
    void resetStats() { 
        nodesEvaluated = 0; 
        pruningCount = 0;
    }

private:
    long long nodesEvaluated;
    long long pruningCount;
    
    // Hàm alpha-beta đệ quy
    int alphaBeta(chess::Board& board, int depth, int alpha, int beta, bool isMaximizing);
    
    // Hàm đánh giá vị trí
    int evaluatePosition(const chess::Board& board);
};

#endif // ALPHA_BETA_H 