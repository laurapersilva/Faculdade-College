// felzenszwalb_stb.cpp
// Compilar:  g++.exe -O2 -std=c++17 -I. felzenszwalb_stb.cpp -o felz_stb.exe
// Uso: ./felz_stb.exe image.png 500 50

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <unordered_map>
#include <chrono>

// Definições para stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

// Estrutura para representar um pixel RGB
struct Pixel {
    unsigned char r, g, b;
};

struct Aresta {
    int u, v;
    double w;
    bool operator<(const Aresta& other) const {
        return w < other.w;
    }
};

// DSU com tamanho do componente e internal difference
struct DSU {
    vector<int> pai;
    vector<int> tamanho;
    vector<double> internal; // Int(C): maior peso interno
    vector<int> rankv;

    DSU(int n) {
        pai.resize(n);
        tamanho.resize(n, 1);
        internal.resize(n, 0.0);
        rankv.resize(n, 0);
        for (int i = 0; i < n; ++i) pai[i] = i;
    }

    int findp(int x) {
        if (pai[x] != x) pai[x] = findp(pai[x]);
        return pai[x];
    }

    // une a e b usando a aresta de peso w
    void unite(int a, int b, double w) {
        a = findp(a);
        b = findp(b);
        if (a == b) return;
        // union by rank
        if (rankv[a] < rankv[b]) swap(a, b);
        pai[b] = a;
        tamanho[a] += tamanho[b];
        // internal difference do novo componente é o máximo entre os internos e o peso desta aresta
        internal[a] = max(internal[a], max(internal[b], w));
        if (rankv[a] == rankv[b]) rankv[a]++;
    }

    int size(int x) {
        x = findp(x);
        return tamanho[x];
    }

    double getInternal(int x) {
        x = findp(x);
        return internal[x];
    }
};

// Função para calcular a diferença de cor entre dois pixels
double diffPixel(const Pixel& a, const Pixel& b) {
    double dr = double(a.r) - double(b.r);
    double dg = double(a.g) - double(b.g);
    double db = double(a.b) - double(b.b);
    return sqrt(dr*dr + dg*dg + db*db);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " entrada.png [K] [min_size]\n";
        return 0;
    }

    string filename = argv[1];
    double K = 500.0;
    int min_size = 20;

    if (argc >= 3) K = atof(argv[2]);
    if (argc >= 4) min_size = atoi(argv[3]);

    int w, h, channels;
    // Carregar imagem com stb_image, forçando 3 canais (RGB)
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &channels, 3);

    if (!data) {
        cerr << "Erro ao carregar imagem: " << filename << endl;
        return -1;
    }

    int N = h * w;

    vector<Aresta> arestas;
    arestas.reserve(2 * N);

    // Construir grafo 4-conectado (não-direcionado)
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int id = y * w + x;
            // Acesso ao pixel: data[(y * w + x) * 3 + channel_index]
            Pixel p1 = {data[(y * w + x) * 3 + 0], data[(y * w + x) * 3 + 1], data[(y * w + x) * 3 + 2]};

            if (x + 1 < w) {
                // Vizinho à direita
                Pixel p2 = {data[(y * w + (x + 1)) * 3 + 0], data[(y * w + (x + 1)) * 3 + 1], data[(y * w + (x + 1)) * 3 + 2]};
                double peso = diffPixel(p1, p2);
                arestas.push_back({id, id + 1, peso});
            }
            if (y + 1 < h) {
                // Vizinho abaixo
                Pixel p2 = {data[((y + 1) * w + x) * 3 + 0], data[((y + 1) * w + x) * 3 + 1], data[((y + 1) * w + x) * 3 + 2]};
                double peso = diffPixel(p1, p2);
                arestas.push_back({id, id + w, peso});
            }
        }
    }

    cout << "Vertices: " << N << ", Arestas: " << arestas.size() << "\n";
    cout << "K = " << K << ", min_size = " << min_size << "\n";
    auto start = std::chrono::high_resolution_clock::now();
    // Ordenar arestas por peso crescente
    sort(arestas.begin(), arestas.end());

    // Inicializar DSU
    DSU dsu(N);

    // Versão modificada do Kruskal (Felzenszwalb & Huttenlocher)
    for (const auto &e : arestas) {
        int a = dsu.findp(e.u);
        int b = dsu.findp(e.v);
        if (a == b) continue;

        double MIntA = dsu.getInternal(a);
        double MIntB = dsu.getInternal(b);

        double thrA = MIntA + K / dsu.size(a);
        double thrB = MIntB + K / dsu.size(b);

        double limite = min(thrA, thrB);

        if (e.w <= limite) {
            dsu.unite(a, b, e.w);
        }
    }

    // Pós-processamento: unir componentes pequenas com vizinhos (usar novamente as arestas ordenadas)
    for (const auto &e : arestas) {
        int a = dsu.findp(e.u);
        int b = dsu.findp(e.v);
        if (a == b) continue;
        if (dsu.size(a) < min_size || dsu.size(b) < min_size) {
            dsu.unite(a, b, e.w);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    cout << "Tempo de execucao: " << duration.count() << " ms" << endl;

    // Gerar cores para cada componente
    unordered_map<int, Pixel> color;
    color.reserve(N);

    // Gerador aleatório de cores (consistente)
    std::mt19937 rng(123456);
    std::uniform_int_distribution<int> dist(0, 255);

    // Buffer de saída (3 canais por pixel)
    unsigned char* output_data = new unsigned char[N * 3];

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int id = y * w + x;
            int r = dsu.findp(id);
            auto it = color.find(r);
            
            Pixel c;
            if (it == color.end()) {
                c = {(unsigned char)dist(rng), (unsigned char)dist(rng), (unsigned char)dist(rng)};
                color[r] = c;
            } else {
                c = it->second;
            }
            
            // Escrever no buffer de saída
            output_data[(y * w + x) * 3 + 0] = c.r;
            output_data[(y * w + x) * 3 + 1] = c.g;
            output_data[(y * w + x) * 3 + 2] = c.b;
        }
    }

    // Salvar resultados com stb_image_write
    string outname = "saida_segmentada_stb.png";
    int success = stbi_write_png(outname.c_str(), w, h, 3, output_data, w * 3);

    if (success) {
        cout << "Segmentacao salva em: " << outname << "\n";
    } else {
        cerr << "Erro ao salvar imagem: " << outname << "\n";
    }
    
    cout << "Numero de componentes: " << color.size() << "\n";

    // Liberar memória
    stbi_image_free(data);
    delete[] output_data;

    return 0;
}
