# Implementação e Análise de Algoritmos de Busca
## Resolução de Labirinto com A*, BFS e Busca Gulosa

**Disciplina:** Inteligência Artificial  
**Professora:** Cristiane Neri Nobre  
**Data de Entrega:** 07/12  
**Valor:** 3 pontos  
**Autores:** [Adicione os nomes dos integrantes do grupo]

---

## 1. Introdução

O presente trabalho tem como objetivo implementar e analisar comparativamente três algoritmos de busca aplicados à resolução de **Labirintos**. A resolução de labirintos é um problema clássico em Inteligência Artificial que serve como benchmark para avaliar a eficiência de diferentes estratégias de busca em grafos.

### 1.1 Descrição do Problema

Um labirinto é representado por uma grade 2D onde cada célula pode ser um caminho livre, uma parede, um ponto de partida ou um ponto de chegada. O objetivo é encontrar um caminho válido (ou o caminho mais curto) do ponto de partida até o ponto de chegada, evitando as paredes.

**Representação do Labirinto:**
- **0:** Caminho livre
- **1:** Parede
- **S:** Ponto de partida (Start)
- **G:** Ponto de chegada (Goal)
- **\*:** Caminho da solução

**Exemplo de Labirinto 10x10:**
```
█ █ █ █ █ █ █ █ █ █
█ S · █ · · · · · █
█ · █ █ · █ █ █ · █
█ · · · · · · █ · █
█ █ █ · █ █ · █ · █
█ · · · █ · · · · █
█ · █ █ █ █ █ █ · █
█ · · · · · · · · █
█ █ █ █ █ █ █ █ G █
█ █ █ █ █ █ █ █ █ █
```

O espaço de estados do labirinto depende do tamanho da grade e da densidade de paredes. Para uma grade 10×10, há até 100 estados possíveis, tornando-o um problema gerenciável para demonstrar as diferenças entre algoritmos de busca.

### 1.2 Objetivo do Trabalho

Este trabalho visa:

1. Implementar o algoritmo **A*** com três heurísticas diferentes
2. Implementar dois algoritmos adicionais de busca (BFS e Busca Gulosa)
3. Comparar o desempenho dos três algoritmos em termos de tempo de execução, número de nós visitados e profundidade da solução
4. Analisar o impacto das diferentes heurísticas no desempenho do A*
5. Fornecer uma interface executável para resolução interativa de labirintos

---

## 2. Métodos de Busca Implementados

### 2.1 Busca em Largura (BFS - Breadth-First Search)

A **Busca em Largura** é um algoritmo de busca não informada que explora sistematicamente todos os nós em profundidade k antes de explorar nós em profundidade k+1. Utiliza uma fila (queue) para manter os nós a serem explorados.

**Características:**
- **Tipo:** Busca não informada
- **Completude:** Sim (garante encontrar solução se existir)
- **Otimalidade:** Sim (encontra a solução com menor número de passos)
- **Complexidade de Tempo:** O(b^d), onde b é o fator de ramificação e d é a profundidade
- **Complexidade de Espaço:** O(b^d)

**Pseudocódigo:**
```
função BFS(posição_inicial):
    fila ← [posição_inicial]
    visitados ← {posição_inicial}
    
    enquanto fila não estiver vazia:
        posição_atual ← fila.remover_primeiro()
        
        se posição_atual é objetivo:
            retorna caminho
        
        para cada vizinho de posição_atual:
            se vizinho não em visitados:
                visitados.adicionar(vizinho)
                fila.adicionar(vizinho)
    
    retorna nenhuma_solução
```

**Vantagens:**
- Garante encontrar a solução mais curta
- Simples de implementar e entender

**Desvantagens:**
- Consome muita memória (armazena todos os nós em profundidade k)
- Não utiliza informação heurística, tornando-a ineficiente para labirintos grandes

### 2.2 Busca Gulosa (Greedy Search)

A **Busca Gulosa** é um algoritmo de busca informada que expande sempre o nó com o menor valor de heurística h(n). Utiliza uma fila de prioridade ordenada pelo valor heurístico.

