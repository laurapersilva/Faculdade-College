// IMAGEM --> GRAFO DIRECIONADO --> ARBORECENCIA

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

struct Aresta {
    int u, v;
    double w;
};

const double INF = 1e18;

// DECLARAÇÃO DA FUNÇÃO EDMONDS
double edmonds(int raiz, int n, vector<Aresta> arestas);

// Função para calcular peso entre pixels
double diffPixel(const Vec3b& a, const Vec3b& b) {
    double dx = a[0] - b[0];
    double dy = a[1] - b[1];
    double dz = a[2] - b[2];
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// transformacao da imagem em grafo
int main() {
    Mat img = imread("entrada.png");

    if (img.empty()) {
        cout << "Erro ao carregar imagem!\n";
        return 0;
    }

    int h = img.rows;
    int w = img.cols;
    int N = h * w;

    vector<Aresta> arestas;
    arestas.reserve(2 * N);

    // Criar grafo DIRECIONADO: pixel -> vizinhos
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            int id = y * w + x;

            // Direita
            if (x + 1 < w) {
                double peso = diffPixel(img.at<Vec3b>(y,x),
                                        img.at<Vec3b>(y, x+1));
                arestas.push_back({id, id+1, peso});
            }

            // Baixo
            if (y + 1 < h) {
                double peso = diffPixel(img.at<Vec3b>(y,x),
                                        img.at<Vec3b>(y+1, x));
                arestas.push_back({id, id+w, peso});
            }
        }
    }

    cout << "Imagem convertida para grafo DIRECIONADO.\n";
    cout << "Vertices: " << N << "\n";
    cout << "Arestas: " << arestas.size() << "\n";

    // testando a arborescência:
    double custo = edmonds(0, N, arestas);
    cout << "Custo da arborescencia: " << custo << endl;

    return 0;
}

// IMPLEMENTAÇÃO DO ALGORITMO DE EDMONDS
double edmonds(int raiz, int n, vector<Aresta> arestas) {
    double result = 0;
    
    while (true) {
        vector<double> in(n, INF);
        vector<int> pre(n, -1);

        // Step 1: Achar aresta minima de entrada para cada vertice
        for (auto &e : arestas) {
            if (e.u != e.v && e.w < in[e.v]) {
                in[e.v] = e.w;
                pre[e.v] = e.u;
            }
        }

        // Raiz nao tem custo
        in[raiz] = 0;

        // Checar se existem vertices que a raiz nao atinge        
        for (int i = 0; i < n; i++) {
            if (in[i] == INF)
                return INF; // Nao existe arborecencia
        }
        
        // Step 2: Detectar ciclos
        int contagem_ciclos = 0;
        vector<int> id(n, -1);
        vector<int> vis(n, -1);

        for (int i = 0; i < n; i++) {
            result += in[i];
            int v = i;

            // Seguir predecessores para achar os ciclos
            while (vis[v] != i && id[v] == -1 && v != raiz) {
                vis[v] = i;
                v = pre[v];
            }
            if (v != raiz && id[v] == -1) {
                // Achou um ciclo
                for (int u = pre[v]; u != v; u = pre[u])
                    id[u] = contagem_ciclos;
                id[v] = contagem_ciclos++;
            }
        }

        if (contagem_ciclos == 0)
            break; // nenhum ciclo = acabou

        // Colocar novos ids para vertices que nao estão em ciclos
        for (int i = 0; i < n; i++) {
            if (id[i] == -1)
                id[i] = contagem_ciclos++;
        }

        // Step 3: Construir um novo grafo contraido
        vector<Aresta> novas;
        
        for (auto &e : arestas) {
            int u = id[e.u];
            int v = id[e.v];
            double w = e.w;

            if (u != v)
                w -= in[e.v]; // reduzir peso

            novas.push_back({u, v, w});
        }

        raiz = id[raiz];
        n = contagem_ciclos;
        arestas = novas;
    }

    return result;
}
