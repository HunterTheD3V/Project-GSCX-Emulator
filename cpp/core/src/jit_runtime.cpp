#include "logger.h"
#include <vector>
#include <cstdint>

namespace gscx {

// Stub: runtime JIT que apenas simula execução
class JitRuntime {
public:
    using CodeBuffer = std::vector<uint8_t>;
    void reset() { code_.clear(); }
    void emit(uint8_t b) { code_.push_back(b); }
    int execute() {
        Logger::info("JIT: executando buffer (stub)");
        return 0; // sucesso
    }
private:
    CodeBuffer code_;
};

} // namespace gscx