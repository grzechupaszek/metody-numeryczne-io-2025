import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from scipy.optimize import fsolve
import warnings
warnings.filterwarnings('ignore')

plt.rcParams['font.size'] = 12
plt.rcParams['figure.figsize'] = (12, 8)

class RootAnalyzer:
    def __init__(self):
        self.tolerance = 1e-10
        self.max_iterations = 1000
    
    # Definicje funkcji
    def function1(self, x):
        """ln(1-x) + 1/(x²+3) = 0"""
        with np.errstate(divide='ignore', invalid='ignore'):
            mask = (x != 1) & np.isfinite(x)
            result = np.full_like(x, np.inf)
            result[mask] = np.log(1 - x[mask]) + 1.0 / (x[mask]**2 + 3)
            return result
    
    def function2(self, x):
        """x³ + 30·sin(x) - 12/x - 28 = 0"""
        with np.errstate(divide='ignore', invalid='ignore'):
            mask = (x != 0) & np.isfinite(x)
            result = np.full_like(x, np.inf)
            result[mask] = x[mask]**3 + 30 * np.sin(x[mask]) - 12.0/x[mask] - 28
            return result
    
    def function3(self, x):
        """cos(3πx)/(x+2) - 1/(x+4) = 0"""
        with np.errstate(divide='ignore', invalid='ignore'):
            mask = (x != -2) & (x != -4) & np.isfinite(x)
            result = np.full_like(x, np.inf)
            result[mask] = (np.cos(3 * np.pi * x[mask]) / (x[mask] + 2) - 
                           1.0 / (x[mask] + 4))
            return result
    
    # Pochodne analityczne
    def derivative1(self, x):
        with np.errstate(divide='ignore', invalid='ignore'):
            mask = (x != 1) & (x != 0) & np.isfinite(x)
            result = np.full_like(x, np.inf)
            result[mask] = -1.0 / (1 - x[mask]) - 2*x[mask] / (x[mask]**2 + 3)**2
            return result
    
    def derivative2(self, x):
        with np.errstate(divide='ignore', invalid='ignore'):
            mask = (x != 0) & np.isfinite(x)
            result = np.full_like(x, np.inf)
            result[mask] = 3*x[mask]**2 + 30 * np.cos(x[mask]) + 12.0 / x[mask]**2
            return result
    
    def derivative3(self, x):
        with np.errstate(divide='ignore', invalid='ignore'):
            mask = (x != -2) & (x != -4) & np.isfinite(x)
            result = np.full_like(x, np.inf)
            result[mask] = (-3 * np.pi * np.sin(3 * np.pi * x[mask]) / (x[mask] + 2) -
                           np.cos(3 * np.pi * x[mask]) / (x[mask] + 2)**2 + 
                           1.0 / (x[mask] + 4)**2)
            return result
    
    def bisection_method(self, func, a, b):
        """Metoda bisekcji"""
        if func(a) * func(b) > 0:
            return None, []
        
        approximations = []
        for i in range(self.max_iterations):
            c = (a + b) / 2
            approximations.append(c)
            
            if abs(func(c)) < self.tolerance or abs(b - a) < self.tolerance:
                return c, approximations
            
            if func(a) * func(c) < 0:
                b = c
            else:
                a = c
        
        return approximations[-1] if approximations else None, approximations
    
    def newton_method(self, func, dfunc, x0):
        """Metoda Newtona"""
        approximations = []
        x = x0
        
        for i in range(self.max_iterations):
            approximations.append(x)
            
            fx = func(x)
            dfx = dfunc(x)
            
            if abs(dfx) < 1e-15:
                break
            
            x_new = x - fx / dfx
            
            if abs(x_new - x) < self.tolerance:
                return x_new, approximations
            
            x = x_new
        
        return approximations[-1] if approximations else None, approximations
    
    def secant_method(self, func, x0, x1):
        """Metoda siecznych"""
        approximations = [x0, x1]
        
        for i in range(self.max_iterations):
            f0 = func(x0)
            f1 = func(x1)
            
            if abs(f1 - f0) < 1e-15:
                break
            
            x2 = x1 - f1 * (x1 - x0) / (f1 - f0)
            approximations.append(x2)
            
            if abs(x2 - x1) < self.tolerance:
                return x2, approximations
            
            x0, x1 = x1, x2
        
        return approximations[-1] if approximations else None, approximations
    
    def find_roots_in_interval(self, func, start, end, num_points=1000):
        """Znajdź przybliżone lokalizacje pierwiastków"""
        x = np.linspace(start, end, num_points)
        y = func(x)
        
        # Znajdź zmiany znaku
        roots = []
        for i in range(len(y) - 1):
            if np.isfinite(y[i]) and np.isfinite(y[i+1]) and y[i] * y[i+1] < 0:
                # Użyj metody bisekcji do dokładnego znajdowania pierwiastka
                root, _ = self.bisection_method(func, x[i], x[i+1])
                if root is not None:
                    roots.append(root)
        
        return roots
    
    def plot_functions(self):
        """Rysuj wszystkie funkcje"""
        functions = [self.function1, self.function2, self.function3]
        titles = [
            r'$f_1(x) = \ln(1-x) + \frac{1}{x^2+3}$',
            r'$f_2(x) = x^3 + 30\sin(x) - \frac{12}{x} - 28$',
            r'$f_3(x) = \frac{\cos(3\pi x)}{x+2} - \frac{1}{x+4}$'
        ]
        
        fig, axes = plt.subplots(3, 1, figsize=(12, 15))
        
        for i, (func, title) in enumerate(zip(functions, titles)):
            ax = axes[i]
            
            # Różne zakresy dla różnych funkcji
            if i == 0:  # function1
                x = np.linspace(-2.9, 0.9, 1000)
            elif i == 1:  # function2
                x1 = np.linspace(-2.9, -0.1, 500)
                x2 = np.linspace(0.1, 3.9, 500)
                x = np.concatenate([x1, x2])
            else:  # function3
                x1 = np.linspace(-2.9, -2.1, 200)
                x2 = np.linspace(-1.9, -4.1, 200)
                x3 = np.linspace(-3.9, 3.9, 600)
                x = np.linspace(-2.9, 3.9, 1000)
                # Usuń punkty blisko asymptot
                mask = (np.abs(x + 2) > 0.1) & (np.abs(x + 4) > 0.1)
                x = x[mask]
            
            y = func(x)
            
            # Usuń wartości nieskończone do plotowania
            mask = np.isfinite(y) & (np.abs(y) < 100)
            x_plot = x[mask]
            y_plot = y[mask]
            
            ax.plot(x_plot, y_plot, 'b-', linewidth=2, label=f'$f_{i+1}(x)$')
            ax.axhline(y=0, color='k', linestyle='--', alpha=0.5)
            ax.grid(True, alpha=0.3)
            ax.set_title(title, fontsize=14)
            ax.set_xlabel('x')
            ax.set_ylabel('f(x)')
            
            # Znajdź i zaznacz pierwiastki
            roots = self.find_roots_in_interval(func, -3, 4)
            for root in roots:
                ax.plot(root, 0, 'ro', markersize=8, label=f'Pierwiastek: {root:.4f}')
            
            ax.legend()
            ax.set_xlim(-3, 4)
            
            # Ustaw limity y dla lepszej widoczności
            if len(y_plot) > 0:
                y_range = np.max(y_plot) - np.min(y_plot)
                ax.set_ylim(np.min(y_plot) - 0.1*y_range, np.max(y_plot) + 0.1*y_range)
        
        plt.tight_layout()
        plt.savefig('functions_plot.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    def compare_methods(self, func, dfunc, root_approx, func_name):
        """Porównaj różne metody dla danego pierwiastka"""
        # Znajdź przedział dla bisekcji
        a, b = root_approx - 0.5, root_approx + 0.5
        while func(a) * func(b) > 0:
            a -= 0.1
            b += 0.1
            if b - a > 2:
                break
        
        # Zastosuj metody
        methods = {}
        
        # Bisekcja
        root_bis, approx_bis = self.bisection_method(func, a, b)
        if root_bis is not None:
            methods['Bisekcja'] = {'root': root_bis, 'approx': approx_bis}
        
        # Newton
        root_newton, approx_newton = self.newton_method(func, dfunc, root_approx + 0.1)
        if root_newton is not None:
            methods['Newton'] = {'root': root_newton, 'approx': approx_newton}
        
        # Sieczne
        root_secant, approx_secant = self.secant_method(func, root_approx - 0.1, root_approx + 0.1)
        if root_secant is not None:
            methods['Sieczne'] = {'root': root_secant, 'approx': approx_secant}
        
        # Wykres zbieżności
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))
        
        # Zbieżność do pierwiastka
        colors = ['red', 'blue', 'green']
        for i, (method_name, data) in enumerate(methods.items()):
            iterations = range(len(data['approx']))
            ax1.plot(iterations, data['approx'], 'o-', color=colors[i], 
                    label=f"{method_name} → {data['root']:.8f}", markersize=4)
        
        ax1.axhline(y=root_approx, color='black', linestyle='--', alpha=0.5, 
                   label=f'Prawdziwy pierwiastek ≈ {root_approx:.8f}')
        ax1.set_xlabel('Iteracja')
        ax1.set_ylabel('Przybliżenie pierwiastka')
        ax1.set_title(f'Zbieżność metod - {func_name}')
        
        # Umieść legendę poza wykresem dla funkcji 3
        if 'Funkcja 3' in func_name:
            ax1.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
        else:
            ax1.legend()
        ax1.grid(True, alpha=0.3)
        
        # Błąd bezwzględny
        for i, (method_name, data) in enumerate(methods.items()):
            iterations = range(len(data['approx']))
            errors = [abs(x - data['root']) for x in data['approx']]
            # Usuń wartości zerowe dla wykresu logarytmicznego
            valid_errors = [(iter, err) for iter, err in zip(iterations, errors) if err > 0]
            if valid_errors:
                valid_iterations, valid_errors = zip(*valid_errors)
                ax2.semilogy(valid_iterations, valid_errors, 'o-', color=colors[i], 
                            label=method_name, markersize=4)
        
        ax2.set_xlabel('Iteracja')
        ax2.set_ylabel('Błąd bezwzględny (skala log)')
        ax2.set_title(f'Błąd bezwzględny - {func_name}')
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig(f'convergence_{func_name.lower().replace(" ", "_")}.png', 
                   dpi=300, bbox_inches='tight')
        plt.show()
        
        return methods
    
    def create_results_table(self, all_results):
        """Utwórz tabelę z wynikami"""
        data = []
        for func_name, results in all_results.items():
            for method_name, method_data in results.items():
                data.append({
                    'Funkcja': func_name,
                    'Metoda': method_name,
                    'Pierwiastek': f"{method_data['root']:.10f}",
                    'Liczba iteracji': len(method_data['approx']),
                    'Błąd końcowy': f"{abs(method_data['approx'][-1] - method_data['root']):.2e}"
                })
        
        df = pd.DataFrame(data)
        print("=== TABELA WYNIKÓW ===")
        print(df.to_string(index=False))
        print()
        
        return df
    
    def run_analysis(self):
        """Uruchom pełną analizę"""
        print("=== ANALIZA METOD NUMERYCZNYCH ===\n")
        
        # Rysuj funkcje
        self.plot_functions()
        
        # Analizuj każdą funkcję
        functions = [
            (self.function1, self.derivative1, "Funkcja 1"),
            (self.function2, self.derivative2, "Funkcja 2"),
            (self.function3, self.derivative3, "Funkcja 3")
        ]
        
        all_results = {}
        
        for func, dfunc, func_name in functions:
            print(f"\n=== {func_name.upper()} ===")
            
            # Znajdź pierwiastki
            roots = self.find_roots_in_interval(func, -3, 4)
            print(f"Znalezione pierwiastki w przedziale [-3, 4]: {len(roots)}")
            
            for i, root in enumerate(roots):
                print(f"Pierwiastek {i+1}: {root:.10f}")
            
            # Analizuj pierwszy pierwiastek (jeśli istnieje)
            if roots:
                root_to_analyze = roots[0]
                print(f"\nAnaliza zbieżności dla pierwiastka: {root_to_analyze:.10f}")
                
                methods_results = self.compare_methods(func, dfunc, root_to_analyze, func_name)
                all_results[func_name] = methods_results
        
        # Utwórz tabelę wyników
        if all_results:
            results_df = self.create_results_table(all_results)
            results_df.to_csv('results_summary.csv', index=False)
            print("Wyniki zapisane do pliku 'results_summary.csv'")

# Uruchom analizę
if __name__ == "__main__":
    analyzer = RootAnalyzer()
    analyzer.run_analysis()