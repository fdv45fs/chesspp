#include "../include/chess_engine.h"
#include "../include/evaluation.h"
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h> // Cần cho SetConsoleOutputCP và SetConsoleCP
#endif

void printBoard(const chess::Board& board) {
    std::cout << "\nBàn cờ hiện tại:" << std::endl;
    std::cout << board << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void compareAlgorithms(const chess::Board& board, int depth) {
    ChessEngine engine;
    
    auto minimaxResult = engine.findBestMoveMinimax(board, depth);
    auto alphaBetaResult = engine.findBestMoveAlphaBeta(board, depth);
    
    std::cout << "\n\n" << std::string(60, '=') << std::endl;
    std::cout << "      SO SÁNH THUẬT TOÁN (Độ sâu: " << depth << ")" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << "Minimax:" << std::endl;
    std::cout << "  - Nước đi: " << chess::uci::moveToUci(minimaxResult.move) << std::endl;
    std::cout << "  - Đánh giá: " << minimaxResult.eval << std::endl;
    std::cout << "  - Nodes đánh giá: " << minimaxResult.nodes << std::endl;
    std::cout << "  - Thời gian: " << minimaxResult.time_ms << " ms" << std::endl;
    
    std::cout << "\nAlpha-Beta Pruning:" << std::endl;
    std::cout << "  - Nước đi: " << chess::uci::moveToUci(alphaBetaResult.move) << std::endl;
    std::cout << "  - Đánh giá: " << alphaBetaResult.eval << std::endl;
    std::cout << "  - Nodes đánh giá: " << alphaBetaResult.nodes << std::endl;
    std::cout << "  - Số lần cắt tỉa: " << alphaBetaResult.prunings << std::endl;
    std::cout << "  - Thời gian: " << alphaBetaResult.time_ms << " ms" << std::endl;
    
    // Tính toán hiệu quả
    if (minimaxResult.nodes > 0 && minimaxResult.time_ms > 0) {
        double nodeReduction = ((double)(minimaxResult.nodes - alphaBetaResult.nodes) / minimaxResult.nodes) * 100;
        double timeReduction = (minimaxResult.time_ms - alphaBetaResult.time_ms) / minimaxResult.time_ms * 100;
        
        std::cout << "\nHiệu quả của Alpha-Beta Pruning:" << std::endl;
        std::cout << "  - Giảm " << nodeReduction << "% số nodes đánh giá" << std::endl;
        std::cout << "  - Giảm " << timeReduction << "% thời gian tìm kiếm" << std::endl;
    }
    
    std::cout << std::string(60, '=') << std::endl;
}

void processFenCsv(int depth) {
    std::ifstream inputFile("fen_input.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Lỗi: Không thể mở file fen_input.csv" << std::endl;
        return;
    }

    std::ofstream outputFile("fen_output.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Lỗi: Không thể tạo file fen_output.csv" << std::endl;
        return;
    }

    outputFile << "fen,best_move,evaluation\n"; // Write header

    std::string line;
    ChessEngine engine;
    int count = 0;

    // Skip header line of input file if it exists
    if (std::getline(inputFile, line)) {
        // We just read the header, do nothing with it
    }

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string fen;
        // Get the FEN, which is the part before the first comma
        std::getline(ss, fen, ',');

        // Trim potential whitespace/carriage returns
        fen.erase(fen.find_last_not_of(" \n\r\t")+1);

        if (fen.empty()) {
            continue;
        }
        
        count++;
        std::cout << "Đang xử lý FEN #" << count << ": " << fen << std::endl;

        chess::Board board;
        if (!board.setFen(fen)) {
            std::cerr << "  Lỗi: FEN không hợp lệ. Bỏ qua." << std::endl;
            outputFile << "\"" << fen << "\",INVALID_FEN,0\n";
            continue;
        }
        
        SearchResult searchResult = engine.findBestMoveAlphaBeta(board, depth);
        
        if (searchResult.move == chess::Move::NO_MOVE) {
             std::cout << "  Không tìm thấy nước đi hợp lệ (game over?)." << std::endl;
             outputFile << "\"" << fen << "\",NO_LEGAL_MOVES,0\n";
             continue;
        }

        // Make the best move to evaluate the resulting position
        chess::Board boardAfterMove = board;
        boardAfterMove.makeMove(searchResult.move);
        int finalEval = Evaluation::evaluateBoard(boardAfterMove);
        
        // Convert the move to SAN for the output file
        std::string sanMove = chess::uci::moveToSan(board, searchResult.move);

        outputFile << "\"" << fen << "\"," << sanMove << "," << finalEval << "\n";
    }

    std::cout << "\nHoàn tất! Đã xử lý " << count << " FEN(s)." << std::endl;
    std::cout << "Kết quả đã được ghi vào fen_output.csv" << std::endl;

    inputFile.close();
    outputFile.close();
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
        int choice;
        std::cout << "\nChọn chế độ:" << std::endl;
        std::cout << "1. Chỉ chạy Minimax" << std::endl;
        std::cout << "2. Chỉ chạy Alpha-Beta Pruning" << std::endl;
        std::cout << "3. So sánh cả hai thuật toán" << std::endl;
        std::cout << "4. Xử lý file FEN CSV" << std::endl;
        std::cout << "5. Chơi với máy (Alpha-Beta)" << std::endl;
        std::cout << "Lựa chọn của bạn: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Lựa chọn không hợp lệ!" << std::endl;
            return 1;
        }
        
        if (choice == 4) {
            int depth;
            std::cout << "Nhập độ sâu tìm kiếm cho việc xử lý file CSV: ";
            std::cin >> depth;
            if(std::cin.fail()){
                 std::cout << "Độ sâu không hợp lệ!" << std::endl;
                 return 1;
            }
            processFenCsv(depth);
        } else if ((choice >= 1 && choice <= 3) || choice == 5) {
            chess::Board board;
            int board_choice;
            std::cout << "\nChọn thế cờ:" << std::endl;
            std::cout << "1. Bắt đầu từ thế cờ mặc định" << std::endl;
            std::cout << "2. Nhập từ FEN" << std::endl;
            std::cout << "Lựa chọn của bạn: ";
            std::cin >> board_choice;

            if (board_choice == 2) {
                std::string fen;
                std::cout << "Nhập FEN: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, fen);
                if (!board.setFen(fen)) {
                    std::cerr << "Lỗi: FEN string không hợp lệ!" << std::endl;
                    return 1;
                }
            }
            
            printBoard(board);
            
            int depth = 0;
            std::cout << "Nhập độ sâu tìm kiếm (khuyến nghị 3-5): ";
            std::cin >> depth;
            
            ChessEngine engine;
            
            if(std::cin.fail()){
                 std::cout << "Độ sâu không hợp lệ!" << std::endl;
                 return 1;
            }

            switch (choice) {
                case 1: {
                    auto result = engine.findBestMoveMinimax(board, depth);
                    std::cout << "\n--- Kết quả Minimax ---" << std::endl;
                    std::cout << "Nước đi tốt nhất: " << chess::uci::moveToUci(result.move) << std::endl;
                    std::cout << "Đánh giá: " << result.eval << std::endl;
                    std::cout << "Số node đã đánh giá: " << result.nodes << std::endl;
                    std::cout << "Thời gian tìm kiếm: " << result.time_ms << " ms" << std::endl;
                    std::cout << "----------------------" << std::endl;
                    break;
                }
                case 2: {
                    auto result = engine.findBestMoveAlphaBeta(board, depth);
                    std::cout << "\n--- Kết quả Alpha-Beta ---" << std::endl;
                    std::cout << "Nước đi tốt nhất: " << chess::uci::moveToUci(result.move) << std::endl;
                    std::cout << "Đánh giá: " << result.eval << std::endl;
                    std::cout << "Số node đã đánh giá: " << result.nodes << std::endl;
                    std::cout << "Số lần cắt tỉa: " << result.prunings << std::endl;
                    std::cout << "Thời gian tìm kiếm: " << result.time_ms << " ms" << std::endl;
                    std::cout << "--------------------------" << std::endl;
                    break;
                }
                case 3: {
                    compareAlgorithms(board, depth);
                    break;
                }
                case 5: {
                    std::cout << "\nChế độ chơi với máy:" << std::endl;
                    if(board.sideToMove() == chess::Color::BLACK) {
                         std::cout << "FEN được cung cấp có lượt đi của Đen. Máy sẽ đi trước." << std::endl;
                    } else {
                         std::cout << "Bạn đi trước (quân Trắng)." << std::endl;
                    }

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
                            auto result = engine.findBestMoveAlphaBeta(board, depth);
                            board.makeMove(result.move);
                            std::cout << "Máy đi: " << chess::uci::moveToUci(result.move) 
                                      << " (Đánh giá: " << result.eval << ")" << std::endl;
                        }
                    }
                    
                    std::cout << "\nGame kết thúc!" << std::endl;
                    auto gameStatus = board.isGameOver();

                    if (gameStatus.first == chess::GameResultReason::CHECKMATE) {
                        std::cout << "Checkmate! ";
                        std::cout << (board.sideToMove() == chess::Color::WHITE ? "Đen thắng!" : "Trắng thắng!") << std::endl;
                    } else if (gameStatus.first == chess::GameResultReason::STALEMATE) {
                        std::cout << "Hòa do hết nước đi (Stalemate)!" << std::endl;
                    } else if (gameStatus.first == chess::GameResultReason::FIFTY_MOVE_RULE) {
                        std::cout << "Hòa do luật 50 nước!" << std::endl;
                    } else if (gameStatus.first == chess::GameResultReason::THREEFOLD_REPETITION) {
                        std::cout << "Hòa do lặp lại thế cờ 3 lần!" << std::endl;
                    } else if (gameStatus.first == chess::GameResultReason::INSUFFICIENT_MATERIAL) {
                        std::cout << "Hòa do không đủ lực lượng để chiếu hết!" << std::endl;
                    } else if (gameStatus.second != chess::GameResult::NONE) {
                        std::cout << "Ván cờ kết thúc không rõ lý do cụ thể." << std::endl;
                    } else {
                        std::cout << "Ván cờ kết thúc với trạng thái không xác định." << std::endl;
                    }
                    break;
                }
            }
        } else {
            std::cout << "Lựa chọn không hợp lệ!" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Lỗi: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 