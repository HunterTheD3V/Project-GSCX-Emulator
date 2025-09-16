#include "gscore_format.h"
#include "logger.h"
#include <fstream>

namespace gscx {

static bool read_exact(std::ifstream& f, void* dst, size_t n) { return bool(f.read(reinterpret_cast<char*>(dst), n)); }

bool load_gscore(const std::string& path, GSCoreBundle& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) { Logger::error("GSCore: falha ao abrir arquivo"); return false; }
    uint32_t magic{}; uint16_t ver{}; uint16_t count{};
    if (!read_exact(f, &magic, 4) || magic != GSCORE_MAGIC) { Logger::error("GSCore: magic inválido"); return false; }
    if (!read_exact(f, &ver, 2) || !read_exact(f, &count, 2)) { Logger::error("GSCore: header inválido"); return false; }
    out.version = ver;
    std::vector<GSCoreEntry> entries; entries.reserve(count);
    for (uint16_t i=0;i<count;i++) {
        uint16_t type, nameLen; uint32_t offset, size;
        if (!read_exact(f, &type, 2) || !read_exact(f, &nameLen, 2)) return false;
        std::string name(nameLen, '\0');
        if (!read_exact(f, name.data(), nameLen)) return false;
        if (!read_exact(f, &offset, 4) || !read_exact(f, &size, 4)) return false;
        entries.push_back(GSCoreEntry{type, name, offset, size});
    }
    // Ler payload completo
    f.seekg(0, std::ios::end);
    auto endpos = f.tellg();
    f.seekg(0, std::ios::beg);

    // Encontrar início do payload (menor offset entre entries)
    uint32_t min_off = UINT32_MAX; for (auto& e: entries) if (e.offset < min_off) min_off = e.offset;
    out.entries = std::move(entries);
    out.blob.resize(static_cast<size_t>(endpos) - min_off);
    f.seekg(min_off, std::ios::beg);
    if (!read_exact(f, out.blob.data(), out.blob.size())) return false;
    Logger::info("GSCore: bundle carregado");
    return true;
}

} // namespace gscx