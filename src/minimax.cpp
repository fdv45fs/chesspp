#include "../include/minimax.h"
#include "../include/evaluation.h"
#include <iostream>
#include <vector>

Minimax::Minimax() : nodesEvaluated(0) {}

chess::Move Minimax::findBestMove(const chess::Board& board, int depth) {
    resetStats();
    
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    
    if (moves.empty()) {
        return chess::Move::NO_MOVE;
    }
    
    chess::Move bestMove = moves[0];
    int bestValue = NEGATIVE_INFINITY;
    
    std::cout << "Minimax đang tìm kiếm với độ sâu " << depth << "..." << std::endl;
    
    for (const auto& move : moves) {
        chess::Board tempBoard = board;
        tempBoard.makeMove(move);
        
        // Gọi minimax với isMaximizing = false vì chúng ta vừa thực hiện nước đi của mình
        int value = minimax(tempBoard, depth - 1, false);
        
        std::cout << "Nước đi: " << chess::uci::moveToUci(move) 
                  << " - Giá trị: " << value << std::endl;
        
        if (value > bestValue) {
            bestValue = value;
            bestMove = move;
        }
    }
    
    std::cout << "Nước đi tốt nhất: " << chess::uci::moveToUci(bestMove) 
              << " với giá trị: " << bestValue << std::endl;
    std::cout << "Số node đã đánh giá: " << nodesEvaluated << std::endl;
    
    return bestMove;
}

int Minimax::minimax(chess::Board& board, int depth, bool isMaximizing) {
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
            int eval = minimax(board, depth - 1, false);
            board.unmakeMove(move);
            
            maxEval = std::max(maxEval, eval);
        }
        
        return maxEval;
    } else {
        int minEval = INFINITY_VALUE;
        
        for (const auto& move : moves) {
            board.makeMove(move);
            int eval = minimax(board, depth - 1, true);
            board.unmakeMove(move);
            
            minEval = std::min(minEval, eval);
        }
        
        return minEval;
    }
}

// Hàm đánh giá vị trí, giờ sử dụng Evaluation::evaluateBoardSimple
int Minimax::evaluatePosition(const chess::Board& board) {
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