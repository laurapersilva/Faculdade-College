"""
Módulo para a implementação dos algoritmos de busca.
Implementa: Busca em Largura (BFS), Busca Gulosa e A* (com três heurísticas).
"""

from typing import List, Dict, Tuple, Callable, Optional
from collections import deque
import heapq
import time
from maze_solver import Maze


class SearchResult:
    """Armazena os resultados de uma busca."""
    
    def __init__(self):
        self.path = []  # Caminho da solução (lista de posições)
        self.nodes_visited = 0  # Número de nós visitados
        self.execution_time = 0.0  # Tempo de execução em segundos
        self.solution_depth = 0  # Profundidade da solução (número de passos)
        self.found = False  # Se a solução foi encontrada
        self.visited_nodes = set() # Conjunto de todos os nós visitados


class SearchAlgorithms:
    """Implementa os algoritmos de busca para o Labirinto."""
    
    def __init__(self, maze: Maze):
        """
        Inicializa os algoritmos de busca.
        
        Args:
            maze: Instância do labirinto.
        """
        self.maze = maze
    
    def bfs(self) -> SearchResult:
        """
        Busca em Largura (BFS - Breadth-First Search).
        Explora todos os nós em profundidade k antes de explorar nós em profundidade k+1.
        Garante encontrar a solução mais curta.
        
        Returns:
            SearchResult contendo o caminho, métricas e status da busca.
        """
        result = SearchResult()
        start_time = time.perf_counter()
        
        queue = deque([(self.maze.start, [self.maze.start])])
        visited = {self.maze.start}
        result.nodes_visited = 1
        
        while queue:
            current_pos, path = queue.popleft()
            
            if self.maze.is_goal(current_pos):
                result.path = path
                result.solution_depth = len(path) - 1
                result.found = True
                result.execution_time = time.perf_counter() - start_time
                result.visited_nodes = visited
                return result
            
            for neighbor in self.maze.get_neighbors(current_pos):
                if neighbor not in visited:
                    visited.add(neighbor)
                    queue.append((neighbor, path + [neighbor]))
                    result.nodes_visited += 1
        
        result.execution_time = time.perf_counter() - start_time
        result.visited_nodes = visited
        return result
    
    def greedy_search(self, heuristic: Callable) -> SearchResult:
        """
        Busca Gulosa (Greedy Search).
        Expande sempre o nó com menor valor de heurística h(n).
        Não garante a solução ótima, mas é rápida.
        
        Args:
            heuristic: Função heurística a ser usada.
        
        Returns:
            SearchResult contendo o caminho, métricas e status da busca.
        """
        result = SearchResult()
        start_time = time.perf_counter()
        
        h_value = heuristic(self.maze.start)
        
        # Heap: (h(n), counter, position, path)
        # counter é usado para desempate (ordem de inserção)
        heap = [(h_value, 0, self.maze.start, [self.maze.start])]
        visited = {self.maze.start}
        result.nodes_visited = 1
        counter = 1
        
        while heap:
            _, _, current_pos, path = heapq.heappop(heap)
            
            if self.maze.is_goal(current_pos):
                result.path = path
                result.solution_depth = len(path) - 1
                result.found = True
                result.execution_time = time.perf_counter() - start_time
                result.visited_nodes = visited
                return result
            
            for neighbor in self.maze.get_neighbors(current_pos):
                if neighbor not in visited:
                    visited.add(neighbor)
                    h_value = heuristic(neighbor)
                    heapq.heappush(heap, (h_value, counter, neighbor, path + [neighbor]))
                    result.nodes_visited += 1
                    counter += 1
        
        result.execution_time = time.perf_counter() - start_time
        result.visited_nodes = visited
        return result
    
    def a_star(self, heuristic: Callable) -> SearchResult:
        """
        Algoritmo A* (A-Star).
        Expande o nó com menor valor de f(n) = g(n) + h(n),
        onde g(n) é o custo do caminho e h(n) é a heurística.
        Garante encontrar a solução ótima se a heurística for admissível.
        
        Args:
            heuristic: Função heurística a ser usada.
        
        Returns:
            SearchResult contendo o caminho, métricas e status da busca.
        """
        result = SearchResult()
        start_time = time.perf_counter()
        
        g_value = 0
        h_value = heuristic(self.maze.start)
        f_value = g_value + h_value
        
        # Heap: (f(n), counter, g(n), position, path)
        # counter é usado para desempate (ordem de inserção)
        heap = [(f_value, 0, g_value, self.maze.start, [self.maze.start])]
        visited = {self.maze.start}
        result.nodes_visited = 1
        counter = 1
        
        while heap:
            _, _, g_current, current_pos, path = heapq.heappop(heap)
            
            if self.maze.is_goal(current_pos):
                result.path = path
                result.solution_depth = len(path) - 1
                result.found = True
                result.execution_time = time.perf_counter() - start_time
                result.visited_nodes = visited
                return result
            
            for neighbor in self.maze.get_neighbors(current_pos):
                if neighbor not in visited:
                    visited.add(neighbor)
                    g_neighbor = g_current + 1
                    h_neighbor = heuristic(neighbor)
                    f_neighbor = g_neighbor + h_neighbor
                    heapq.heappush(heap, (f_neighbor, counter, g_neighbor, neighbor, path + [neighbor]))
                    result.nodes_visited += 1
                    counter += 1
        
        result.execution_time = time.perf_counter() - start_time
        result.visited_nodes = visited
        return result
