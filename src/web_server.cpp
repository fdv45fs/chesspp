#include "../include/httplib.h"
#include "../include/json.hpp"
#include "../include/chess_engine.h"
#include <iostream>
#include <memory>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

// Sử dụng namespace cho gọn
using json = nlohmann::json;

// Biến toàn cục để lưu trạng thái game
// (Trong một ứng dụng thực tế, bạn có thể muốn một cách quản lý state tốt hơn)
std::unique_ptr<chess::Board> game_board;
std::unique_ptr<ChessEngine> chess_engine;
int search_depth = 0;
chess::Color player_side = chess::Color::WHITE;

// Hàm gửi nước đi của máy tới UI bằng TCP Socket thô
void send_engine_move(const chess::Move& move) {
#ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return;
    }

    json move_json;
    auto uci_move = chess::uci::moveToUci(move);
    move_json["start"] = uci_move.substr(0, 2);
    move_json["end"] = uci_move.substr(2, 2);
    if (uci_move.length() > 4) {
        move_json["promotionPiece"] = uci_move.substr(4, 1);
    } else {
        move_json["promotionPiece"] = "";
    }
    std::string json_str = move_json.dump();
    std::cout << "Gửi nước đi của máy tới UI (TCP): " << json_str << std::endl;

    struct addrinfo *result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo("127.0.0.1", "5051", &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return;
    }

    SOCKET ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }
    
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to UI server on port 5051!" << std::endl;
        WSACleanup();
        return;
    }

    iResult = send(ConnectSocket, json_str.c_str(), (int)json_str.length(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Gửi nước đi thành công." << std::endl;
    }

    closesocket(ConnectSocket);
    WSACleanup();
#else
    // Triển khai cho Linux/macOS ở đây nếu cần
    std::cerr << "Gửi nước đi chưa được triển khai cho hệ điều hành này." << std::endl;
#endif
}


// Hàm để máy tính toán nước đi trong một luồng riêng
void think_and_move() {
    if (!game_board || !chess_engine) return;

    std::cout << "Máy đang suy nghĩ..." << std::endl;
    auto result = chess_engine->findBestMoveAlphaBeta(*game_board, search_depth);
    
    if(result.move != chess::Move::NO_MOVE) {
        game_board->makeMove(result.move);
        std::cout << "Máy đi: " << chess::uci::moveToUci(result.move) << std::endl;
        send_engine_move(result.move);
    } else {
        std::cout << "Máy không tìm thấy nước đi nào." << std::endl;
    }
}

void start_web_server() {
    httplib::Server svr;

    // Endpoint: /game (POST) - Bắt đầu ván cờ mới
    svr.Post("/game", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json game_info = json::parse(req.body);

            std::string side = game_info["playerSide"];
            search_depth = game_info["depth"];

            if (side == "White") {
                player_side = chess::Color::WHITE;
            } else {
                player_side = chess::Color::BLACK;
            }

            // Khởi tạo bàn cờ và engine
            game_board = std::make_unique<chess::Board>();
            chess_engine = std::make_unique<ChessEngine>();
            
            std::cout << "\n====================\n";
            std::cout << "Ván cờ mới bắt đầu!" << std::endl;
            std::cout << "Người chơi cầm quân: " << side << std::endl;
            std::cout << "Độ sâu tìm kiếm: " << search_depth << std::endl;
            std::cout << "====================\n";

            res.set_content("Game started successfully!", "text/plain");

             // Nếu người chơi cầm quân Đen, máy (Trắng) sẽ đi trước
            if (player_side == chess::Color::BLACK) {
                std::thread(think_and_move).detach();
            }

        } catch (const json::parse_error& e) {
            res.status = 400; // Bad Request
            res.set_content("Invalid JSON format", "text/plain");
            std::cerr << "Lỗi parse JSON: " << e.what() << std::endl;
        }
    });

    // Endpoint: /move (POST) - Nhận nước đi từ người chơi
    svr.Post("/move", [](const httplib::Request& req, httplib::Response& res) {
        if (!game_board || !chess_engine) {
            res.status = 400; // Bad Request
            res.set_content("Game not started. Please POST to /game first.", "text/plain");
            return;
        }

        try {
            json move_info = json::parse(req.body);
            std::string start = move_info["start"];
            std::string end = move_info["end"];
            std::string promotion = move_info["promotionPiece"];
            
            std::string uci_move_str = start + end;
            if (!promotion.empty()) {
                uci_move_str += promotion;
            }

            std::cout << "Nhận nước đi từ người chơi: " << uci_move_str << std::endl;

            // Thực hiện nước đi của người chơi
            try {
                auto move = chess::uci::uciToMove(*game_board, uci_move_str);
                game_board->makeMove(move);
                res.set_content("Move received.", "text/plain");

                // Sau khi người chơi đi, đến lượt máy
                // Chạy hàm suy nghĩ của máy trong một luồng riêng để không block request
                std::thread(think_and_move).detach();

            } catch(const std::exception& e) {
                res.status = 400; // Bad Request
                res.set_content("Invalid move: " + std::string(e.what()), "text/plain");
                std::cerr << "Nước đi không hợp lệ: " << e.what() << std::endl;
            }

        } catch (const json::parse_error& e) {
            res.status = 400; // Bad Request
            res.set_content("Invalid JSON format for move", "text/plain");
            std::cerr << "Lỗi parse JSON: " << e.what() << std::endl;
        }
    });
    
    // Endpoint /ping để kiểm tra server
    svr.Get("/ping", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("pong", "text/plain");
    });


    std::cout << "Máy chủ đang lắng nghe tại http://localhost:5050" << std::endl;
    svr.listen("localhost", 5050);
} 