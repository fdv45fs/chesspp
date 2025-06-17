#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include "chess.hpp"
#include "transposition_table.h"
#include <string>
#include <vector>
#include <chrono>
#include <memory>

// Một cấu trúc để giữ tất cả kết quả từ một cuộc tìm kiếm
struct SearchResult {
    chess::Move move = chess::Move::NO_MOVE;
    int eval = 0;
    double time_ms = 0.0;
    long long nodes = 0;
    long long prunings = 0;
    long long tt_hits = 0;
};

class ChessEngine {
public:
    // Constructor sẽ khởi tạo TTable
    ChessEngine();

    // Tìm nước đi tốt nhất sử dụng minimax đơn giản
    SearchResult findBestMoveMinimax(const chess::Board& board, int depth);

    // Tìm nước đi tốt nhất sử dụng alpha-beta pruning
    SearchResult findBestMoveAlphaBeta(const chess::Board& board, int depth);
    
    // Tìm nước đi tốt nhất sử dụng alpha-beta pruning với bảng hoán vị
    SearchResult findBestMoveAlphaBetaTT(const chess::Board& board, int depth);

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
    
    std::unique_ptr<TranspositionTable> tt;
    int current_age = 0; // Biến để theo dõi "tuổi" của mỗi lượt tìm kiếm
    
    void startTimer();
    void stopTimer();
};

#endif //CHESS_ENGINE_H 