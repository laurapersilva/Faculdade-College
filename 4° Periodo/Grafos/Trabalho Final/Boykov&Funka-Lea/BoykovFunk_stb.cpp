// BoykovFunk_stb.cpp
// Compilar:
//   g++ -O2 -std=c++17 BoykovFunk_stb.cpp -o boykov.exe
//
// Uso:
//   ./boykov.exe image.png [lambda] [sigma]

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>
#include <string>
#include <cstdlib>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

struct Pixel {
    unsigned char r, g, b;
};

inline double sqr(double x) { return x * x; }

// ---------------- Dinic max-flow ----------------

struct Edge {
    int to;
    int rev;
    double cap;
};

struct Dinic {
    int N;
    vector<vector<Edge>> G;
    vector<int> level;
    vector<int> it;

    Dinic(int n) : N(n), G(n), level(n), it(n) {}

    void addEdge(int from, int to, double cap) {
        Edge fwd{to, (int)G[to].size(), cap};
        Edge rev{from, (int)G[from].size(), 0.0};
        G[from].push_back(fwd);
        G[to].push_back(rev);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (const auto &e : G[v]) {
                if (e.cap > 1e-9 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    double dfs(int v, int t, double f) {
        if (v == t) return f;
        for (int &i = it[v]; i < (int)G[v].size(); ++i) {
            Edge &e = G[v][i];
            if (e.cap > 1e-9 && level[v] < level[e.to]) {
                double d = dfs(e.to, t, min(f, e.cap));
                if (d > 1e-9) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0.0;
    }

    double maxFlow(int s, int t) {
        const double INF_D = 1e18;
        double flow = 0.0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            double f;
            while ((f = dfs(s, t, INF_D)) > 1e-9) {
                flow += f;
            }
        }
        return flow;
    }
};

// ---------------- Util ----------------

double diff2Pixel(const Pixel &a, const Pixel &b) {
    double dr = double(a.r) - double(b.r);
    double dg = double(a.g) - double(b.g);
    double db = double(a.b) - double(b.b);
    return dr*dr + dg*dg + db*db;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " image.png [lambda] [sigma]\n";
        return 0;
    }

    string img_name = argv[1];
    double lambda = 50.0;
    double sigma  = 10.0;

    if (argc >= 3) lambda = atof(argv[2]);
    if (argc >= 4) sigma  = atof(argv[3]);

    int w, h, ch;
    unsigned char* img_data = stbi_load(img_name.c_str(), &w, &h, &ch, 3);
    if (!img_data) {
        cerr << "Erro ao carregar imagem: " << img_name << "\n";
        return -1;
    }

    int N = w * h;
    cout << "Imagem: " << w << "x" << h << " (" << N << " pixels)\n";
    cout << "lambda = " << lambda << ", sigma = " << sigma << "\n";

    vector<Pixel> P(N);
    for (int i = 0; i < N; ++i) {
        P[i].r = img_data[i*3 + 0];
        P[i].g = img_data[i*3 + 1];
        P[i].b = img_data[i*3 + 2];
    }

    // -------- K-means k=2 em RGB (auto "seeds") --------
    double center[2][3];

    mt19937 rng(123456);
    uniform_int_distribution<int> dist_idx(0, N - 1);

    int idx0 = dist_idx(rng);
    int idx1 = dist_idx(rng);
    while (idx1 == idx0) idx1 = dist_idx(rng);

    center[0][0] = P[idx0].r;
    center[0][1] = P[idx0].g;
    center[0][2] = P[idx0].b;

    center[1][0] = P[idx1].r;
    center[1][1] = P[idx1].g;
    center[1][2] = P[idx1].b;

    vector<int> label(N, 0);

    const int KMEANS_ITERS = 10;
    for (int it = 0; it < KMEANS_ITERS; ++it) {
        double sum[2][3] = {{0,0,0},{0,0,0}};
        int cnt[2] = {0,0};

        // atribuição
        for (int i = 0; i < N; ++i) {
            double d0 =
                sqr(double(P[i].r) - center[0][0]) +
                sqr(double(P[i].g) - center[0][1]) +
                sqr(double(P[i].b) - center[0][2]);

            double d1 =
                sqr(double(P[i].r) - center[1][0]) +
                sqr(double(P[i].g) - center[1][1]) +
                sqr(double(P[i].b) - center[1][2]);

            int c = (d0 <= d1) ? 0 : 1;
            label[i] = c;
            sum[c][0] += P[i].r;
            sum[c][1] += P[i].g;
            sum[c][2] += P[i].b;
            cnt[c]++;
        }

        // atualização
        for (int c = 0; c < 2; ++c) {
            if (cnt[c] > 0) {
                center[c][0] = sum[c][0] / cnt[c];
                center[c][1] = sum[c][1] / cnt[c];
                center[c][2] = sum[c][2] / cnt[c];
            } else {
                int idx = dist_idx(rng);
                center[c][0] = P[idx].r;
                center[c][1] = P[idx].g;
                center[c][2] = P[idx].b;
            }
        }
    }

    // cluster 0 = "foreground", cluster 1 = "background"
    double mu_fg[3] = { center[0][0], center[0][1], center[0][2] };
    double mu_bg[3] = { center[1][0], center[1][1], center[1][2] };

    double sigma_data = sigma;
    if (sigma_data < 1e-3) sigma_data = 1.0;
    double sigma_col = sigma;
    if (sigma_col < 1e-3) sigma_col = 1.0;

    double inv_two_sigma_data2 = 1.0 / (2.0 * sigma_data * sigma_data);
    double inv_two_sigma_col2  = 1.0 / (2.0 * sigma_col  * sigma_col);

    // -------- Monta grafo --------
    int source = N;
    int sink   = N + 1;
    int total_nodes = N + 2;

    auto start = std::chrono::high_resolution_clock::now();
    Dinic dinic(total_nodes);

    // t-links (data term)
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int id = y * w + x;
            Pixel p = P[id];

            double d2_fg =
                sqr(double(p.r) - mu_fg[0]) +
                sqr(double(p.g) - mu_fg[1]) +
                sqr(double(p.b) - mu_fg[2]);

            double d2_bg =
                sqr(double(p.r) - mu_bg[0]) +
                sqr(double(p.g) - mu_bg[1]) +
                sqr(double(p.b) - mu_bg[2]);

            double D_fg = d2_fg * inv_two_sigma_data2;
            double D_bg = d2_bg * inv_two_sigma_data2;

            double cap_s_i = D_bg; // custo de rotular FG
            double cap_i_t = D_fg; // custo de rotular BG

            if (cap_s_i < 0) cap_s_i = 0;
            if (cap_i_t < 0) cap_i_t = 0;

            dinic.addEdge(source, id, cap_s_i);
            dinic.addEdge(id,     sink, cap_i_t);
        }
    }

    // n-links (suavização) 4-conectada
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int id = y * w + x;
            Pixel p = P[id];

            if (x + 1 < w) {
                int id2 = y * w + (x + 1);
                Pixel q = P[id2];
                double d2 = diff2Pixel(p, q);
                double w_ij = lambda * exp(-d2 * inv_two_sigma_col2);
                dinic.addEdge(id,  id2, w_ij);
                dinic.addEdge(id2, id,  w_ij);
            }
            if (y + 1 < h) {
                int id2 = (y + 1) * w + x;
                Pixel q = P[id2];
                double d2 = diff2Pixel(p, q);
                double w_ij = lambda * exp(-d2 * inv_two_sigma_col2);
                dinic.addEdge(id,  id2, w_ij);
                dinic.addEdge(id2, id,  w_ij);
            }
        }
    }

    cout << "Rodando max-flow/min-cut...\n";
    double flow = dinic.maxFlow(source, sink);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    cout << "Tempo de execucao: " << duration.count() << " ms" << endl;
    cout << "Flow final: " << flow << "\n";

    // -------- Recupera partição (pixels alcançáveis de s no residual) --------
    vector<char> vis(total_nodes, 0);
    queue<int> q;
    vis[source] = 1;
    q.push(source);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (const auto &e : dinic.G[v]) {
            if (e.cap > 1e-9 && !vis[e.to]) {
                vis[e.to] = 1;
                q.push(e.to);
            }
        }
    }

    // Imagem de saída: FG verde, BG vermelho
    unsigned char* out_data = new unsigned char[N * 3];
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int id = y * w + x;
            bool isFG = (vis[id] != 0);

            unsigned char r, g, b;
            if (isFG) {
                r = 0; g = 255; b = 0;
            } else {
                r = 255; g = 0; b = 0;
            }
            out_data[id*3 + 0] = r;
            out_data[id*3 + 1] = g;
            out_data[id*3 + 2] = b;
        }
    }

    string out_name = "saida_graphcut_auto_stb.png";
    int ok = stbi_write_png(out_name.c_str(), w, h, 3, out_data, w * 3);
    if (!ok) {
        cerr << "Erro ao salvar imagem: " << out_name << "\n";
    } else {
        cout << "Segmentacao salva em: " << out_name << "\n";
    }

    stbi_image_free(img_data);
    delete[] out_data;

    return 0;
}
