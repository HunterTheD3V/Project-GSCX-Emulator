#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace gscx::cell {

// Minimal IR for CELL PPC64 (PPE) and SPE (SPU) ops
enum class OpKind : uint16_t {
    Nop,
    Add,
    Sub,
    And,
    Or,
    Xor,
    Load,
    Store,
    Branch,
    Call,
    Return,
    // TODO: extend with PPC/SPU specific ops
};

struct Operand {
    enum class Type : uint8_t { Reg, Imm, Mem };
    Type type{Type::Reg};
    uint8_t reg{0};
    int64_t imm{0};
    uint64_t addr{0};
};

struct InstrIR {
    OpKind kind{OpKind::Nop};
    Operand dst{};
    Operand src1{};
    Operand src2{};
};

struct BlockIR {
    uint64_t ppu_pc{0};
    std::vector<InstrIR> instrs;
};

} // namespace gscx::cell