#include "../include/chess_engine.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h> // Cần cho SetConsoleOutputCP và SetConsoleCP
#endif

void printBoard(const chess::Board& board) {
    std::cout << "\nBàn cờ hiện tại:" << std::endl;
    std::cout << board << std::endl;
}

void compareAlgorithms(const chess::Board& board, int depth) {
    ChessEngine engine;
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "SO SÁNH THUẬT TOÁN MINIMAX VÀ ALPHA-BETA PRUNING" << std::endl;
    std::cout << "Độ sâu tìm kiếm: " << depth << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Test Minimax
    auto minimaxMove = engine.findBestMoveMinimax(board, depth);
    long long minimaxNodes = engine.getNodesEvaluated();
    double minimaxTime = engine.getSearchTime();
    
    // Test Alpha-Beta
    auto alphaBetaMove = engine.findBestMoveAlphaBeta(board, depth);
    long long alphaBetaNodes = engine.getNodesEvaluated();
    double alphaBetaTime = engine.getSearchTime();
    
    // So sánh kết quả
    std::cout << "\n" << std::string(60, '-') << std::endl;
    std::cout << "KẾT QUẢ SO SÁNH:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    std::cout << "Minimax:" << std::endl;
    std::cout << "  - Nước đi: " << chess::uci::moveToUci(minimaxMove) << std::endl;
    std::cout << "  - Nodes đánh giá: " << minimaxNodes << std::endl;
    std::cout << "  - Thời gian: " << minimaxTime << " ms" << std::endl;
    
    std::cout << "\nAlpha-Beta Pruning:" << std::endl;
    std::cout << "  - Nước đi: " << chess::uci::moveToUci(alphaBetaMove) << std::endl;
    std::cout << "  - Nodes đánh giá: " << alphaBetaNodes << std::endl;
    std::cout << "  - Thời gian: " << alphaBetaTime << " ms" << std::endl;
    
    // Tính toán hiệu quả
    if (minimaxNodes > 0) {
        double nodeReduction = ((double)(minimaxNodes - alphaBetaNodes) / minimaxNodes) * 100;
        double timeReduction = ((minimaxTime - alphaBetaTime) / minimaxTime) * 100;
        
        std::cout << "\nHiệu quả của Alpha-Beta Pruning:" << std::endl;
        std::cout << "  - Giảm " << nodeReduction << "% số nodes đánh giá" << std::endl;
        std::cout << "  - Giảm " << timeReduction << "% thời gian tìm kiếm" << std::endl;
    }
    
    std::cout << std::string(60, '=') << std::endl;
}

int main() {
#ifdef _WIN32
    // Thiết lập code page của console output sang UTF-8 trên Windows
    SetConsoleOutputCP(CP_UTF8);
    // Thiết lập code page của console input sang UTF-8 trên Windows (nếu cần nhập liệu tiếng Việt)
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "CHƯƠNG TRÌNH DEMO THUẬT TOÁN MINIMAX VÀ ALPHA-BETA PRUNING" << std::endl;
    std::cout << "Dành cho cờ vua - Sử dụng thư viện chess-library" << std::endl;
    
    try {
        // Khởi tạo bàn cờ từ vị trí ban đầu
        chess::Board board;
        
        printBoard(board);
        
        int choice;
        std::cout << "\nChọn chế độ:" << std::endl;
        std::cout << "1. Chỉ chạy Minimax" << std::endl;
        std::cout << "2. Chỉ chạy Alpha-Beta Pruning" << std::endl;
        std::cout << "3. So sánh cả hai thuật toán" << std::endl;
        std::cout << "4. Chơi với máy (Alpha-Beta)" << std::endl;
        std::cout << "Lựa chọn của bạn: ";
        std::cin >> choice;
        
        int depth;
        std::cout << "Nhập độ sâu tìm kiếm (khuyến nghị 3-5): ";
        std::cin >> depth;
        
        ChessEngine engine;
        
        switch (choice) {
            case 1: {
                auto move = engine.findBestMoveMinimax(board, depth);
                std::cout << "Nước đi được đề xuất: " << chess::uci::moveToUci(move) << std::endl;
                break;
            }
            case 2: {
                auto move = engine.findBestMoveAlphaBeta(board, depth);
                std::cout << "Nước đi được đề xuất: " << chess::uci::moveToUci(move) << std::endl;
                break;
            }
            case 3: {
                compareAlgorithms(board, depth);
                break;
            }
            case 4: {
                std::cout << "\nChế độ chơi với máy (bạn là quân trắng):" << std::endl;
                
                while (board.isGameOver().second == chess::GameResult::NONE) {
                    printBoard(board);
                    
                    if (board.sideToMove() == chess::Color::WHITE) {
                        // Lượt người chơi
                        std::string moveStr;
                        std::cout << "Nhập nước đi của bạn (ví dụ: e2e4): ";
                        std::cin >> moveStr;
                        
                        try {
                            auto move = chess::uci::uciToMove(board, moveStr);
                            board.makeMove(move);
                        } catch (const std::exception& e) {
                            std::cout << "Nước đi không hợp lệ! Thử lại." << std::endl;
                            continue;
                        }
                    } else {
                        // Lượt máy
                        std::cout << "Máy đang suy nghĩ..." << std::endl;
                        auto move = engine.findBestMoveAlphaBeta(board, depth);
                        board.makeMove(move);
                        std::cout << "Máy đi: " << chess::uci::moveToUci(move) << std::endl;
                    }
                }
                
                std::cout << "\nGame kết thúc!" << std::endl;
                auto gameStatus = board.isGameOver();

                if (gameStatus.first == chess::GameResultReason::CHECKMATE) {
                    std::cout << "Checkmate! ";
                    // Khi bị chiếu hết, sideToMove() là bên thua cuộc.
                    std::cout << (board.sideToMove() == chess::Color::WHITE ? "Đen thắng!" : "Trắng thắng!") << std::endl;
                } else if (gameStatus.first == chess::GameResultReason::STALEMATE) {
                    std::cout << "Hòa do hết nước đi (Stalemate)!" << std::endl;
                } else if (gameStatus.first == chess::GameResultReason::FIFTY_MOVE_RULE) {
                    std::cout << "Hòa do luật 50 nước!" << std::endl;
                } else if (gameStatus.first == chess::GameResultReason::THREEFOLD_REPETITION) {
                    std::cout << "Hòa do lặp lại thế cờ 3 lần!" << std::endl;
                } else if (gameStatus.first == chess::GameResultReason::INSUFFICIENT_MATERIAL) {
                    std::cout << "Hòa do không đủ lực lượng để chiếu hết!" << std::endl;
                } else if (gameStatus.second != chess::GameResult::NONE) { // Các trường hợp hòa hoặc kết thúc khác
                    std::cout << "Ván cờ kết thúc không rõ lý do cụ thể (có thể là Draw agreed, etc.)" << std::endl;
                } else {
                    // Trường hợp này không nên xảy ra nếu vòng lặp while ở trên đã thoát
                    std::cout << "Ván cờ kết thúc với trạng thái không xác định." << std::endl;
                }
                break;
            }
            default:
                std::cout << "Lựa chọn không hợp lệ!" << std::endl;
                break;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Lỗi: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 