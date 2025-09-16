#include "cell_ir.h"
#include "logger.h"
#include <vector>

namespace gscx::cell {

// Stub: decodifica bytes PPC/SPU para IR simplificado
std::vector<InstrIR> decode_block(const uint8_t* code, size_t size) {
    std::vector<InstrIR> out;
    // TODO: real decoder. Por enquanto, gera um NOP e um RETURN para demonstrar fluxo
    out.push_back({OpKind::Nop, {}, {}, {}});
    out.push_back({OpKind::Return, {}, {}, {}});
    gscx::Logger::info("Decoder: bloco decodificado (stub)");
    return out;
}

} // namespace gscx::cell