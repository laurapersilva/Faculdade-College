"""
Interface Gráfica (GUI) para o Solver de Labirinto.
Permite visualizar e resolver labirintos de forma interativa.
"""

import tkinter as tk
from tkinter import ttk, messagebox
import threading
from maze_solver import Maze
from search_algorithms import SearchAlgorithms
import time


class MazeGUI:
    """Interface gráfica para o solver de labirinto."""
    
    def __init__(self, root):
        """Inicializa a interface gráfica."""
        self.root = root
        self.root.title("Solver de Labirinto com Algoritmos de Busca")
        self.root.geometry("1200x800")
        
        # Variáveis de controle
        self.maze = None
        self.search_alg = None
        self.current_result = None
        self.cell_size = 30
        self.is_solving = False
        self.stop_event = threading.Event() # Evento para sinalizar a parada
        
        # Cores
        self.COLOR_WALL = "#2C3E50"
        self.COLOR_PATH = "#F39C12"
        self.COLOR_START = "#27AE60"
        self.COLOR_GOAL = "#E74C3C"
        self.COLOR_EMPTY = "#ECF0F1"
        self.COLOR_VISITED = "#3498DB"
        
        # Criar interface
        self._create_widgets()
        
        # Criar labirinto padrão
        self.maze = Maze()
        self.search_alg = SearchAlgorithms(self.maze)
        self._draw_maze()
        self.root.bind("<Configure>", self._on_resize)
    
    def _create_widgets(self):
        """Cria os widgets da interface."""
        # Frame principal
        main_frame = ttk.Frame(self.root)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Frame esquerdo (Controles)
        left_frame = ttk.Frame(main_frame)
        left_frame.pack(side=tk.LEFT, fill=tk.Y, padx=(0, 10))
        
        # Título dos controles
        title_label = ttk.Label(left_frame, text="CONTROLES", font=("Arial", 12, "bold"))
        title_label.pack(pady=(0, 10))
        
        # --- Seção: Tamanho do Labirinto ---
        size_frame = ttk.LabelFrame(left_frame, text="Tamanho do Labirinto", padding=10)
        size_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.size_var = tk.StringVar(value="10")
        
        sizes = [
            ("Pequeno (10x10)", "10"),
            ("Médio (15x15)", "15"),
            ("Grande (20x20)", "20"),
        ]
        
        for text, value in sizes:
            ttk.Radiobutton(size_frame, text=text, variable=self.size_var, value=value,
                          command=self._on_size_changed).pack(anchor=tk.W)
        
        # Customizado
        custom_frame = ttk.Frame(size_frame)
        custom_frame.pack(anchor=tk.W, pady=(5, 0))
        ttk.Radiobutton(custom_frame, text="Customizado:", variable=self.size_var, value="custom",
                       command=self._on_size_changed).pack(side=tk.LEFT)
        
        self.custom_size_entry = ttk.Entry(custom_frame, width=5)
        self.custom_size_entry.pack(side=tk.LEFT, padx=(5, 0))
        self.custom_size_entry.insert(0, "15")
        
        # --- Seção: Densidade de Paredes ---
        density_frame = ttk.LabelFrame(left_frame, text="Densidade de Paredes", padding=10)
        density_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.density_var = tk.DoubleVar(value=0.3)
        density_scale = ttk.Scale(density_frame, from_=0.0, to=1.0, variable=self.density_var,
                                 orient=tk.HORIZONTAL)
        density_scale.pack(fill=tk.X)
        
        self.density_label = ttk.Label(density_frame, text="30%")
        self.density_label.pack()
        density_scale.configure(command=self._update_density_label)
        
        # --- Seção: Botões de Ação ---
        action_frame = ttk.LabelFrame(left_frame, text="Ações", padding=10)
        action_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Button(action_frame, text="Gerar Novo Labirinto", 
                  command=self._generate_new_maze).pack(fill=tk.X, pady=(0, 5))
        
        ttk.Button(action_frame, text="Usar Padrão", 
                  command=self._use_default_maze).pack(fill=tk.X)
        
        # --- Seção: Seleção de Algoritmo ---
        algo_frame = ttk.LabelFrame(left_frame, text="Algoritmo de Busca", padding=10)
        algo_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.algo_var = tk.StringVar(value="1")
        
        algorithms = [
            ("BFS (Busca em Largura)", "1"),
            ("Greedy (Manhattan)", "2"),
            ("A* (Euclidiana)", "3"),
            ("A* (Manhattan)", "4"),
            ("A* (Chebyshev)", "5"),
        ]
        
        for text, value in algorithms:
            ttk.Radiobutton(algo_frame, text=text, variable=self.algo_var, value=value).pack(anchor=tk.W)
        
        # --- Seção: Botão Resolver ---
        solve_frame = ttk.LabelFrame(left_frame, text="Resolver", padding=10)
        solve_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.solve_button = ttk.Button(solve_frame, text="RESOLVER", command=self._solve_maze)
        self.solve_button.pack(fill=tk.X, pady=5)
        
        self.stop_button = ttk.Button(solve_frame, text="Parar", command=self._stop_solving, state=tk.DISABLED)
        self.stop_button.pack(fill=tk.X)
        
        # --- Seção: Resultados ---
        results_frame = ttk.LabelFrame(left_frame, text="Resultados", padding=10)
        results_frame.pack(fill=tk.BOTH, expand=True)
        
        self.results_text = tk.Text(results_frame, height=15, width=30, state=tk.DISABLED)
        self.results_text.pack(fill=tk.BOTH, expand=True)
        
        # Frame direito (Visualização do Labirinto)
        right_frame = ttk.Frame(main_frame)
        right_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
        # Canvas para desenhar o labirinto
        canvas_frame = ttk.LabelFrame(right_frame, text="Visualização do Labirinto", padding=5)
        canvas_frame.pack(fill=tk.BOTH, expand=True)
        
        self.canvas = tk.Canvas(canvas_frame, bg=self.COLOR_EMPTY, highlightthickness=1)
        self.canvas.pack(fill=tk.BOTH, expand=True)
    
    def _on_resize(self, event):
        """Redesenha o labirinto ao redimensionar a janela."""
        if event.widget == self.root:
            self._draw_maze()
    
    def _update_density_label(self, value):
        """Atualiza o label de densidade."""
        density = float(value)
        self.density_label.config(text=f"{int(density * 100)}%")
    
    def _on_size_changed(self):
        """Chamado quando o tamanho do labirinto é alterado."""
        pass
    
    def _generate_new_maze(self):
        """Gera um novo labirinto aleatório."""
        if self.is_solving:
            messagebox.showwarning("Aviso", "Aguarde o término da resolução atual.")
            return
        
        size_choice = self.size_var.get()
        
        if size_choice == "custom":
            try:
                size = int(self.custom_size_entry.get())
                if size < 5 or size > 50:
                    messagebox.showerror("Erro", "Tamanho deve estar entre 5 e 50.")
                    return
            except ValueError:
                messagebox.showerror("Erro", "Tamanho inválido.")
                return
        else:
            size = int(size_choice)
        
        density = self.density_var.get()
        
        self.maze = Maze.create_random_maze(size, size, density)
        self.search_alg = SearchAlgorithms(self.maze)
        self.current_result = None
        self._draw_maze()
        self._clear_results()
    
    def _use_default_maze(self):
        """Usa o labirinto padrão."""
        if self.is_solving:
            messagebox.showwarning("Aviso", "Aguarde o término da resolução atual.")
            return
        
        self.maze = Maze()
        self.search_alg = SearchAlgorithms(self.maze)
        self.current_result = None
        self._draw_maze()
        self._clear_results()
    
    def _draw_maze(self, path_to_draw=None, visited_to_draw=None):
        """Desenha o labirinto no canvas."""
        self.canvas.delete("all")
        
        if not self.maze:
            return
        
        # Obter dimensões do canvas
        self.canvas.update_idletasks()
        canvas_width = self.canvas.winfo_width()
        canvas_height = self.canvas.winfo_height()
        
        if canvas_width <= 1 or canvas_height <= 1:
            return
        
        # Calcular tamanho da célula
        cell_size_w = max(10, (canvas_width - 20) // self.maze.cols)
        cell_size_h = max(10, (canvas_height - 20) // self.maze.rows)
        self.cell_size = min(cell_size_w, cell_size_h)
        
        # Centralizar o labirinto
        total_width = self.maze.cols * self.cell_size
        total_height = self.maze.rows * self.cell_size
        offset_x = (canvas_width - total_width) // 2
        offset_y = (canvas_height - total_height) // 2
        
        # Conjuntos para visualização
        path_set = set(path_to_draw) if path_to_draw else set()
        visited_set = set(visited_to_draw) if visited_to_draw else set()
        
        if self.current_result:
            if not path_to_draw:
                path_set = set(self.current_result.path)
            if not visited_to_draw:
                visited_set = self.current_result.visited_nodes
        
        # Desenhar células
        for row in range(self.maze.rows):
            for col in range(self.maze.cols):
                x1 = offset_x + col * self.cell_size
                y1 = offset_y + row * self.cell_size
                x2 = x1 + self.cell_size
                y2 = y1 + self.cell_size
                
                pos = (row, col)
                
                # Determinar cor (prioridade: Start/Goal > Path > Visited > Wall > Empty)
                color = self.COLOR_EMPTY
                
                if self.maze.grid[row][col] == 1:
                    color = self.COLOR_WALL
                elif pos in visited_set:
                    color = self.COLOR_VISITED
                
                if pos in path_set:
                    color = self.COLOR_PATH
                
                if pos == self.maze.start:
                    color = self.COLOR_START
                elif pos == self.maze.goal:
                    color = self.COLOR_GOAL
                
                self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline="#BDC3C7")
    
    def _solve_maze(self):
        """Resolve o labirinto usando o algoritmo selecionado."""
        if self.is_solving:
            messagebox.showwarning("Aviso", "Já existe uma resolução em andamento.")
            return
        
        if not self.maze:
            messagebox.showerror("Erro", "Nenhum labirinto disponível.")
            return
        
        # Resetar o evento de parada
        self.stop_event.clear()
        
        # Executar em thread separada para não congelar a GUI
        thread = threading.Thread(target=self._solve_thread)
        thread.daemon = True
        thread.start()
    
    def _solve_thread(self):
        """Thread para resolver o labirinto."""
        self.is_solving = True
        self.solve_button.config(state=tk.DISABLED)
        self.stop_button.config(state=tk.NORMAL)
        
        try:
            algo_choice = int(self.algo_var.get())
            
            if algo_choice == 1:
                result = self.search_alg.bfs()
                algo_name = "Busca em Largura (BFS)"
            elif algo_choice == 2:
                result = self.search_alg.greedy_search(self.maze.manhattan_distance)
                algo_name = "Busca Gulosa (Manhattan)"
            elif algo_choice == 3:
                result = self.search_alg.a_star(self.maze.euclidean_distance)
                algo_name = "A* (Euclidiana)"
            elif algo_choice == 4:
                result = self.search_alg.a_star(self.maze.manhattan_distance)
                algo_name = "A* (Manhattan)"
            elif algo_choice == 5:
                result = self.search_alg.a_star(self.maze.chebyshev_distance)
                algo_name = "A* (Chebyshev)"
            else:
                return # Opção inválida
            
            self.current_result = result
            self._display_results(algo_name, result)
            
            # Visualização final (caminho e nós visitados)
            self._draw_maze()
            
            # Animação (opcional, mas legal)
            if result.found and not self.stop_event.is_set():
                path = result.path
                for i in range(len(path)):
                    if self.stop_event.is_set():
                        break
                    
                    # Desenha o caminho percorrido até o passo atual
                    self._draw_maze(path_to_draw=path[:i+1], visited_to_draw=result.visited_nodes)
                    self.root.update()
                    time.sleep(0.05) # Pequeno delay para visualização
                
                # Desenho final
                self._draw_maze()
            
        except Exception as e:
            messagebox.showerror("Erro", f"Erro ao resolver: {str(e)}")
        
        finally:
            self.is_solving = False
            self.solve_button.config(state=tk.NORMAL)
            self.stop_button.config(state=tk.DISABLED)
    
    def _stop_solving(self):
        """Sinaliza para parar a resolução."""
        self.stop_event.set()
        self.is_solving = False
        self.solve_button.config(state=tk.NORMAL)
        self.stop_button.config(state=tk.DISABLED)
    
    def _display_results(self, algo_name, result):
        """Exibe os resultados da busca."""
        self.results_text.config(state=tk.NORMAL)
        self.results_text.delete(1.0, tk.END)
        
        text = f"{'='*28}\n"
        text += f"ALGORITMO: {algo_name}\n"
        text += f"{'='*28}\n\n"
        
        if result.found:
            text += "✓ SOLUÇÃO ENCONTRADA!\n\n"
            text += f"Profundidade: {result.solution_depth}\n"
            text += f"Nós Visitados: {result.nodes_visited}\n"
            text += f"Tempo: {result.execution_time:.6f}s\n"
            text += f"Tempo: {result.execution_time*1000:.2f}ms\n"
        else:
            text += "✗ SOLUÇÃO NÃO ENCONTRADA!\n\n"
            text += f"Nós Visitados: {result.nodes_visited}\n"
            text += f"Tempo: {result.execution_time:.6f}s\n"
        
        self.results_text.insert(tk.END, text)
        self.results_text.config(state=tk.DISABLED)
    
    def _clear_results(self):
        """Limpa os resultados exibidos."""
        self.results_text.config(state=tk.NORMAL)
        self.results_text.delete(1.0, tk.END)
        self.results_text.config(state=tk.DISABLED)


def main():
    """Função principal."""
    root = tk.Tk()
    gui = MazeGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()
