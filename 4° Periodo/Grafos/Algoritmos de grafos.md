# Algoritmos de TGC \- 02/2025

## Algoritmos em pseudocódigo ou em explicação 

---

# Até a primeira prova

## BFS

### Pseudocódigo

busca\_BFS(G, s) é  
    seja Q uma fila  
    rotule s como explorado  
    Q.enfileirar(s)  
    enquanto Q não estiver vazio faça  
        u \= Q.desenfileirar()  
        para todos os vizinhos v de u faça  
            se v não for rotulado como explorado, então  
                rotule v como explorado  
                Q.enfileirar(v)

### Explicação

Quando queremos uma busca em largura, para cada vértice não explorado rodamos o algoritmo e vamos os marcando como explorados ao correr do código. O iniciamos em um vértice qualquer e, após marcá-lo como visitado, adicionamos na fila. Então, entramos em um loop que continuará até a lista se esvaziar por completo, no qual passamos por todos os vizinhos desse vértice inicial, checamos se já foram visitados, os marcamos e os enfileirando. É importante notar que no inicio desse loop há a retirada inicial do vértice iterado, para manter a ordem da fila. 

---

## DFS

### Pseudocódigo

DFS\_com\_Pilha(G, s):  
    criar um conjunto visitados \= vazio  
    criar uma pilha P  
    P.empilhar(s)

    enquanto P não estiver vazia faça:  
        v \= P.desempilhar

        se v não estiver em visitados então:  
            marcar v como visitado  
            processar(v)   // aqui pode imprimir ou guardar v

            para cada vizinho u de v faça:  
                se u não estiver em visitados então:  
                    marcar u como visitado   
                    u \= P.empilhar(u)  
                       
---

## DFS com 3 marcações

DFS(G) é  
    para cada vértice u em G faça  
        u.cor \= branco  
    para cada vértice u em G faça  
        se u.cor \= branco então  
            DFS\_Visita(G, u)

DFS\_Visita(G, u) é  
    u.cor \= vermelho     // descoberto  
    para cada vizinho v de u faça  
        se v.cor \= branco então  
            DFS\_Visita(G, v)  
   vu.cor \= preto        // totalmente explorado

---

## Maior caminho

Maior\_caminho(grafo G):

   ordem \= ordenacaoTopologica(G)

   para cada vértice v em V:  
      dist\[v\] \= 0  
     
   para cada vértice u na ordem:  
      para cada vizinho v de u:  
         se dist\[v\] \< dist\[u\] \+ 1:  
            dist\[v\] \= dist\[u\] \+ 1  
     
   maior \= 0  
   para cada vértice v em V:  
      maior \= max(maior, dist\[v\])

   return maior  
---

## Identificação de componentes fortemente conexos (kosaraju)

1. Busca em profundidade marcando tempos  
2. Transpor o grafo  
3. Busca em profundidade novamente na ordem inversa dos tempos

---

## Identificação de bases

Se o grafo for cíclico  
    Busca em profundidade marcando tempos  
    Transpor o grafo  
    Busca em profundidade novamente na ordem inversa dos tempos  
    Cada DFS é um componente fortemente conexo que vai virar um supervertice, criando um novo grafo  
         Nesse novo grafo basta conta os graus de entrada(se for 0 \= base) e saida(se for 0 \= anti-base)  
Se nao  
     Basta conta os graus de entrada(se for 0 \= base) e saida(se for 0 \= anti-base)  
     Contando que o grafo é uma matriz linha \= saida e coluna \= entrada

## ---

Excentricidade 

excentricidade(G,u):  
   d=0  
   para cada v em V:  
      d \= max(distancia(G, u, v), d)

distancia(G, u, v):  
   para todo w em V:  
      d\[w\] \= ∞  
   d\[u\] \= 0  
   criar conjunto S  //vértices visitados   
   criar conjunto F  //controle de visita  
   f \= inserir(u)  
     
   enquanto F não estiver vazia:  
      w \= F.retirar()  
      S \= S U {u}  
      se  w\!= v:  
         para cada vizinho w\* de w e nao pertencente a S:  
            F.inserir(w\*)  
            d\[w\*\] \= min(d\[w\*\], d\[u\] \+1)  
      senao:  
         F \= esvaziar completamente 

---

## Se um grafo é acíclico

Existem duas maneiras: o com cor e usando CFC  
Vou fazer de CFC:

1. Busca em profundidade marcando tempos  
2. Transpor o grafo  
3. Busca em profundidade novamente na ordem inversa dos tempos  
4. Se o número de CFC \= |V| é um grafo acíclico  
5. Podemos também entrar em cada CFC e ver se tem mais de 1 vértice ou mais de 2 (grafo simples)

---

# Até a segunda prova

## Menor caminho ponderado (DIJKSTRA) \*ou maior se mudar um pouco

### Pseudocódigo 

Dijkstra(G, u) é  
    para cada vértice v em V faça  
        dist\[v\] \= ∞  
    dist\[u\] \= 0  
   S \<- V  
    enquanto S não estiver vazia faça  
        u \= extrair\_mínimo{S}   // vértice com menor dist\[u\]  
          
        para cada vizinho v de u com peso w(u, v) E pertencente a S faça  
            se dist\[v\] \> dist\[u\] \+ w(u, v) então  
                dist\[v\] \= dist\[u\] \+ w(u, v)  
                atualizar\_prioridade(Q, v, dist\[v\])  //atualiza o menor caminho no dicionario aresta/peso

### Explicação 

