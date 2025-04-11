import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Wczytanie danych z pliku
def plot_convergence():
    try:
        data = pd.read_csv('convergence_data.txt')
        
        # Sprawdź, czy kolumna 'exact' istnieje w danych
        has_exact = 'exact' in data.columns
        
        # Odczytaj dokładną wartość z pliku, jeśli kolumna nie istnieje
        if not has_exact:
            try:
                with open('exact_poly.txt', 'r') as f:
                    exact_value = float(f.read().strip())
                data['exact'] = exact_value
                has_exact = True
            except:
                print("Nie można odczytać dokładnej wartości z pliku exact_poly.txt")
        
        plt.figure(figsize=(12, 8))
        
        # Skala logarytmiczna dla n
        plt.semilogx(data['n'], data['rectangle'], 'ro-', label='Metoda prostokątów')
        plt.semilogx(data['n'], data['trapezoid'], 'gs-', label='Metoda trapezów')
        plt.semilogx(data['n'], data['simpson'], 'bd-', label='Metoda Simpsona')
        
        # Dodaj linię dla dokładnej wartości, jeśli jest dostępna
        if has_exact:
            plt.semilogx(data['n'], [data['exact'].iloc[0]] * len(data['n']), 'k--', 
                        label='Dokładna wartość')
        
        plt.xlabel('Liczba podziałów (n)')
        plt.ylabel('Wartość całki')
        plt.title('Zbieżność metod całkowania numerycznego')
        plt.grid(True, which="both", ls="-")
        plt.legend()
        
        plt.savefig('convergence_plot.png', dpi=300, bbox_inches='tight')
        plt.close()
        
        # Obliczenie błędów względem dokładnej wartości, jeśli jest dostępna
        if has_exact:
            exact_value = data['exact'].iloc[0]
            data['rect_error'] = np.abs((data['rectangle'] - exact_value) / exact_value)
            data['trap_error'] = np.abs((data['trapezoid'] - exact_value) / exact_value)
            data['simp_error'] = np.abs((data['simpson'] - exact_value) / exact_value)
        else:
            # Jeśli brak dokładnej wartości, oblicz błędy względem najdokładniejszej metody
            max_n_idx = data['n'].idxmax()
            best_rect = data.loc[max_n_idx, 'rectangle']
            best_trap = data.loc[max_n_idx, 'trapezoid']
            best_simp = data.loc[max_n_idx, 'simpson']
            
            data['rect_error'] = np.abs((data['rectangle'] - best_rect) / best_rect)
            data['trap_error'] = np.abs((data['trapezoid'] - best_trap) / best_trap)
            data['simp_error'] = np.abs((data['simpson'] - best_simp) / best_simp)
        
        plt.figure(figsize=(12, 8))
        
        plt.loglog(data['n'], data['rect_error'], 'ro-', label='Metoda prostokątów')
        plt.loglog(data['n'], data['trap_error'], 'gs-', label='Metoda trapezów')
        plt.loglog(data['n'], data['simp_error'], 'bd-', label='Metoda Simpsona')
        
        # Dodaj linie trendu, aby pokazać rząd zbieżności
        n_values = data['n'].values
        
        # Dla metody prostokątów (O(h^2))
        plt.loglog(n_values, 1.0 / (n_values**2) * n_values[0]**2 * data['rect_error'].iloc[0], 
                  'r--', alpha=0.5, label='O(h²)')
        
        # Dla metody trapezów (O(h^2))
        plt.loglog(n_values, 1.0 / (n_values**2) * n_values[0]**2 * data['trap_error'].iloc[0], 
                  'g--', alpha=0.5)
        
        # Dla metody Simpsona (O(h^4))
        plt.loglog(n_values, 1.0 / (n_values**4) * n_values[0]**4 * data['simp_error'].iloc[0], 
                  'b--', alpha=0.5, label='O(h⁴)')
        
        plt.xlabel('Liczba podziałów (n)')
        plt.ylabel('Błąd względny')
        plt.title('Zbieżność błędu dla metod całkowania numerycznego')
        plt.grid(True, which="both", ls="-")
        plt.legend()
        
        plt.savefig('error_plot.png', dpi=300, bbox_inches='tight')
        plt.close()
        
        # Generowanie wykresu porównania metod dla funkcji x*cos^3(x)
        try:
            # Wczytaj dokładną wartość całki x*cos^3(x)
            with open('exact_xcos3x.txt', 'r') as f:
                exact_xcos3x = float(f.read().strip())
            
            # Utwórz dane do wykresu
            methods = ['Prostokąty', 'Trapezy', 'Simpson', 'Dokładna']
            
            # Przykładowe wartości - zastąp je rzeczywistymi wynikami
            # Możesz je przekazać jako argumenty lub odczytać z pliku
            values = [4.20251, 4.2025, 4.2025, exact_xcos3x]
            
            plt.figure(figsize=(10, 6))
            bars = plt.bar(methods, values)
            
            # Dodaj etykiety
            for bar in bars:
                height = bar.get_height()
                plt.text(bar.get_x() + bar.get_width()/2., height + 0.001,
                        f'{height:.6f}', ha='center', va='bottom', rotation=0)
            
            plt.title('Porównanie metod całkowania dla funkcji x*cos³(x)')
            plt.ylabel('Wartość całki')
            plt.grid(axis='y', linestyle='--', alpha=0.7)
            plt.savefig('comparison_xcos3x.png', dpi=300, bbox_inches='tight')
            plt.close()
            
        except Exception as e:
            print(f"Błąd podczas generowania wykresu porównania: {e}")
        
        print("Wykresy zostały zapisane jako 'convergence_plot.png', 'error_plot.png' i 'comparison_xcos3x.png'")
        
    except Exception as e:
        print(f"Wystąpił błąd: {e}")

if __name__ == "__main__":
    plot_convergence()