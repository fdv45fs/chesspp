#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include "chess.hpp"
#include <vector>
#include <cstdint>

// Enum để chỉ định loại điểm số được lưu trữ
enum NodeType { 
    EXACT,      // score là giá trị chính xác
    LOWERBOUND, // score là cận dưới
    UPPERBOUND  // score là cận trên
};

// Cấu trúc cho một entry trong Bảng Hoán vị
struct TTEntry {
    uint64_t zobrist_key = 0;   // Khóa 64-bit để xác thực
    chess::Move best_move = chess::Move::NO_MOVE; // Nước đi tốt nhất tìm được
    int depth = 0;              // Độ sâu đã tìm kiếm
    int score = 0;              // Điểm số trả về của thế cờ
    NodeType type = EXACT;      // Loại điểm số
    int age = 0;                // Tuổi của entry, dùng cho chiến lược thay thế
};

class TranspositionTable {
public:
    // Khởi tạo bảng với kích thước cho trước (tính bằng MB)
    explicit TranspositionTable(size_t sizeInMB);

    // Xóa toàn bộ nội dung của bảng
    void clear();

    // Thăm dò bảng, trả về con trỏ tới entry nếu tìm thấy và key khớp
    TTEntry* probe(uint64_t key);

    // Lưu một entry vào bảng
    void store(uint64_t key, int depth, int score, NodeType type, chess::Move best_move, int age);

private:
    // Tính toán chỉ số trong bảng từ key
    size_t getIndex(uint64_t key) const;

    std::vector<TTEntry> table;
    size_t numEntries;
};

#endif // TRANSPOSITION_TABLE_H 