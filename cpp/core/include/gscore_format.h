#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace gscx {

// Formato de bundle GSCore (conceitual):
// [Header]
//   magic: 'GSCR' (0x47534352)
//   version: uint16
//   count: uint16  (quantidade de entradas)
// [Entries]
//   Para cada entrada:
//     type: uint16 (1=CPU_CELL, 2=GPU_RSX, 3=RECOVERY, ...)
//     nameLen: uint16
//     name: bytes
//     offset: uint32 (a partir do início do arquivo)
//     size: uint32
// O payload de cada entrada pode ser uma DLL ou blob de dados.

struct GSCoreEntry {
    uint16_t type{};
    std::string name;
    uint32_t offset{};
    uint32_t size{};
};

struct GSCoreBundle {
    uint16_t version{1};
    std::vector<GSCoreEntry> entries;
    std::vector<uint8_t> blob; // dados concatenados
};

inline constexpr uint32_t GSCORE_MAGIC = 0x47534352; // 'GSCR'

} // namespace gscx