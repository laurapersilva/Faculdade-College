import subprocess
import cv2
import numpy as np
import matplotlib.pyplot as plt
import os
import re

# --- CONFIGURAÇÃO ---
IMG_INPUT = "image.jpg"  # Coloque o nome da sua imagem de teste aqui
EXE_FELZ = "felz_stb.exe"
EXE_BOYKOV = "boykov.exe"

# Parâmetros dos algoritmos para o teste
PARAMS_FELZ = ["1000", "2000"]       # K, min_size
PARAMS_BOYKOV = ["50", "10"]      # lambda, sigma

# --- FUNÇÕES ---

def gerar_ruidos(img_path):
    img = cv2.imread(img_path)
    if img is None:
        print(f"Erro: Imagem {img_path} não encontrada.")
        return False
    
    # Original
    cv2.imwrite("teste_original.png", img)
    
    # 1. Ruído Sal e Pimenta (S&P)
    sp = img.copy()
    prob = 0.05
    mask = np.random.rand(img.shape[0], img.shape[1])
    sp[mask < prob] = 0
    sp[mask > 1 - prob] = 255
    cv2.imwrite("teste_sp.png", sp)
    
    # 2. Ruído Gaussiano
    gauss = img.copy()
    noise = np.zeros(img.shape, dtype=np.uint8)
    cv2.randn(noise, (0,0,0), (50,50,50))
    gauss = cv2.add(img, noise)
    cv2.imwrite("teste_gauss.png", gauss)
    
    return True

def rodar_algoritmo(executavel, imagem, args):
    cmd = [f"./{executavel}", imagem] + args
    
    # No Windows, as vezes precisa chamar sem o ./
    if os.name == 'nt' and not executavel.startswith('.'):
         cmd = [executavel, imagem] + args
         
    try:
        # Roda o exe e captura o texto de saída
        resultado = subprocess.run(cmd, capture_output=True, text=True, check=True)
        saida = resultado.stdout
        
        # Procura a linha "Tempo de execucao: X ms" usando Regex
        match = re.search(r"Tempo de execucao:\s*([\d\.]+)", saida)
        if match:
            return float(match.group(1))
        else:
            return 0.0
    except Exception as e:
        print(f"Erro ao rodar {executavel}: {e}")
        return 0.0

# --- MAIN ---

if __name__ == "__main__":
    if not gerar_ruidos(IMG_INPUT):
        exit()

    cenarios = ["teste_original.png", "teste_sp.png", "teste_gauss.png"]
    labels = ["Original", "Sal & Pimenta", "Gaussiano"]
    
    tempos_felz = []
    tempos_boyk = []

    print("-" * 30)
    print("INICIANDO BENCHMARK")
    print("-" * 30)

    for img in cenarios:
        print(f"\nProcessando: {img}")
        
        # Rodar Felzenszwalb
        t_f = rodar_algoritmo(EXE_FELZ, img, PARAMS_FELZ)
        tempos_felz.append(t_f)
        print(f"  > Felzenszwalb: {t_f:.2f} ms")
        # Renomeia a saída para não sobrescrever
        if os.path.exists("saida_segmentada_stb.png"):
            os.rename("saida_segmentada_stb.png", f"out_felz_{img}")

        # Rodar Boykov
        t_b = rodar_algoritmo(EXE_BOYKOV, img, PARAMS_BOYKOV)
        tempos_boyk.append(t_b)
        print(f"  > Boykov:       {t_b:.2f} ms")
        # Renomeia a saída
        if os.path.exists("saida_graphcut_auto_stb.png"):
            os.rename("saida_graphcut_auto_stb.png", f"out_boyk_{img}")

    # --- GERAR GRÁFICO ---
    x = np.arange(len(labels))
    width = 0.35

    fig, ax = plt.subplots(figsize=(10, 6))
    rects1 = ax.bar(x - width/2, tempos_felz, width, label='Felzenszwalb', color='skyblue')
    rects2 = ax.bar(x + width/2, tempos_boyk, width, label='Boykov (GraphCut)', color='salmon')

    ax.set_ylabel('Tempo (ms)')
    ax.set_title('Comparação de Tempo de Execução')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    def autolabel(rects):
        for rect in rects:
            height = rect.get_height()
            ax.annotate(f'{height:.1f}',
                        xy=(rect.get_x() + rect.get_width() / 2, height),
                        xytext=(0, 3),
                        textcoords="offset points",
                        ha='center', va='bottom')

    autolabel(rects1)
    autolabel(rects2)

    plt.tight_layout()
    plt.savefig("resultado_benchmark.png")
    print("\nGráfico salvo como 'resultado_benchmark.png'")
    plt.show()