**Características:**
- **Tipo:** Busca informada
- **Completude:** Não (pode ficar presa em loops)
- **Otimalidade:** Não (não garante a solução ótima)
- **Complexidade de Tempo:** O(b^m), onde m é a profundidade máxima
- **Complexidade de Espaço:** O(b^m)

**Pseudocódigo:**
```
função BuscaGulosa(posição_inicial, heurística):
    fila_prioridade ← [(h(posição_inicial), posição_inicial)]
    visitados ← {posição_inicial}
    
    enquanto fila_prioridade não estiver vazia:
        posição_atual ← fila_prioridade.remover_menor()
        
        se posição_atual é objetivo:
            retorna caminho
        
        para cada vizinho de posição_atual:
            se vizinho não em visitados:
                visitados.adicionar(vizinho)
                prioridade ← h(vizinho)
                fila_prioridade.adicionar((prioridade, vizinho))
    
    retorna nenhuma_solução
```

**Vantagens:**
- Usa informação heurística para guiar a busca
- Geralmente mais rápida que BFS em termos de tempo

**Desvantagens:**
- Não garante a solução ótima
- Pode explorar caminhos desnecessários se a heurística for enganosa

### 2.3 Algoritmo A* (A-Star)

O **A*** é um algoritmo de busca informada que combina o custo real do caminho g(n) com a heurística h(n), expandindo sempre o nó com menor valor de f(n) = g(n) + h(n).

**Características:**
- **Tipo:** Busca informada
- **Completude:** Sim (garante encontrar solução se existir)
- **Otimalidade:** Sim (se a heurística for admissível)
- **Complexidade de Tempo:** Depende da heurística
- **Complexidade de Espaço:** O(b^d)

**Pseudocódigo:**
```
função A*(posição_inicial, heurística):
    fila_prioridade ← [(g(posição_inicial) + h(posição_inicial), posição_inicial)]
    visitados ← {posição_inicial}
    g_valores ← {posição_inicial: 0}
    
    enquanto fila_prioridade não estiver vazia:
        f_atual, posição_atual ← fila_prioridade.remover_menor()
        
        se posição_atual é objetivo:
            retorna caminho
        
        para cada vizinho de posição_atual:
            se vizinho não em visitados:
                g_novo ← g_valores[posição_atual] + 1
                
                se vizinho não em g_valores ou g_novo < g_valores[vizinho]:
                    g_valores[vizinho] ← g_novo
                    f_novo ← g_novo + h(vizinho)
                    visitados.adicionar(vizinho)
                    fila_prioridade.adicionar((f_novo, vizinho))
    
    retorna nenhuma_solução
```

**Vantagens:**
- Garante a solução ótima com heurística admissível
- Combina o melhor de BFS (completude e otimalidade) com busca gulosa (eficiência)
- Mais eficiente que BFS em muitos casos

**Desvantagens:**
- Requer uma boa heurística para ser eficiente
- Consome mais memória que Busca Gulosa

---

## 3. Heurísticas Implementadas para o A*

Uma heurística é uma função que estima o custo de um estado até o objetivo. A qualidade da heurística afeta significativamente o desempenho do A*.

### 3.1 H1: Distância Euclidiana

A heurística **H1** calcula a distância em linha reta (distância euclidiana) do ponto atual ao objetivo.

**Fórmula:**
$$h_1(n) = \sqrt{(x_n - x_{objetivo})^2 + (y_n - y_{objetivo})^2}$$

**Exemplo:**
```
Posição Atual: (3, 5)
Objetivo: (8, 8)

h₁ = √((3-8)² + (5-8)²) = √(25 + 9) = √34 ≈ 5.83
```

**Propriedades:**
- **Admissibilidade:** Sim (nunca superestima o custo real em labirintos com movimentos em 4 direções)
- **Consistência:** Não
- **Qualidade:** Média (fornece estimativa razoável do custo real)

**Vantagens:**
- Simples de calcular
- Sempre admissível para movimentos em 4 direções

**Desvantagens:**
- Subestima o custo real em labirintos com paredes
- Menos informativa que heurísticas que consideram a geometria do labirinto

