import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def plot_gl_convergence(filename, title, output_filename):
    """
    Rysuje wykres zbieżności kwadratury Gaussa-Legendre'a
    """
    try:
        data = pd.read_csv(filename)
        
        plt.figure(figsize=(10, 6))
        
        # Wykres wartości całki
        plt.subplot(1, 2, 1)
        plt.plot(data['n'], data['gl_value'], 'bo-', linewidth=2, markersize=8)
        plt.axhline(y=data['exact'].iloc[0], color='r', linestyle='--', label='Dokładna wartość')
        plt.xlabel('Liczba węzłów (n)')
        plt.ylabel('Wartość całki')
        plt.title(f'Zbieżność kwadratury G-L dla {title}')
        plt.grid(True)
        plt.legend()
        
        # Wykres błędu względnego
        plt.subplot(1, 2, 2)
        plt.semilogy(data['n'], data['error'], 'ro-', linewidth=2, markersize=8)
        plt.xlabel('Liczba węzłów (n)')
        plt.ylabel('Błąd względny (skala logarytmiczna)')
        plt.title(f'Błąd względny kwadratury G-L dla {title}')
        plt.grid(True)
        
        plt.tight_layout()
        plt.savefig(output_filename, dpi=300, bbox_inches='tight')
        plt.close()
        
        print(f"Wykres zbieżności kwadratury G-L zapisany jako {output_filename}")
        
    except Exception as e:
        print(f"Błąd podczas generowania wykresu zbieżności: {e}")

def plot_method_comparison(filename, title, output_filename):
    """
    Rysuje wykres porównawczy różnych metod całkowania
    """
    try:
        data = pd.read_csv(filename)
        
        # Przygotuj dane do wykresu
        methods = data['method'].tolist()
        values = data['value'].tolist()
        errors = data['error'].tolist()
        
        # Znajdź indeks dokładnej wartości
        exact_idx = methods.index('exact')
        exact_value = values[exact_idx]
        
        # Posortuj metody G-L
        gl_indices = [i for i, m in enumerate(methods) if m.startswith('gl_')]
        gl_methods = [methods[i] for i in gl_indices]
        gl_values = [values[i] for i in gl_indices]
        gl_errors = [errors[i] for i in gl_indices]
        
        # Posortuj tradycyjne metody
        trad_indices = [i for i, m in enumerate(methods) if m in ['rectangle', 'trapezoid', 'simpson']]
        trad_methods = [methods[i] for i in trad_indices]
        trad_values = [values[i] for i in trad_indices]
        trad_errors = [errors[i] for i in trad_indices]
        
        # Dodaj dokładną wartość
        trad_methods.append('exact')
        trad_values.append(exact_value)
        trad_errors.append(0)
        
        # Nazwy do wyświetlenia na wykresie
        method_names = {
            'rectangle': 'Prostokąty',
            'trapezoid': 'Trapezy',
            'simpson': 'Simpson',
            'exact': 'Dokładna',
            'gl_1': 'G-L (n=1)',
            'gl_2': 'G-L (n=2)',
            'gl_3': 'G-L (n=3)',
            'gl_4': 'G-L (n=4)',
            'gl_5': 'G-L (n=5)'
        }
        
        # Konwertuj nazwy metod na czytelne etykiety
        trad_labels = [method_names[m] for m in trad_methods]
        gl_labels = [method_names[m] for m in gl_methods]
        
        plt.figure(figsize=(12, 10))
        
        # Wykres wartości całki dla tradycyjnych metod
        plt.subplot(2, 2, 1)
        bars = plt.bar(trad_labels, trad_values)
        plt.axhline(y=exact_value, color='r', linestyle='--', label='Dokładna wartość')
        plt.ylabel('Wartość całki')
        plt.title(f'Porównanie tradycyjnych metod całkowania dla {title}')
        plt.grid(axis='y')
        
        # Dodaj etykiety z wartościami
        for bar in bars:
            height = bar.get_height()
            plt.text(bar.get_x() + bar.get_width()/2., height + 0.0001*np.abs(exact_value),
                    f'{height:.8f}', ha='center', va='bottom', rotation=90)
        
        # Wykres wartości całki dla kwadratury G-L
        plt.subplot(2, 2, 2)
        bars = plt.bar(gl_labels, gl_values)
        plt.axhline(y=exact_value, color='r', linestyle='--', label='Dokładna wartość')
        plt.ylabel('Wartość całki')
        plt.title(f'Porównanie kwadratury G-L dla {title}')
        plt.grid(axis='y')
        
        # Dodaj etykiety z wartościami
        for bar in bars:
            height = bar.get_height()
            plt.text(bar.get_x() + bar.get_width()/2., height + 0.0001*np.abs(exact_value),
                    f'{height:.8f}', ha='center', va='bottom', rotation=90)
        
        # Wykres błędu względnego dla tradycyjnych metod
        plt.subplot(2, 2, 3)
        plt.bar(trad_labels[:-1], trad_errors[:-1])  # Bez dokładnej wartości
        plt.ylabel('Błąd względny')
        plt.title(f'Błąd względny tradycyjnych metod dla {title}')
        plt.grid(axis='y')
        plt.yscale('log')
        
        # Wykres błędu względnego dla kwadratury G-L
        plt.subplot(2, 2, 4)
        plt.bar(gl_labels, gl_errors)
        plt.ylabel('Błąd względny')
        plt.title(f'Błąd względny kwadratury G-L dla {title}')
        plt.grid(axis='y')
        plt.yscale('log')
        
        plt.tight_layout()
        plt.savefig(output_filename, dpi=300, bbox_inches='tight')
        plt.close()
        
        print(f"Wykres porównawczy metod całkowania zapisany jako {output_filename}")
        
    except Exception as e:
        print(f"Błąd podczas generowania wykresu porównawczego: {e}")

if __name__ == "__main__":
    # Plot convergence for x^2*sin^3(x)
    plot_gl_convergence("gl_convergence_x_sin3x.txt", "x^2*sin^3(x)", "gl_convergence_x_sin3x.png")
    
    # Plot convergence for exp(x^2)*(1-x)
    plot_gl_convergence("gl_convergence_exp_x2.txt", "exp(x^2)*(1-x)", "gl_convergence_exp_x2.png")
    
    # Plot comparison for polynomial
    plot_method_comparison("comparison_poly.txt", "wielomianu", "comparison_poly.png")
    
    # Plot comparison for x*cos^3(x)
    plot_method_comparison("comparison_xcos3x.txt", "x*cos^3(x)", "comparison_xcos3x.png")