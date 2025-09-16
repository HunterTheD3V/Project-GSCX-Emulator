#include "cell_ir.h"
#include "logger.h"
#include <string>

namespace gscx::cell {

// Stub: traduz IR para pseudo x86-64 (aqui apenas logging)
void translate_to_x86(const BlockIR& blk) {
    Logger::info("Translator: iniciando tradução CELL->x86-64 (stub)");
    for (const auto& ins : blk.instrs) {
        switch (ins.kind) {
        case OpKind::Nop:
            Logger::info("  emit: nop");
            break;
        case OpKind::Add:
            Logger::info("  emit: add reg, reg/imm");
            break;
        case OpKind::Return:
            Logger::info("  emit: ret");
            break;
        default:
            Logger::warn("  emit: op não suportada (stub)");
            break;
        }
    }
}

} // namespace gscx::cell