### 3.2 H2: Distância de Manhattan

A heurística **H2** calcula a soma das distâncias verticais e horizontais (distância de Manhattan) do ponto atual ao objetivo.

**Fórmula:**
$$h_2(n) = |x_n - x_{objetivo}| + |y_n - y_{objetivo}|$$

**Exemplo:**
```
Posição Atual: (3, 5)
Objetivo: (8, 8)

h₂ = |3-8| + |5-8| = 5 + 3 = 8
```

**Propriedades:**
- **Admissibilidade:** Sim
- **Consistência:** Sim
- **Qualidade:** Alta (fornece boa estimativa do custo real para movimentos em 4 direções)

**Vantagens:**
- Mais informativa que H1 para movimentos em 4 direções
- Consistente (melhora a eficiência do A*)
- Rápida de calcular

**Desvantagens:**
- Subestima o custo real em labirintos com muitas paredes
- Não é perfeita, pois ignora a presença de paredes

### 3.3 H3: Distância de Chebyshev (Máximo)

A heurística **H3** calcula o máximo entre a distância vertical e a distância horizontal do ponto atual ao objetivo.

**Fórmula:**
$$h_3(n) = \max(|x_n - x_{objetivo}|, |y_n - y_{objetivo}|)$$

**Exemplo:**
```
Posição Atual: (3, 5)
Objetivo: (8, 8)

h₃ = max(|3-8|, |5-8|) = max(5, 3) = 5
```

**Propriedades:**
- **Admissibilidade:** Sim (para movimentos em 4 direções)
- **Consistência:** Sim
- **Qualidade:** Baixa (subestima mais que H2)

**Vantagens:**
- Muito rápida de calcular (apenas comparações)
- Consistente

**Desvantagens:**
- Menos informativa que H2
- Subestima significativamente o custo real

### 3.4 Comparação das Heurísticas

| Heurística | Admissibilidade | Consistência | Qualidade | Custo Computacional |
| :--- | :---: | :---: | :---: | :---: |
| **H1 (Euclidiana)** | Sim | Não | Média | Baixo |
| **H2 (Manhattan)** | Sim | Sim | Alta | Muito Baixo |
| **H3 (Chebyshev)** | Sim | Sim | Baixa | Muito Baixo |

---

## 4. Resultados Experimentais

### 4.1 Configuração dos Testes

**Ambiente de Execução:**
- Processador: [Especificar]
- Memória RAM: [Especificar]
- Sistema Operacional: [Especificar]
- Linguagem de Programação: Python 3.x

**Labirintos de Teste:**
- [Descrever os labirintos utilizados nos testes: tamanho, densidade de paredes, etc.]
- Número de testes realizados: [Especificar]

### 4.2 Resultados por Algoritmo

#### 4.2.1 Busca em Largura (BFS)

| Labirinto | Profundidade | Nós Visitados | Tempo (ms) |
| :--- | ---: | ---: | ---: |
| [Labirinto 1] | [valor] | [valor] | [valor] |
| [Labirinto 2] | [valor] | [valor] | [valor] |
| **Média** | **[valor]** | **[valor]** | **[valor]** |

#### 4.2.2 Busca Gulosa (com Heurística de Manhattan)

| Labirinto | Profundidade | Nós Visitados | Tempo (ms) |
| :--- | ---: | ---: | ---: |
| [Labirinto 1] | [valor] | [valor] | [valor] |
| [Labirinto 2] | [valor] | [valor] | [valor] |
| **Média** | **[valor]** | **[valor]** | **[valor]** |

#### 4.2.3 A* com H1 (Distância Euclidiana)

| Labirinto | Profundidade | Nós Visitados | Tempo (ms) |
| :--- | ---: | ---: | ---: |
| [Labirinto 1] | [valor] | [valor] | [valor] |
| [Labirinto 2] | [valor] | [valor] | [valor] |
| **Média** | **[valor]** | **[valor]** | **[valor]** |

#### 4.2.4 A* com H2 (Distância de Manhattan)

