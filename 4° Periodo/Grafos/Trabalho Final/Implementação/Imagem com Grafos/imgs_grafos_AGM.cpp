// IMAGEM --> GRAFO NAO DIRECIONADO --> ARVORE GERADORA MINIMA

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct Aresta {
    int u, v;
    double w;

    bool operator<(const Aresta& other) const {
        return w < other.w;
    }
};

// DSU
struct DSU {
    vector<int> pai, rank;

    DSU(int n) {
        pai.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++)
            pai[i] = i;
    }

    int acha_v(int x) {
        if (pai[x] != x)
            pai[x] = acha_v(pai[x]);
        return pai[x];
    }

    void unir_vertices(int a, int b) {
        a = acha_v(a);
        b = acha_v(b);
        if (a != b) {
            if (rank[a] < rank[b]) swap(a, b);
            pai[b] = a;
            if (rank[a] == rank[b]) rank[a]++;
        }
    }
};

// Diferença entre pixels
double diffPixel(const Vec3b& a, const Vec3b& b) {
    double dx = a[0] - b[0];
    double dy = a[1] - b[1];
    double dz = a[2] - b[2];
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// KRUSKAL
pair<double, vector<Aresta>> kruskal(int n, vector<Aresta>& aresta) {
    sort(aresta.begin(), aresta.end());
    DSU dsu(n);

    vector<Aresta> agm;
    double total = 0.0;

    for (auto& e : aresta) {
        if (dsu.acha_v(e.u) != dsu.acha_v(e.v)) {
            dsu.unir_vertices(e.u, e.v);
            agm.push_back(e);
            total += e.w;
        }
    }

    return {total, agm};
}


int main() {
    Mat img = imread("entrada.png");

    if (img.empty()) {
        cout << "Erro ao carregar imagem!\n";
        return 0;
    }

    int h = img.rows;
    int w = img.cols;
    int N = h * w;

    vector<Aresta> aresta;
    aresta.reserve(2 * N);

    // Criar grafo NAO-DIRECIONADO (4 conexões)
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            int id = y * w + x;

            // direita
            if (x + 1 < w) {
                double peso = diffPixel(img.at<Vec3b>(y, x), img.at<Vec3b>(y, x + 1));
                aresta.push_back({id, id + 1, peso});
            }

            // abaixo
            if (y + 1 < h) {
                double peso = diffPixel(img.at<Vec3b>(y, x), img.at<Vec3b>(y + 1, x));
                aresta.push_back({id, id + w, peso});
            }
        }
    }

    cout << "Imagem convertida para grafo NAO-DIRECIONADO.\n";
    cout << "Vertices: " << N << "\n";
    cout << "Arestas: " << aresta.size() << "\n";

    // já rodando Kruskal
    auto [custo, agm] = kruskal(N, aresta);
    cout << "Custo total da AGM: " << custo << endl;

    return 0;
}
