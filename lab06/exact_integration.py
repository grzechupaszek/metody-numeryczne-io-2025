import numpy as np
import sympy as sp
from scipy import integrate

def exact_polynomial_integral():
    """
    Oblicza dokładną wartość całki wielomianu z pliku dane.txt
    używając operacji symbolicznych
    """
    # Wczytaj dane z pliku
    with open('dane.txt', 'r') as f:
        lines = f.readlines()
    
    # Parsuj dane
    degree = int(lines[0].strip())
    coeffs = list(map(float, lines[1].strip().split()))
    a_range, b_range = map(float, lines[2].strip().split())
    
    # Utwórz zmienną symboliczną
    x = sp.Symbol('x')
    
    # Utwórz wielomian symboliczny
    polynomial = 0
    for i, coeff in enumerate(coeffs):
        polynomial += coeff * x**(degree-i)
    
    # Oblicz całkę symboliczną
    integral = sp.integrate(polynomial, (x, a_range, b_range))
    
    # Konwertuj wynik na liczbę zmiennoprzecinkową
    exact_result = float(integral)
    
    print(f"Dokładna wartość całki wielomianu: {exact_result}")
    
    # Zapisz wynik do pliku
    with open('exact_poly.txt', 'w') as f:
        f.write(str(exact_result))
    
    return exact_result

def exact_xcos3x_integral():
    """
    Oblicza dokładną wartość całki x*cos^3(x) w przedziale [3.5, 6.52968912439344]
    używając operacji symbolicznych
    """
    # Zdefiniuj granice całkowania
    a_range = 3.5
    b_range = 6.52968912439344
    
    # Utwórz zmienną symboliczną
    x = sp.Symbol('x')
    
    # Zdefiniuj funkcję do całkowania
    function = x * sp.cos(x)**3
    
    # Oblicz całkę symboliczną
    integral = sp.integrate(function, (x, a_range, b_range))
    
    # Konwertuj wynik na liczbę zmiennoprzecinkową
    exact_result = float(integral)
    
    print(f"Dokładna wartość całki x*cos^3(x): {exact_result}")
    
    # Alternatywnie, użyj scipy.integrate dla numerycznej walidacji
    def func(x):
        return x * np.cos(x)**3
    
    numerical_result, error = integrate.quad(func, a_range, b_range)
    print(f"Numeryczna wartość całki (scipy): {numerical_result}, szacowany błąd: {error}")
    
    # Użyj wartości z quad jako bardziej dokładnej
    final_result = numerical_result
    
    # Zapisz wynik do pliku
    with open('exact_xcos3x.txt', 'w') as f:
        f.write(str(final_result))
    
    return final_result

def calculate_errors(exact_poly, exact_xcos3x):
    """
    Oblicza błędy względne dla różnych metod całkowania
    """
    # Wyniki z programu C++
    rect_poly = -68495.1
    trap_poly = -68495.8
    simp_poly = -68495.3
    
    rect_xcos3x = 4.20251
    trap_xcos3x = 4.2025
    simp_xcos3x = 4.2025
    
    # Błędy względne dla wielomianu
    rect_error_poly = abs((rect_poly - exact_poly) / exact_poly) * 100
    trap_error_poly = abs((trap_poly - exact_poly) / exact_poly) * 100
    simp_error_poly = abs((simp_poly - exact_poly) / exact_poly) * 100
    
    print("\nBłędy względne dla wielomianu:")
    print(f"Metoda prostokątów: {rect_error_poly:.6f}%")
    print(f"Metoda trapezów: {trap_error_poly:.6f}%")
    print(f"Metoda Simpsona: {simp_error_poly:.6f}%")
    
    # Błędy względne dla x*cos^3(x)
    rect_error_xcos3x = abs((rect_xcos3x - exact_xcos3x) / exact_xcos3x) * 100
    trap_error_xcos3x = abs((trap_xcos3x - exact_xcos3x) / exact_xcos3x) * 100
    simp_error_xcos3x = abs((simp_xcos3x - exact_xcos3x) / exact_xcos3x) * 100
    
    print("\nBłędy względne dla funkcji x*cos^3(x):")
    print(f"Metoda prostokątów: {rect_error_xcos3x:.6f}%")
    print(f"Metoda trapezów: {trap_error_xcos3x:.6f}%")
    print(f"Metoda Simpsona: {simp_error_xcos3x:.6f}%")

if __name__ == "__main__":
    print("Obliczanie dokładnych wartości całek...")
    exact_poly = exact_polynomial_integral()
    exact_xcos3x = exact_xcos3x_integral()
    
    print("\nObliczanie błędów względnych...")
    calculate_errors(exact_poly, exact_xcos3x)