| Labirinto | Profundidade | Nós Visitados | Tempo (ms) |
| :--- | ---: | ---: | ---: |
| [Labirinto 1] | [valor] | [valor] | [valor] |
| [Labirinto 2] | [valor] | [valor] | [valor] |
| **Média** | **[valor]** | **[valor]** | **[valor]** |

#### 4.2.5 A* com H3 (Distância de Chebyshev)

| Labirinto | Profundidade | Nós Visitados | Tempo (ms) |
| :--- | ---: | ---: | ---: |
| [Labirinto 1] | [valor] | [valor] | [valor] |
| [Labirinto 2] | [valor] | [valor] | [valor] |
| **Média** | **[valor]** | **[valor]** | **[valor]** |

### 4.3 Tabela Comparativa Geral

| Algoritmo | Profundidade Média | Nós Visitados Médios | Tempo Médio (ms) | Ótimo |
| :--- | ---: | ---: | ---: | :---: |
| **BFS** | [valor] | [valor] | [valor] | Sim |
| **Busca Gulosa** | [valor] | [valor] | [valor] | Não |
| **A* (H1)** | [valor] | [valor] | [valor] | Sim |
| **A* (H2)** | [valor] | [valor] | [valor] | Sim |
| **A* (H3)** | [valor] | [valor] | [valor] | Sim |

---

## 5. Análise Comparativa

### 5.1 Comparação entre os Três Algoritmos (BFS, Busca Gulosa e A*)

#### 5.1.1 Qual Algoritmo foi Mais Rápido?

[Descrever qual algoritmo apresentou o menor tempo de execução e por quê. Incluir análise de dados.]

#### 5.1.2 Qual Visitou Menos Nós?

[Descrever qual algoritmo visitou o menor número de nós e analisar as implicações em termos de eficiência de memória.]

#### 5.1.3 Diferenças no Caminho Encontrado

[Analisar se os algoritmos encontraram caminhos diferentes. Discutir por que BFS garante a solução mais curta enquanto outros podem não.]

#### 5.1.4 Análise de Profundidade

[Comparar as profundidades das soluções encontradas por cada algoritmo.]

### 5.2 Comparação das Três Heurísticas do A*

#### 5.2.1 Desempenho de H1 (Distância Euclidiana)

[Descrever o desempenho de H1, incluindo número de nós visitados e tempo de execução. Explicar por que H1 pode ser menos eficiente.]

#### 5.2.2 Desempenho de H2 (Distância de Manhattan)

[Descrever o desempenho de H2. Explicar por que é geralmente a melhor heurística para labirintos com movimentos em 4 direções.]

#### 5.2.3 Desempenho de H3 (Distância de Chebyshev)

[Descrever o desempenho de H3. Comparar com H2 e discutir o trade-off entre qualidade da heurística e custo computacional.]

#### 5.2.4 Impacto das Heurísticas no Desempenho do A*

[Analisar como a qualidade da heurística (admissibilidade e consistência) afeta o desempenho do A*. Incluir gráficos ou tabelas comparativas.]

### 5.3 Discussão: Qual Abordagem teve Melhor Desempenho?

[Sintetizar os achados e indicar qual algoritmo/heurística foi mais eficiente. Discutir as razões técnicas por trás do desempenho observado.]

---

## 6. Conclusão

[Resumir os principais achados do trabalho. Discutir as implicações práticas dos resultados. Mencionar possíveis extensões ou melhorias futuras, como a implementação de heurísticas mais sofisticadas que considerem a presença de paredes.]

---

## 7. Referências

[Adicionar referências bibliográficas relevantes sobre algoritmos de busca, A*, heurísticas, resolução de labirintos, etc.]

---

## Anexo A: Código-Fonte Completo

### A.1 maze_solver.py

[Inserir código completo do arquivo maze_solver.py]

### A.2 search_algorithms.py

[Inserir código completo do arquivo search_algorithms.py]

### A.3 main.py

[Inserir código completo do arquivo main.py]

---

**Documento preparado por:** Manus AI  
**Data de Preparação:** [Data Atual]
