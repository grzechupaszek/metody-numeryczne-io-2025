import matplotlib.pyplot as plt
import numpy as np
import os

# Ustawienia wykresów
plt.rcParams['figure.figsize'] = (10, 6)
plt.rcParams['font.size'] = 12
plt.rcParams['lines.linewidth'] = 2

# Wczytywanie danych
def load_data(filename):
    try:
        data = np.loadtxt(filename)
        return data[:, 0], data[:, 1]
    except Exception as e:
        print(f"Błąd wczytywania pliku {filename}: {e}")
        return None, None

# Wczytaj dane analityczne na początku
t_ana, T_ana = load_data('analytical.dat')
if t_ana is None:
    print("UWAGA: Nie można wczytać danych analitycznych!")

# Wykres 1: Krzywe chłodzenia dla różnych kroków czasowych (metoda RK4)
plt.figure(1)
time_steps = [10.0, 5.0, 2.0, 1.0]
colors = ['red', 'blue', 'green', 'orange']

for i, h in enumerate(time_steps):
    # Formatowanie nazwy pliku zgodnie z C++
    h_str = str(int(h)) if h.is_integer() else str(h)
    t, T = load_data(f'rk4_h{h_str}.dat')
    if t is not None:
        plt.plot(t, T, color=colors[i], label=f'h = {h} s', linewidth=2)

# Rozwiązanie analityczne
t_ana, T_ana = load_data('analytical.dat')
if t_ana is not None:
    plt.plot(t_ana, T_ana, 'k--', label='Rozwiązanie analityczne', linewidth=2)

plt.xlabel('Czas [s]')
plt.ylabel('Temperatura [°C]')
plt.title('Krzywe chłodzenia kuli - Metoda Rungego-Kutty 4. rzędu')
plt.legend()
plt.grid(True, alpha=0.3)
plt.xlim(0, 2374)
plt.savefig('cooling_curves_rk4.png', dpi=300, bbox_inches='tight')
plt.close()

# Wykres 2: Porównanie metod dla h = 10.0 (większy krok dla lepszej widoczności różnic)
plt.figure(2)
h = 10.0  # Zmiana z 2.0 na 10.0
h_str = str(int(h)) if h.is_integer() else str(h)

t_heun, T_heun = load_data(f'heun_h{h_str}.dat')
t_mid, T_mid = load_data(f'midpoint_h{h_str}.dat')
t_rk4, T_rk4 = load_data(f'rk4_h{h_str}.dat')

# Ograniczenie do pierwszych 500 sekund dla lepszej widoczności
max_time = 500
if t_heun is not None:
    mask = t_heun <= max_time
    plt.plot(t_heun[mask], T_heun[mask], 'r-', label='Metoda Heuna', linewidth=2)
if t_mid is not None:
    mask = t_mid <= max_time
    plt.plot(t_mid[mask], T_mid[mask], 'b--', label='Metoda punktu środkowego', linewidth=2, alpha=0.8)
if t_rk4 is not None:
    mask = t_rk4 <= max_time
    plt.plot(t_rk4[mask], T_rk4[mask], 'g-.', label='Metoda RK4', linewidth=2)
if t_ana is not None:
    mask = t_ana <= max_time
    plt.plot(t_ana[mask], T_ana[mask], 'k:', label='Rozwiązanie analityczne', linewidth=3)

plt.xlabel('Czas [s]')
plt.ylabel('Temperatura [°C]')
plt.title(f'Porównanie metod numerycznych (h = {h} s, pierwsze 500 s)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.xlim(0, max_time)
plt.savefig('methods_comparison.png', dpi=300, bbox_inches='tight')
plt.close()

# Wykres 3: Błąd MSE w funkcji kroku czasowego
plt.figure(3)
mse_data = np.loadtxt('mse_results.dat', skiprows=1)
h_values = mse_data[:, 0]
mse_heun = mse_data[:, 1]
mse_mid = mse_data[:, 2]
mse_rk4 = mse_data[:, 3]

plt.loglog(h_values, mse_heun, 'ro-', label='Metoda Heuna', linewidth=2, markersize=8)
plt.loglog(h_values, mse_mid, 'bs-', label='Metoda punktu środkowego', linewidth=2, markersize=8)
plt.loglog(h_values, mse_rk4, 'g^-', label='Metoda RK4', linewidth=2, markersize=8)

plt.xlabel('Krok czasowy h [s]')
plt.ylabel('Średni błąd kwadratowy MSE')
plt.title('Zależność błędu MSE od kroku czasowego')
plt.legend()
plt.grid(True, alpha=0.3, which="both")
plt.savefig('mse_comparison.png', dpi=300, bbox_inches='tight')
plt.close()

# Wykres 4: Błąd bezwzględny dla h = 1.0
plt.figure(4)
h = 1.0
h_str = str(int(h)) if h.is_integer() else str(h)

# Wczytaj dane ponownie dla pewności
t_heun, T_heun = load_data(f'heun_h{h_str}.dat')
t_mid, T_mid = load_data(f'midpoint_h{h_str}.dat')
t_rk4, T_rk4 = load_data(f'rk4_h{h_str}.dat')
t_analytical, T_analytical = load_data('analytical.dat')

if t_analytical is not None and T_analytical is not None:
    # Interpolacja rozwiązania analitycznego
    from scipy.interpolate import interp1d
    f_ana = interp1d(t_analytical, T_analytical, kind='linear', fill_value='extrapolate')
    
    # Oblicz i wykreśl błędy dla każdej metody
    if t_heun is not None and T_heun is not None:
        error_heun = np.abs(T_heun - f_ana(t_heun))
        plt.semilogy(t_heun[::10], error_heun[::10], 'r-', label='Metoda Heuna', linewidth=3)
        print(f"Błąd Heuna - min: {np.min(error_heun):.2e}, max: {np.max(error_heun):.2e}")
    
    if t_mid is not None and T_mid is not None:
        error_mid = np.abs(T_mid - f_ana(t_mid))
        plt.semilogy(t_mid[::10], error_mid[::10], 'b--', label='Metoda punktu środkowego', linewidth=2, alpha=0.8)
        print(f"Błąd punktu środkowego - min: {np.min(error_mid):.2e}, max: {np.max(error_mid):.2e}")
    
    if t_rk4 is not None and T_rk4 is not None:
        error_rk4 = np.abs(T_rk4 - f_ana(t_rk4))
        # Dodaj małą wartość aby uniknąć log(0)
        error_rk4[error_rk4 < 1e-15] = 1e-15
        plt.semilogy(t_rk4[::10], error_rk4[::10], 'g-.', label='Metoda RK4', linewidth=2)
        print(f"Błąd RK4 - min: {np.min(error_rk4):.2e}, max: {np.max(error_rk4):.2e}")
    
    plt.xlabel('Czas [s]')
    plt.ylabel('Błąd bezwzględny [°C]')
    plt.title(f'Błąd bezwzględny w czasie (h = {h} s)')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.xlim(0, 2374)
    plt.ylim(1e-15, 1e-2)
else:
    print("Błąd: Nie można wczytać danych analitycznych!")
    plt.text(0.5, 0.5, 'Błąd wczytywania danych analitycznych', 
             horizontalalignment='center', verticalalignment='center',
             transform=plt.gca().transAxes)

plt.savefig('absolute_error.png', dpi=300, bbox_inches='tight')
plt.close()

print("Wykresy zostały wygenerowane pomyślnie!")