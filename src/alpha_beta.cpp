#include "../include/alpha_beta.h"
#include "../include/evaluation.h"
#include <iostream>
#include <vector>

AlphaBeta::AlphaBeta() : nodesEvaluated(0), pruningCount(0) {}

chess::Move AlphaBeta::findBestMove(const chess::Board& board, int depth) {
    resetStats();
    
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    
    if (moves.empty()) {
        return chess::Move::NO_MOVE;
    }
    
    chess::Move bestMove = moves[0];
    int bestValue = NEGATIVE_INFINITY;
    int alpha = NEGATIVE_INFINITY;
    int beta = INFINITY_VALUE;
    
    std::cout << "Alpha-Beta Pruning đang tìm kiếm với độ sâu " << depth << "..." << std::endl;
    
    for (const auto& move : moves) {
        chess::Board tempBoard = board;
        tempBoard.makeMove(move);
        
        // Gọi alpha-beta với isMaximizing = false vì chúng ta vừa thực hiện nước đi của mình
        int value = alphaBeta(tempBoard, depth - 1, alpha, beta, false);
        
        std::cout << "Nước đi: " << chess::uci::moveToUci(move) 
                  << " - Giá trị: " << value << std::endl;
        
        if (value > bestValue) {
            bestValue = value;
            bestMove = move;
        }
        
        // Cập nhật alpha cho root level
        alpha = std::max(alpha, value);
    }
    
    std::cout << "Nước đi tốt nhất: " << chess::uci::moveToUci(bestMove) 
              << " với giá trị: " << bestValue << std::endl;
    std::cout << "Số node đã đánh giá: " << nodesEvaluated << std::endl;
    std::cout << "Số lần cắt tỉa: " << pruningCount << std::endl;
    
    return bestMove;
}

int AlphaBeta::alphaBeta(chess::Board& board, int depth, int alpha, int beta, bool isMaximizing) {
    nodesEvaluated++;
    
    // Điều kiện dừng: độ sâu = 0 hoặc game kết thúc
    // Sử dụng .second != chess::GameResult::NONE để kiểm tra game đã kết thúc hay chưa
    if (depth == 0 || board.isGameOver().second != chess::GameResult::NONE) {
        return evaluatePosition(board);
    }
    
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    
    if (isMaximizing) {
        int maxEval = NEGATIVE_INFINITY;
        
        for (const auto& move : moves) {
            board.makeMove(move);
            int eval = alphaBeta(board, depth - 1, alpha, beta, false);
            board.unmakeMove(move);
            
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            
            // Beta cutoff (cắt tỉa)
            if (beta <= alpha) {
                pruningCount++;
                break; // Cắt tỉa - không cần đánh giá các nước đi còn lại
            }
        }
        
        return maxEval;
    } else {
        int minEval = INFINITY_VALUE;
        
        for (const auto& move : moves) {
            board.makeMove(move);
            int eval = alphaBeta(board, depth - 1, alpha, beta, true);
            board.unmakeMove(move);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            // Alpha cutoff (cắt tỉa)
            if (beta <= alpha) {
                pruningCount++;
                break; // Cắt tỉa - không cần đánh giá các nước đi còn lại
            }
        }
        
        return minEval;
    }
}

// Hàm đánh giá vị trí, giờ sử dụng Evaluation::evaluateBoardSimple
int AlphaBeta::evaluatePosition(const chess::Board& board) {
    auto gameStatus = board.isGameOver();

    // gameStatus.first là GameResultReason, gameStatus.second là GameResult
    if (gameStatus.first == chess::GameResultReason::CHECKMATE) {
        // board.sideToMove() là bên bị chiếu hết (thua cuộc).
        // Hàm đánh giá của Minimax/AlphaBeta nên trả về điểm từ góc nhìn của Trắng.
        // Nếu Trắng bị chiếu hết -> điểm xấu nhất cho Trắng (-INF).
        // Nếu Đen bị chiếu hết -> điểm tốt nhất cho Trắng (+INF).
        return (board.sideToMove() == chess::Color::WHITE) ? NEGATIVE_INFINITY : INFINITY_VALUE;
    } else if (gameStatus.second != chess::GameResult::NONE) { 
        // Các trường hợp hòa khác (STALEMATE, INSUFFICIENT_MATERIAL, FIFTY_MOVE_RULE, THREEFOLD_REPETITION etc.)
        // board.isGameOver().second sẽ không phải là NONE nếu ván cờ đã kết thúc.
        return 0; // Hòa cờ
    }
    
    // Gọi hàm đánh giá material đơn giản
    return Evaluation::evaluateBoardSimple(board);
} 