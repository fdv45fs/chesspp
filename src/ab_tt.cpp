#include "../include/ab_tt.h"
#include "../include/evaluation.h"
#include <chrono>
#include <algorithm>
#include <iostream>

// Giá trị vô cực, dùng cho Alpha-Beta
const int INFINITY_ABTT = 1000000;
const int NEGATIVE_INFINITY_ABTT = -1000000;
const int MATE_SCORE_ABTT = 900000; // Điểm số cho chiếu hết

// Biến cục bộ cho file này để theo dõi thống kê
long long nodes_evaluated_abtt = 0;
long long tt_hits_abtt = 0;

// Hàm đệ quy chính, có thêm tham số TranspositionTable
int alphaBetaSearch(chess::Board& board, TranspositionTable& tt, int depth, int alpha, int beta, int age);

// Hàm bao bọc (wrapper) để khởi động tìm kiếm
SearchResult findBestMoveAlphaBetaTT(chess::Board& board, TranspositionTable& tt, int depth, int age) {
    nodes_evaluated_abtt = 0;
    tt_hits_abtt = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    int alpha = NEGATIVE_INFINITY_ABTT;
    int beta = INFINITY_ABTT;
    
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    
    if (moves.empty()) {
        return {chess::Move::NO_MOVE, Evaluation::evaluateBoard(board), 0, 0, 0, 0};
    }
    
    chess::Move bestMove = moves[0];
    int bestValue = NEGATIVE_INFINITY_ABTT;

    // Sắp xếp nước đi ở gốc, ưu tiên nước đi từ TTable nếu có
    TTEntry* ttEntry = tt.probe(board.hash());
    if (ttEntry && ttEntry->best_move != chess::Move::NO_MOVE) {
        for (chess::Movelist::size_type i = 0; i < moves.size(); ++i) {
            if (moves[i] == ttEntry->best_move) {
                std::swap(moves[0], moves[i]);
                break;
            }
        }
    }

    for (const auto& move : moves) {
        board.makeMove(move);
        int value = -alphaBetaSearch(board, tt, depth - 1, -beta, -alpha, age);
        board.unmakeMove(move);

        if (value > bestValue) {
            bestValue = value;
            bestMove = move;
        }
        alpha = std::max(alpha, value);
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    int finalEval = (board.sideToMove() == chess::Color::WHITE) ? bestValue : -bestValue;
    std::cout << "AB+TT Hits: " << tt_hits_abtt << std::endl;
    return {bestMove, finalEval, (double)duration.count(), nodes_evaluated_abtt, 0, tt_hits_abtt};
}

// Hàm tìm kiếm Alpha-Beta sử dụng thuật toán Negamax và TTable
int alphaBetaSearch(chess::Board& board, TranspositionTable& tt, int depth, int alpha, int beta, int age) {
    nodes_evaluated_abtt++;
    
    if (board.isGameOver().second != chess::GameResult::NONE) {
        if (board.isGameOver().first == chess::GameResultReason::CHECKMATE) return -MATE_SCORE_ABTT;
        return 0;
    }

    if (depth <= 0) {
        int perspective = (board.sideToMove() == chess::Color::WHITE) ? 1 : -1;
        return Evaluation::evaluateBoard(board) * perspective;
    }
    
    uint64_t key = board.hash();
    TTEntry* ttEntry = tt.probe(key);

    if (ttEntry != nullptr) {
        if (ttEntry->depth >= depth) {
            tt_hits_abtt++;
            if (ttEntry->type == EXACT) return ttEntry->score;
            if (ttEntry->type == LOWERBOUND) alpha = std::max(alpha, ttEntry->score);
            else if (ttEntry->type == UPPERBOUND) beta = std::min(beta, ttEntry->score);
            if (alpha >= beta) return ttEntry->score;
        }
    }
    
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

    if (ttEntry != nullptr && ttEntry->best_move != chess::Move::NO_MOVE) {
        for (chess::Movelist::size_type i = 0; i < moves.size(); ++i) {
            if (moves[i] == ttEntry->best_move) {
                std::swap(moves[0], moves[i]);
                break;
            }
        }
    }

    if (moves.empty()) return 0;

    chess::Move bestMoveForNode = moves[0];
    int bestScore = NEGATIVE_INFINITY_ABTT;
    NodeType type = UPPERBOUND;

    for (const auto& move : moves) {
        board.makeMove(move);
        int score = -alphaBetaSearch(board, tt, depth - 1, -beta, -alpha, age);
        board.unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;
            bestMoveForNode = move;
        }
        
        if (bestScore > alpha) {
            alpha = bestScore;
            type = EXACT;
        }

        if (alpha >= beta) {
            tt.store(key, depth, beta, LOWERBOUND, bestMoveForNode, age);
            return beta;
        }
    }

    tt.store(key, depth, bestScore, type, bestMoveForNode, age);
    return bestScore;
} 