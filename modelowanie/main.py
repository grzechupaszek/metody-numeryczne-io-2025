import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
import matplotlib.ticker as ticker

# Dane z tabeli
methods = ["Jacobi", "Gauss-Seidel", "GMRES", "GMRES+Jacobi", "GMRES+GS", "GMRES+ILU(0)"]
convergent = ["NIE", "TAK", "NIE", "NIE", "TAK", "TAK"]
max_norms = [6.65e+23, 1.9e-6, 1068.769, 3.41e+18, 0.005622, 0.000599]
iterations = [100, 100, 100, 100, 100, 13]
red_norms = [1.545396, 0.800983, 0.85074, 1.247147, 0.38859, 0.11504]
times = [0.082684, 0.019541, 0.01049, 0.007068, 0.008741, 0.00983]

# Tworzenie sztucznych danych dla przebiegu zbieżności normy w kolejnych iteracjach
n_iterations = max(iterations)
convergence_data = {}

for i, method in enumerate(methods):
    if convergent[i] == "TAK":
        # Dla zbieżnych metod - wykładniczy spadek normy
        if method == "GMRES+ILU(0)":
            # Specjalny przypadek dla metody, która zbiegła w 13 iteracjach
            temp_data = np.geomspace(1, max_norms[i], 13)
            convergence_data[method] = np.pad(temp_data[::-1], (0, n_iterations-13), 'constant', constant_values=max_norms[i])
        else:
            # Pozostałe zbieżne metody
            convergence_data[method] = np.geomspace(1, max_norms[i], n_iterations)[::-1]
    else:
        # Dla niezbieżnych metod - różne scenariusze
        if method == "Jacobi":
            # Rosnąca wykładniczo
            convergence_data[method] = np.geomspace(1, max_norms[i], n_iterations)
        elif method == "GMRES":
            # Oscylująca z tendencją rosnącą
            base = np.linspace(1, max_norms[i], n_iterations)
            noise = np.sin(np.linspace(0, 20*np.pi, n_iterations)) * max_norms[i] * 0.1
            convergence_data[method] = base + noise
        else:
            # Początkowo malejąca, potem rosnąca
            half = n_iterations // 2
            first_half = np.geomspace(1, 1e-3, half)[::-1]
            second_half = np.geomspace(1e-3, max_norms[i], n_iterations - half)
            convergence_data[method] = np.concatenate([first_half, second_half])

# Tworzenie wykresu
plt.figure(figsize=(12, 7))
cmap = cm.get_cmap('viridis', len(methods))

for i, method in enumerate(methods):
    plt.semilogy(range(1, n_iterations+1), convergence_data[method], 
                 label=f"{method} ({'zbieżna' if convergent[i]=='TAK' else 'niezbieżna'})",
                 color=cmap(i), linewidth=2)

# Dodanie pionowej linii dla GMRES+ILU(0), która zbiegła w 13 iteracjach
plt.axvline(x=13, color='red', linestyle='--', alpha=0.5, 
            label="GMRES+ILU(0) zbiegła po 13 iteracjach")

plt.title('Symulacja przebiegu normy błędu w zależności od iteracji', fontsize=14)
plt.xlabel('Iteracja', fontsize=12)
plt.ylabel('Norma błędu (skala logarytmiczna)', fontsize=12)
plt.grid(True, which="both", ls="--", alpha=0.7)
plt.legend(fontsize=10, loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=2)
plt.tight_layout()

# Drugi wykres - porównanie norm końcowych
plt.figure(figsize=(12, 6))

# Sortowanie metod według końcowej normy
sorted_indices = np.argsort(max_norms)
sorted_methods = [methods[i] for i in sorted_indices]
sorted_norms = [max_norms[i] for i in sorted_indices]
sorted_convergent = [convergent[i] for i in sorted_indices]

# Kolorowanie słupków w zależności od zbieżności
colors = ['green' if conv == 'TAK' else 'red' for conv in sorted_convergent]

bars = plt.bar(sorted_methods, sorted_norms, color=colors, alpha=0.7)
plt.yscale('log')
plt.title('Porównanie końcowych norm błędu dla różnych metod', fontsize=14)
plt.xlabel('Metoda', fontsize=12)
plt.ylabel('Końcowa norma błędu (skala logarytmiczna)', fontsize=12)
plt.xticks(rotation=45, ha='right')
plt.grid(True, which="both", ls="--", alpha=0.4, axis='y')

# Dodanie etykiet z wartościami
def format_value(value):
    if value >= 1e6:
        return f"{value:.2e}"
    elif value < 0.001:
        return f"{value:.2e}"
    else:
        return f"{value:.6f}"

for bar in bars:
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2., height,
            format_value(height),
            ha='center', va='bottom', rotation=90, fontsize=9)

plt.tight_layout()
plt.show()

# UWAGA: Ten kod generuje symulowane dane zbieżności, ponieważ nie mamy
# rzeczywistych danych dla każdej iteracji. Prawdziwy wykres powinien
# być oparty na faktycznych wartościach normy zapisanych podczas obliczeń.