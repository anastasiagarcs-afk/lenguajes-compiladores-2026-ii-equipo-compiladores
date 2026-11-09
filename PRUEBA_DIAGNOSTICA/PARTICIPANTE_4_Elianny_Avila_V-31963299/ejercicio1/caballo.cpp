#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>

struct Pos { int col; int row; };

static bool parse_pos(const std::string& s, Pos& p) {
    if (s.size() < 2) return false;
    if (s[0] < 'a' || s[0] > 'h') return false;
    if (s[1] < '1' || s[1] > '8') return false;
    p.col = s[0] - 'a';
    p.row = s[1] - '1';
    return true;
}

static bool es_mov_caballo(Pos a, Pos b) {
    int dc = std::abs(b.col - a.col);
    int dr = std::abs(b.row - a.row);
    return (dc == 1 && dr == 2) || (dc == 2 && dr == 1);
}

// 0 = correcto, 1 = movimiento invalido, 2 = pieza distinta de K
static int validar(Pos ini, const std::vector<std::string>& movs,
                   int& falla_idx, char& letra_mala) {
    Pos actual = ini;
    for (int i = 0; i < (int)movs.size(); ++i) {
        const std::string& m = movs[i];
        if (m.empty() || m[0] != 'K') {
            letra_mala = m.empty() ? '?' : m[0];
            falla_idx = i + 1;
            return 2;
        }
        Pos dest;
        if (!parse_pos(m.substr(1), dest)) { falla_idx = i + 1; return 1; }
        if (!es_mov_caballo(actual, dest)) { falla_idx = i + 1; return 1; }
        actual = dest;
    }
    return 0;
}

static void imprimir_resultado(int r, int idx, char letra) {
    if (r == 0) std::cout << "movimiento CORRECTO\n";
    else if (r == 2) std::cout << "movimiento INCORRECTO falla letra " << letra
                               << " no pertenece a un caballo\n";
    else std::cout << "movimiento INCORRECTO falla en el movimiento " << idx << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <pos_inicial> <n> <mov1> ... <movN>\n";
        std::cerr << "     " << argv[0] << " --bench <pos_inicial> <archivo>\n";
        return 1;
    }

    // Modo benchmark
    if (std::string(argv[1]) == "--bench") {
        if (argc < 4) { std::cerr << "Falta archivo\n"; return 1; }
        Pos ini;
        if (!parse_pos(argv[2], ini)) { std::cerr << "Pos inicial invalida\n"; return 1; }

        std::ifstream f(argv[3]);
        if (!f) { std::cerr << "No se pudo abrir archivo\n"; return 1; }
        std::vector<std::string> movs;
        std::string tok;
        while (f >> tok) movs.push_back(tok);
        f.close();

        int idx = 0; char letra = 0;
        validar(ini, movs, idx, letra); // warm-up

        const int rep = 10;
        auto t0 = std::chrono::high_resolution_clock::now();
        for (int k = 0; k < rep; ++k) validar(ini, movs, idx, letra);
        auto t1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> d = t1 - t0;
        double total = d.count();
        double prom  = total / rep;

        std::ofstream o("resultados/tiempos_cpp.txt");
        o << std::fixed << std::setprecision(9);
        o << "Lenguaje: C++\n";
        o << "n movimientos: " << movs.size() << "\n";
        o << "repeticiones: " << rep << "\n";
        o << "tiempo total (s): " << total << "\n";
        o << "tiempo promedio (s): " << prom << "\n";
        o << "tiempo promedio (ms): " << (prom * 1000.0) << "\n";
        if (idx == 0) o << "resultado: CORRECTO\n";
        else if (letra) o << "resultado: INCORRECTO (letra " << letra
                          << " en mov " << idx << ")\n";
        else o << "resultado: INCORRECTO en mov " << idx << "\n";
        o.close();

        std::cout << "Benchmark C++ listo -> resultados/tiempos_cpp.txt\n";
        if (idx == 0) std::cout << "Estado de la secuencia: CORRECTO\n";
        else std::cout << "Estado de la secuencia: INCORRECTO en mov " << idx << "\n";
        return 0;
    }

    // Modo prueba normal
    Pos ini;
    if (!parse_pos(argv[1], ini)) { std::cerr << "Pos inicial invalida\n"; return 1; }
    int n = std::stoi(argv[2]);
    std::vector<std::string> movs;
    for (int i = 0; i < n; ++i) {
        if (3 + i >= argc) { std::cerr << "Faltan movimientos\n"; return 1; }
        movs.push_back(argv[3 + i]);
    }
    int idx = 0; char letra = 0;
    int r = validar(ini, movs, idx, letra);
    imprimir_resultado(r, idx, letra);
    return 0;
}