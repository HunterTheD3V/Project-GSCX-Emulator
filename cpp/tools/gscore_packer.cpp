#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

// Pequena ferramenta (standalone) para empacotar um bundle GSCore
// Uso: gscore_packer out.gscb type name file [type name file] ...

namespace fs = std::filesystem;

static bool write_exact(std::ofstream& f, const void* src, size_t n) { return bool(f.write(reinterpret_cast<const char*>(src), n)); }

int main(int argc, char** argv) {
    if (argc < 5 || ((argc - 2) % 3 != 0)) {
        std::cerr << "Uso: gscore_packer out.gscb type name file [...repete]" << std::endl;
        return 1;
    }
    std::string outPath = argv[1];
    struct EntryIn { uint16_t type; std::string name; std::vector<uint8_t> data; };
    std::vector<EntryIn> ins;
    for (int i = 2; i < argc; i += 3) {
        uint16_t type = static_cast<uint16_t>(std::stoi(argv[i]));
        std::string name = argv[i+1];
        std::string file = argv[i+2];
        std::ifstream f(file, std::ios::binary);
        if (!f) { std::cerr << "Falha ao abrir: " << file << std::endl; return 1; }
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(f)), {});
        ins.push_back({type, name, std::move(data)});
    }
    std::ofstream out(outPath, std::ios::binary);
    if (!out) { std::cerr << "Falha ao criar: " << outPath << std::endl; return 1; }
    uint32_t magic = 0x47534352; uint16_t ver = 1; uint16_t count = static_cast<uint16_t>(ins.size());
    write_exact(out, &magic, 4); write_exact(out, &ver, 2); write_exact(out, &count, 2);
    uint32_t offset = 4 + 2 + 2; // header
    // calcular tamanho da tabela
    for (auto& e : ins) {
        uint16_t nameLen = static_cast<uint16_t>(e.name.size());
        offset += 2 + 2 + nameLen + 4 + 4;
    }
    uint32_t cur = offset;
    // escrever entradas
    for (auto& e : ins) {
        uint16_t type = e.type; uint16_t nameLen = static_cast<uint16_t>(e.name.size()); uint32_t size = static_cast<uint32_t>(e.data.size());
        write_exact(out, &type, 2); write_exact(out, &nameLen, 2); write_exact(out, e.name.data(), nameLen);
        write_exact(out, &cur, 4); write_exact(out, &size, 4);
        cur += size;
    }
    // escrever blobs
    for (auto& e : ins) {
        if (!e.data.empty()) write_exact(out, e.data.data(), e.data.size());
    }
    std::cout << "Bundle criado: " << outPath << std::endl;
    return 0;
}