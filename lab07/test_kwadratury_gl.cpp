#include <iostream>
#include <cmath>
#include <vector>
#include <functional>

using namespace std;

// Struktura do przechowywania węzłów i wag kwadratury G-L
struct GLNode {
    double point;
    double weight;
};

// Funkcja zwracająca węzły i wagi kwadratury G-L dla zadanej liczby węzłów
vector<GLNode> get_gl_nodes_and_weights(int n) {
    vector<GLNode> nodes;
    
    // Wartości z tabeli w instrukcji laboratoryjnej
    if (n == 1) {
        // Jeden węzeł (n=1)
        nodes.push_back({0.0, 2.0});
    } else if (n == 2) {
        // Dwa węzły (n=2)
        double point = 1.0 / sqrt(3);
        nodes.push_back({-point, 1.0});
        nodes.push_back({point, 1.0});
    } else if (n == 3) {
        // Trzy węzły (n=3)
        nodes.push_back({0.0, 8.0/9.0});
        double point = sqrt(3.0/5.0);
        nodes.push_back({-point, 5.0/9.0});
        nodes.push_back({point, 5.0/9.0});
    } else if (n == 4) {
        // Cztery węzły (n=4)
        double point1 = sqrt((3.0-2.0*sqrt(6.0/5.0))/7.0);
        double point2 = sqrt((3.0+2.0*sqrt(6.0/5.0))/7.0);
        double weight1 = (18.0+sqrt(30.0))/36.0;
        double weight2 = (18.0-sqrt(30.0))/36.0;
        
        nodes.push_back({-point2, weight2});
        nodes.push_back({-point1, weight1});
        nodes.push_back({point1, weight1});
        nodes.push_back({point2, weight2});
    } else {
        cerr << "Błąd: Nieprawidłowa liczba węzłów dla kwadratury G-L. Obsługiwane: 1-4." << endl;
    }
    
    return nodes;
}

// Funkcja do mapowania z przedziału [-1,1] na przedział [a,b]
double map_to_ab(double x, double a, double b) {
    return 0.5 * ((b - a) * x + (b + a));
}

// Kwadratura Gaussa-Legendre'a
double gauss_legendre_quadrature(function<double(double)> f, double a, double b, int n) {
    // Pobierz węzły i wagi
    vector<GLNode> nodes = get_gl_nodes_and_weights(n);
    
    // Oblicz całkę
    double result = 0.0;
    for (const auto& node : nodes) {
        // Mapowanie z przedziału [-1,1] na [a,b]
        double x = map_to_ab(node.point, a, b);
        // Dodanie do sumy
        result += node.weight * f(x);
    }
    
    // Mnożenie przez (b-a)/2
    result *= (b - a) / 2.0;
    
    return result;
}

// Funkcje testowe
double func_x_sin3x(double x) {
    return x * x * pow(sin(x), 3);
}

double func_exp_x2_times_1_minus_x(double x) {
    return exp(x*x) * (1.0 - x);
}

double func_xcos3x(double x) {
    return x * pow(cos(x), 3);
}

// Funkcja wielomianowa stopnia 5
double polynomial(double x) {
    return 5*x*x*x*x*x - 3*x*x*x + 2*x - 7;
}

int main() {
    cout << "Test kwadratury Gaussa-Legendre'a:" << endl;
    cout << "===================================" << endl;
    
    // Test dla wielomianu stopnia 5
    double a_poly = -1.0;
    double b_poly = 1.0;
    cout << "Całkowanie wielomianu 5*x^5 - 3*x^3 + 2*x - 7 w przedziale [" << a_poly << ", " << b_poly << "]:" << endl;
    
    // Dokładna wartość dla wielomianu (można obliczyć analitycznie)
    // ∫(5*x^5 - 3*x^3 + 2*x - 7)dx od -1 do 1 = [5*x^6/6 - 3*x^4/4 + x^2 - 7x]_{-1}^{1} = 0 - 0 + 2 - 0 - (0 - 0 + 2 - 0) = -14
    double exact_poly = -14.0;
    
    for (int n = 1; n <= 4; ++n) {
        double result = gauss_legendre_quadrature(polynomial, a_poly, b_poly, n);
        double error = fabs((result - exact_poly) / exact_poly) * 100.0;
        
        cout << "n = " << n << ": " << result 
             << " (błąd: " << error << "%)" << endl;
    }
    cout << "Dokładna wartość: " << exact_poly << endl;
    cout << endl;
    
    // Test dla funkcji x*sin^3(x)
    double a_sin = 1.0;
    double b_sin = 4.764798248;
    cout << "Całkowanie funkcji x^2*sin^3(x) w przedziale [" << a_sin << ", " << b_sin << "]:" << endl;
    
    // Przybliżona dokładna wartość
    double exact_sin = -7.78662;
    
    for (int n = 1; n <= 4; ++n) {
        double result = gauss_legendre_quadrature(func_x_sin3x, a_sin, b_sin, n);
        double error = fabs((result - exact_sin) / exact_sin) * 100.0;
        
        cout << "n = " << n << ": " << result 
             << " (błąd: " << error << "%)" << endl;
    }
    cout << "Przybliżona dokładna wartość: " << exact_sin << endl;
    cout << endl;
    
    // Test dla funkcji exp(x^2)*(1-x)
    double a_exp = -2.0;
    double b_exp = 3.2087091329;
    cout << "Całkowanie funkcji exp(x^2)*(1-x) w przedziale [" << a_exp << ", " << b_exp << "]:" << endl;
    
    // Przybliżona dokładna wartość
    double exact_exp = 75.3894;
    
    for (int n = 1; n <= 4; ++n) {
        double result = gauss_legendre_quadrature(func_exp_x2_times_1_minus_x, a_exp, b_exp, n);
        double error = fabs((result - exact_exp) / exact_exp) * 100.0;
        
        cout << "n = " << n << ": " << result 
             << " (błąd: " << error << "%)" << endl;
    }
    cout << "Przybliżona dokładna wartość: " << exact_exp << endl;
    
    return 0;
}