"""
Script principal para a execução dos algoritmos de busca no Labirinto.
Fornece uma interface de linha de comando para seleção de labirinto e método de busca.
Coleta e exibe métricas de desempenho.
"""

from maze_solver import Maze
from search_algorithms import SearchAlgorithms
import json
import random


def print_menu():
    """Exibe o menu principal."""
    print("\n" + "="*60)
    print("         SOLVER DE LABIRINTO COM ALGORITMOS DE BUSCA")
    print("="*60)
    print("\n1. Usar labirinto padrão (10x10)")
    print("2. Usar labirinto aleatório")
    print("3. Sair")
    print("-"*60)


def get_random_maze_size():
    """Permite ao usuário escolher o tamanho do labirinto aleatório."""
    print("\nTamanho do labirinto aleatório:")
    print("1. Pequeno (10x10)")
    print("2. Médio (15x15)")
    print("3. Grande (20x20)")
    print("4. Customizado")
    
    choice = input("Escolha uma opção: ").strip()
    
    if choice == '1':
        return 10, 10, 0.25
    elif choice == '2':
        return 15, 15, 0.30
    elif choice == '3':
        return 20, 20, 0.35
    elif choice == '4':
        try:
            rows = int(input("Número de linhas: "))
            cols = int(input("Número de colunas: "))
            density = float(input("Densidade de paredes (0.0 a 1.0): "))
            return rows, cols, min(max(density, 0.0), 1.0)
        except ValueError:
            print("Entrada inválida. Usando padrão (15x15).")
            return 15, 15, 0.30
    else:
        print("Opção inválida. Usando padrão (15x15).")
        return 15, 15, 0.30


def print_algorithm_menu():
    """Exibe o menu de seleção de algoritmos."""
    print("\n" + "-"*60)
    print("SELECIONE OS ALGORITMOS A EXECUTAR:")
    print("-"*60)
    print("1. Busca em Largura (BFS)")
    print("2. Busca Gulosa com Heurística de Manhattan")
    print("3. A* com Heurística Euclidiana (H1)")
    print("4. A* com Heurística de Manhattan (H2)")
    print("5. A* com Heurística de Chebyshev (H3)")
    print("6. Executar todos os algoritmos")
    print("7. Voltar ao menu principal")
    print("-"*60)


def display_results(algorithm_name, result, maze):
    """Exibe os resultados de uma busca."""
    print("\n" + "="*60)
    print(f"RESULTADOS - {algorithm_name}")
    print("="*60)
    
    if result.found:
        print(f"✓ Solução encontrada!")
        print(f"\nPonto de Partida: {maze.position_to_string(maze.start)}")
        print(f"Ponto de Chegada: {maze.position_to_string(maze.goal)}")
        print(f"\nCaminho (primeiros 10 passos):")
        for i, pos in enumerate(result.path[:10]):
            print(f"  {i}: {maze.position_to_string(pos)}")
        if len(result.path) > 10:
            print(f"  ... ({len(result.path) - 10} passos adicionais)")
        
        print(f"\n{'Métrica':<30} {'Valor':<20}")
        print("-"*50)
        print(f"{'Profundidade da Solução':<30} {result.solution_depth:<20}")
        print(f"{'Nós Visitados':<30} {result.nodes_visited:<20}")
        print(f"{'Tempo de Execução (s)':<30} {result.execution_time:<20.6f}")
        print(f"{'Tempo de Execução (ms)':<30} {result.execution_time*1000:<20.2f}")
        
        print(f"\nLabirinto com solução:")
        print(maze.get_maze_string(result.path))
    else:
        print("✗ Solução NÃO encontrada!")
        print(f"{'Nós Visitados':<30} {result.nodes_visited:<20}")
        print(f"{'Tempo de Execução (s)':<30} {result.execution_time:<20.6f}")


def run_algorithm(maze, search_alg, algorithm_choice):
    """Executa um algoritmo específico."""
    if algorithm_choice == 1:
        result = search_alg.bfs()
        display_results("Busca em Largura (BFS)", result, maze)
    
    elif algorithm_choice == 2:
        result = search_alg.greedy_search(maze.manhattan_distance)
        display_results("Busca Gulosa (Manhattan)", result, maze)
    
    elif algorithm_choice == 3:
        result = search_alg.a_star(maze.euclidean_distance)
        display_results("A* com Heurística H1 (Euclidiana)", result, maze)
    
    elif algorithm_choice == 4:
        result = search_alg.a_star(maze.manhattan_distance)
        display_results("A* com Heurística H2 (Manhattan)", result, maze)
    
    elif algorithm_choice == 5:
        result = search_alg.a_star(maze.chebyshev_distance)
        display_results("A* com Heurística H3 (Chebyshev)", result, maze)
    
    elif algorithm_choice == 6:
        # Executar todos
        for choice in range(1, 6):
            run_algorithm(maze, search_alg, choice)


def save_results_to_file(maze, search_alg, filename="maze_results.json"):
    """Salva os resultados de todos os algoritmos em um arquivo JSON."""
    results = {
        "maze_size": (maze.rows, maze.cols),
        "start": maze.start,
        "goal": maze.goal,
        "algorithms": {}
    }
    
    algorithms = [
        (1, "BFS", lambda: search_alg.bfs()),
        (2, "Greedy (Manhattan)", lambda: search_alg.greedy_search(maze.manhattan_distance)),
        (3, "A* (H1 - Euclidean)", lambda: search_alg.a_star(maze.euclidean_distance)),
        (4, "A* (H2 - Manhattan)", lambda: search_alg.a_star(maze.manhattan_distance)),
        (5, "A* (H3 - Chebyshev)", lambda: search_alg.a_star(maze.chebyshev_distance)),
    ]
    
    for _, name, algo_func in algorithms:
        result = algo_func()
        results["algorithms"][name] = {
            "found": result.found,
            "solution_depth": result.solution_depth,
            "nodes_visited": result.nodes_visited,
            "execution_time": result.execution_time
        }
    
    with open(filename, 'w') as f:
        json.dump(results, f, indent=2)
    
    print(f"\n✓ Resultados salvos em '{filename}'")


def main():
    """Função principal."""
    while True:
        print_menu()
        choice = input("Escolha uma opção: ").strip()
        
        if choice == '1':
            maze = Maze()
        elif choice == '2':
            rows, cols, density = get_random_maze_size()
            maze = Maze.create_random_maze(rows, cols, density)
        elif choice == '3':
            print("\nAté logo!")
            break
        else:
            print("Opção inválida!")
            continue
        
        search_alg = SearchAlgorithms(maze)
        
        print(f"\nLabirinto ({maze.rows}x{maze.cols}):")
        print(maze.get_maze_string())
        
        while True:
            print_algorithm_menu()
            algo_choice = input("Escolha uma opção: ").strip()
            
            try:
                algo_choice = int(algo_choice)
                
                if algo_choice == 7:
                    break
                elif 1 <= algo_choice <= 6:
                    run_algorithm(maze, search_alg, algo_choice)
                    
                    # Perguntar se deseja salvar resultados
                    if algo_choice == 6:
                        save_choice = input("\nDeseja salvar os resultados em um arquivo? (s/n): ").strip().lower()
                        if save_choice == 's':
                            save_results_to_file(maze, search_alg)
                else:
                    print("Opção inválida!")
            except ValueError:
                print("Entrada inválida!")


if __name__ == "__main__":
    main()
