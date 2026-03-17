"""
Módulo para a implementação do Labirinto (Maze Solver).
Define a estrutura do labirinto, movimentos possíveis e verificação de objetivo.
"""

from typing import List, Tuple, Set
import random
import copy


class Maze:
    """
    Representa um labirinto em uma grade 2D.
    
    0 = caminho livre
    1 = parede
    2 = ponto de partida (start)
    3 = ponto de chegada (goal)
    """
    
    def __init__(self, grid: List[List[int]] = None, start: Tuple[int, int] = None, goal: Tuple[int, int] = None):
        """
        Inicializa o labirinto com uma grade.
        
        Args:
            grid: Grade 2D representando o labirinto.
            start: Tupla (linha, coluna) do ponto de partida.
            goal: Tupla (linha, coluna) do ponto de chegada.
        """
        if grid is None:
            # Cria um labirinto padrão 10x10
            self.grid = self._create_default_maze()
            self.start = (1, 1)
            self.goal = (8, 8)
        else:
            self.grid = grid
            self.start = start if start is not None else (1, 1)
            self.goal = goal if goal is not None else (len(grid) - 2, len(grid[0]) - 2)
        
        self.rows = len(self.grid)
        self.cols = len(self.grid[0])
    
    def _create_default_maze(self) -> List[List[int]]:
        """Cria um labirinto padrão 10x10."""
        maze = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 1, 0, 0, 0, 0, 0, 1],
            [1, 0, 1, 1, 0, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 1, 0, 1, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 1, 0, 0, 0, 0, 1],
            [1, 0, 1, 1, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
        ]
        return maze
    
    def is_valid_position(self, row: int, col: int) -> bool:
        """Verifica se uma posição é válida (dentro dos limites e não é parede)."""
        return (0 <= row < self.rows and 
                0 <= col < self.cols and 
                self.grid[row][col] != 1)
    
    def get_neighbors(self, position: Tuple[int, int]) -> List[Tuple[int, int]]:
        """
        Retorna todos os vizinhos válidos (movimentos possíveis).
        Permite movimento em 4 direções: cima, baixo, esquerda, direita.
        
        Args:
            position: Tupla (linha, coluna) da posição atual.
        
        Returns:
            Lista de posições vizinhas válidas.
        """
        row, col = position
        neighbors = []
        
        # Movimentos: cima, baixo, esquerda, direita
        moves = [(-1, 0), (1, 0), (0, -1), (0, 1)]
        
        for dr, dc in moves:
            new_row, new_col = row + dr, col + dc
            if self.is_valid_position(new_row, new_col):
                neighbors.append((new_row, new_col))
        
        return neighbors
    
    def is_goal(self, position: Tuple[int, int]) -> bool:
        """Verifica se a posição é o objetivo."""
        return position == self.goal
    
    def euclidean_distance(self, position: Tuple[int, int]) -> float:
        """
        Heurística H1: Distância Euclidiana.
        Distância em linha reta do ponto atual ao objetivo.
        
        Args:
            position: Tupla (linha, coluna) da posição atual.
        
        Returns:
            Distância euclidiana até o objetivo.
        """
        row, col = position
        goal_row, goal_col = self.goal
        return ((row - goal_row) ** 2 + (col - goal_col) ** 2) ** 0.5
    
    def manhattan_distance(self, position: Tuple[int, int]) -> int:
        """
        Heurística H2: Distância de Manhattan.
        Soma das distâncias verticais e horizontais do ponto atual ao objetivo.
        
        Args:
            position: Tupla (linha, coluna) da posição atual.
        
        Returns:
            Distância de Manhattan até o objetivo.
        """
        row, col = position
        goal_row, goal_col = self.goal
        return abs(row - goal_row) + abs(col - goal_col)
    
    def chebyshev_distance(self, position: Tuple[int, int]) -> int:
        """
        Heurística H3: Distância de Chebyshev (Máximo).
        Máximo entre a distância vertical e a distância horizontal do ponto atual ao objetivo.
        
        Args:
            position: Tupla (linha, coluna) da posição atual.
        
        Returns:
            Distância de Chebyshev até o objetivo.
        """
        row, col = position
        goal_row, goal_col = self.goal
        return max(abs(row - goal_row), abs(col - goal_col))
    
    def position_to_string(self, position: Tuple[int, int]) -> str:
        """Converte uma posição para string formatada."""
        return f"({position[0]}, {position[1]})"
    
    def print_maze(self, path: List[Tuple[int, int]] = None) -> None:
        """
        Imprime o labirinto de forma legível.
        
        Args:
            path: Caminho da solução a ser destacado (opcional).
        """
        path_set = set(path) if path else set()
        
        for row in range(self.rows):
            for col in range(self.cols):
                if (row, col) == self.start:
                    print("S", end=" ")
                elif (row, col) == self.goal:
                    print("G", end=" ")
                elif (row, col) in path_set:
                    print("*", end=" ")
                elif self.grid[row][col] == 1:
                    print("█", end=" ")
                else:
                    print("·", end=" ")
            print()
    
    def get_maze_string(self, path: List[Tuple[int, int]] = None) -> str:
        """Retorna uma representação em string do labirinto."""
        result = ""
        path_set = set(path) if path else set()
        
        for row in range(self.rows):
            for col in range(self.cols):
                if (row, col) == self.start:
                    result += "S "
                elif (row, col) == self.goal:
                    result += "G "
                elif (row, col) in path_set:
                    result += "* "
                elif self.grid[row][col] == 1:
                    result += "█ "
                else:
                    result += "· "
            result += "\n"
        
        return result
    
    @staticmethod
    def create_random_maze(rows: int = 15, cols: int = 15, density: float = 0.3) -> 'Maze':
        """
        Cria um labirinto aleatório.
        
        Args:
            rows: Número de linhas.
            cols: Número de colunas.
            density: Densidade de paredes (0.0 a 1.0).
        
        Returns:
            Uma instância de Maze com o labirinto aleatório.
        """
        # Cria as bordas como paredes
        grid = [[1 if row == 0 or row == rows - 1 or col == 0 or col == cols - 1 else 0 
                 for col in range(cols)] for row in range(rows)]
        
        # Adiciona paredes aleatórias
        for row in range(1, rows - 1):
            for col in range(1, cols - 1):
                if random.random() < density:
                    grid[row][col] = 1
        
        # Define start e goal em posições válidas
        start = (1, 1)
        goal = (rows - 2, cols - 2)
        
        # Garante que start e goal não são paredes
        grid[start[0]][start[1]] = 0
        grid[goal[0]][goal[1]] = 0
        
        return Maze(grid, start, goal)