1. Marcar todos os vértices como não visitados  
2. Escolher o vértice inicial  
3. Escolher o vértice não visitado com a menor distância atual  
4. Atualizar as distâncias dos vizinhos (se dist\[v\] \> dist\[u\] \+ w(u, v) então dist\[v\] \= dist\[u\] \+ w(u, v))  
5. Marcar o vértice atual como visitado  
6. Repetir os passos 3–5 até todos os vértices forem visitados ou até chegar ao destino desejado.

---

## Caminho ou ciclo euleriano \-  Fleury

### Explicação

1. Iniciar o processo por 1 dos 2 vértices de grau ímpar (se for caminho, se for ciclo pode começar em qualquer vértice)  
2. Escolher a aresta que não for ponte (a não ser que seja a única opção) e seguir para o vértice vizinho  
3. Apagar a aresta seguida   
4. Repetir os passos 2 e 3 até o grafo não possuir mais nenhuma aresta

---

## Prim

### Pseudocódigo

Prim(G, origem):  
    T \= {}                       \# vértices já na árvore  
    A \= {}                       \# arestas da árvore  
    escolher um vértice origem  
    adicionar origem a T

    enquanto |T| \< número de vértices do grafo:  
        encontrar a aresta (u, v) de menor peso tal que:  
            u ∈ T e v ∉ T  
        adicionar v a T  
        adicionar aresta (u, v) a A

    retornar A   \# conjunto de arestas da árvore mínima

### Explicação

1. Escolher um vértice inicial (qualquer vértice, será o início da árvore)  
2. Marcar o vértice como visitado  
    Ele passa a fazer parte do conjunto de vértices da árvore (conjunto T).  
3. Escolher a aresta de menor peso  
    Dentre todas as arestas que ligam um vértice de T a um vértice fora de T, escolha a de menor peso.  
4. Adicionar o vértice e a aresta à árvore  
5. Voltar ao passo 3 até todos os vértices estarem conectados.  
   

---

## Kruskal

1. Listar todas as arestas com seus pesos (Ex: A–B \= 4, B–C \= 2, etc.)  
2. Ordenar as arestas em ordem crescente de peso.  
3. Criar uma árvore vazia (sem arestas).  
4. Percorrer a lista de arestas ordenadas:  
* Se a aresta não formar ciclo com as arestas já escolhidas, adicione-a à árvore.  
* Se formar ciclo, ignore-a.  
5. Repetir até a árvore ter (V−1) arestas, onde V é o número de vértices do grafo.

---

## Remoção reversa

1) Eliminar a aresta de maior peso  
2) Se o grafo ainda estiver conectado, voltar ao passo 1, senão, não removeremos a aresta, pois ela é uma ponte e seguimos  
3) Repetir os passos 1 e 2 até o número de arestas ser igual ao número de vértices \- 1\.

---

## Bellman-Ford

### Pseudocódigo

para cada vértice v em V faça  
    dist\[0,v\] \= ∞  
dist\[0,u\] \= 0  
i=1

while (i \< |V| \+ 1\)  
    para todo vertice v em V  
        dist\[i,v\] \= d\[i-1, v\]

    para todo e=(k,w) em E   //analise de todas as arestas dessa iteracao  
        d\[i,w\] \= min(d\[i,w\], d\[i-1,v\] \+ P(k,w))

    i++  
---

# Até a terceira prova

## Ford-Fulkerson \- Fluxo máximo

### Explicação

1. Inicialize o fluxo como 0\.  
2. Encontre um caminho aumentante de `s` até `t`, ou seja, um caminho onde todas as arestas ainda têm capacidade disponível.  
3. Calcule a capacidade mínima do caminho (o gargalo).  
4. Aumente o fluxo desse caminho com o valor do gargalo.  
5. Atualize as capacidades residuais:  
   * Subtraia o fluxo das arestas usadas.  
   * Adicione arestas no sentido contrário (possibilitando “devolver” fluxo no futuro, se necessário).  
6. Repita até não existir mais caminho aumentante.

### Pseudocódigo

Ford-Fulkerson(G, s, t):  
    fluxo \= 0  
    enquanto existir caminho aumentante de s até t:  
        c \= capacidade mínima do caminho  
        fluxo \= fluxo \+ c  
        atualize o grafo residual  
    retorne fluxo

O que é o Grafo Residual? É um grafo que mostra quanto fluxo ainda pode ser enviado por cada aresta e quanto pode ser devolvido (ou “desfeito”) do fluxo já enviado. Ele representa o estado atual das capacidades após cada aumento de fluxo.

Cada aresta do grafo original pode ter:

* Capacidade restante → quanto ainda pode passar naquela direção.  
* Aresta reversa → permite *retirar* ou *realocar* fluxo se necessário.

Por que existe a aresta reversa? Ela é necessária porque:

* Pode acontecer de um outro caminho futuro usar parte do fluxo já enviado.  
* Então precisamos ter flexibilidade para ajustar o fluxo.  
* Isso é parte essencial da busca do fluxo máximo\!

---

## Caminho Crítico (CPM)

1. Representar o grafo como DAG;  
2. Fazer Ordenação Topológica;  
   1. Serve para organizar as atividades na ordem correta, respeitando as dependências.  
   2. Se existe A → B, A deve vir antes de B na lista.  
3. Calcular o maior tempo possível até cada vértice;  
   1. Fazemos um cálculo acumulado.  
   2. Para cada vértice, calculamos o maior tempo possível para chegar até ele.  
4. Descobrir o caminho mais longo até o último vértice.  
   1. O CAMINHO CRÍTICO é o caminho que demora mais tempo para ser concluído.

