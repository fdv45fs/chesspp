#include "../include/transposition_table.h"
#include <iostream>

TranspositionTable::TranspositionTable(size_t sizeInMB) {
    numEntries = (sizeInMB * 1024 * 1024) / sizeof(TTEntry);
    table.resize(numEntries);
    clear();
    std::cout << "Bảng hoán vị được khởi tạo với " << numEntries << " entries." << std::endl;
}

void TranspositionTable::clear() {
    // Đặt tất cả các entry về giá trị 0
    std::fill(table.begin(), table.end(), TTEntry{});
}

TTEntry* TranspositionTable::probe(uint64_t key) {
    size_t index = getIndex(key);
    TTEntry* entry = &table[index];

    // Chỉ trả về entry nếu key khớp, nếu không đây là entry của một thế cờ khác
    if (entry->zobrist_key == key) {
        return entry;
    }

    return nullptr;
}

void TranspositionTable::store(uint64_t key, int depth, int score, NodeType type, chess::Move best_move, int age) {
    size_t index = getIndex(key);
    TTEntry* entry = &table[index];

    // Chiến lược thay thế: Ưu tiên ghi đè entry cũ (age khác)
    // hoặc entry mới có độ sâu lớn hơn.
    if (entry->age != age || depth >= entry->depth) {
        entry->zobrist_key = key;
        entry->depth = depth;
        entry->score = score;
        entry->type = type;
        entry->best_move = best_move;
        entry->age = age;
    }
}

size_t TranspositionTable::getIndex(uint64_t key) const {
    // Sử dụng phép toán modulo để ánh xạ key vào một chỉ số trong bảng
    return key % numEntries;
} 