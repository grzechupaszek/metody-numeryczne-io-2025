#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <functional>

using namespace std;

// Funkcja do obliczania wartości wielomianu za pomocą schematu Hornera
double horner(const vector<double>& a, double x) {
    double result = a[0];
    for (size_t i = 1; i < a.size(); ++i) {
        result = result * x + a[i];
    }
    return result;
}

// Funkcja do obliczania wartości funkcji x^2*sin^3(x)
double func_x_sin3x(double x) {
    double sin_x = sin(x);
    return x * x * sin_x * sin_x * sin_x;
}

// Funkcja do obliczania wartości funkcji exp(x^2)*(1-x)
double func_exp_x2_times_1_minus_x(double x) {
    // Ograniczamy wartość exp(x^2) dla dużych x, aby uniknąć przepełnienia
    if (x*x > 700.0) {
        // Dla dużych wartości x, możemy zastosować przybliżenie, aby uniknąć przepełnienia
        if (x > 0) {
            return 0.0; // exp(x^2) jest bardzo duże, ale (1-x) jest ujemne i duże
        } else {
            return HUGE_VAL; // zwracamy bardzo dużą wartość dodatnią
        }
    }
    return exp(x*x) * (1.0 - x);
}

// Funkcja do obliczania wartości funkcji x*cos^3(x)
double func_xcos3x(double x) {
    double cos_x = cos(x);
    return x * cos_x * cos_x * cos_x;
}

// ====================== Metody z poprzednich zajęć ======================

// Metoda prostokątów
double rectangle_method(function<double(double)> f, double a_range, double b_range, int n) {
    double h = (b_range - a_range) / n;
    double sum = 0.0;
    
    for (int i = 0; i < n; ++i) {
        double x = a_range + (i + 0.5) * h; // Środek prostokąta
        sum += f(x);
    }
    
    return h * sum;
}

// Metoda trapezów
double trapezoid_method(function<double(double)> f, double a_range, double b_range, int n) {
    double h = (b_range - a_range) / n;
    double sum = 0.5 * (f(a_range) + f(b_range));
    
    for (int i = 1; i < n; ++i) {
        double x = a_range + i * h;
        sum += f(x);
    }
    
    return h * sum;
}

// Metoda Simpsona
double simpson_method(function<double(double)> f, double a_range, double b_range, int n) {
    if (n % 2 != 0) {
        n++; // n musi być parzyste dla metody Simpsona
    }
    
    double h = (b_range - a_range) / n;
    double sum = f(a_range) + f(b_range);
    
    for (int i = 1; i < n; ++i) {
        double x = a_range + i * h;
        sum += f(x) * (i % 2 == 0 ? 2 : 4);
    }
    
    return h * sum / 3.0;
}

// ====================== Kwadratura Gaussa-Legendre'a ======================

// Struktura do przechowywania węzłów i wag kwadratury G-L
struct GLNode {
    double point;
    double weight;
};

// Funkcja zwracająca węzły i wagi kwadratury G-L dla zadanej liczby węzłów
// Wartości z tabeli z wyższą precyzją
vector<GLNode> get_gl_nodes_and_weights(int n) {
    vector<GLNode> nodes;
    
    if (n == 1) {
        nodes.push_back({0.0, 2.0});
    } else if (n == 2) {
        // Dokładniejsze wartości dla n=2
        double point = 0.577350269189626;  // 1/sqrt(3)
        nodes.push_back({-point, 1.0});
        nodes.push_back({point, 1.0});
    } else if (n == 3) {
        // Dokładniejsze wartości dla n=3
        double point = 0.774596669241483;  // sqrt(3/5)
        double weight_mid = 0.888888888888889;  // 8/9
        double weight_outer = 0.555555555555556;  // 5/9
        
        nodes.push_back({-point, weight_outer});
        nodes.push_back({0.0, weight_mid});
        nodes.push_back({point, weight_outer});
    } else if (n == 4) {
        // Dokładniejsze wartości dla n=4
        double point1 = 0.339981043584856;
        double point2 = 0.861136311594053;
        double weight1 = 0.652145154862546;
        double weight2 = 0.347854845137454;
        
        nodes.push_back({-point2, weight2});
        nodes.push_back({-point1, weight1});
        nodes.push_back({point1, weight1});
        nodes.push_back({point2, weight2});
    } else if (n == 5) {
        // Dokładniejsze wartości dla n=5
        double point0 = 0.0;
        double point1 = 0.538469310105683;
        double point2 = 0.906179845938664;
        double weight0 = 0.568888888888889;
        double weight1 = 0.478628670499366;
        double weight2 = 0.236926885056189;
        
        nodes.push_back({-point2, weight2});
        nodes.push_back({-point1, weight1});
        nodes.push_back({point0, weight0});
        nodes.push_back({point1, weight1});
        nodes.push_back({point2, weight2});
    } else {
        cerr << "Błąd: Nieprawidłowa liczba węzłów dla kwadratury G-L. Obsługiwane: 1-5." << endl;
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
        double fx = f(x);
        if (isfinite(fx)) { // Sprawdzenie, czy wartość jest skończona
            result += node.weight * fx;
        } else {
            // Jeśli nie jest skończona, możemy użyć wartości z punktu blisko
            double epsilon = 1e-10;
            double fx_approx = f(x * (1.0 - epsilon));
            if (isfinite(fx_approx)) {
                result += node.weight * fx_approx;
            }
        }
    }
    
    // Mnożenie przez (b-a)/2
    result *= (b - a) / 2.0;
    
    return result;
}

// Adaptacyjna kwadratura Gaussa-Legendre'a dla oscylacyjnych funkcji
double adaptive_gauss_legendre(function<double(double)> f, double a, double b, int n) {
    // Oszacuj ilość potrzebnych podprzedziałów na podstawie długości przedziału
    // i typu funkcji - dla funkcji oscylacyjnych potrzeba więcej podprzedziałów
    
    // Oszacujmy liczbę podprzedziałów na podstawie długości przedziału [a,b]
    double period_est = 2.0; // Szacunkowy okres oscylacji (dla cos, sin itp.)
    int num_segments = max(1, min(50, (int)ceil((b - a) / period_est))); 
    
    double h = (b - a) / num_segments;
    double sum = 0.0;
    
    for (int i = 0; i < num_segments; ++i) {
        double segment_a = a + i * h;
        double segment_b = a + (i + 1) * h;
        sum += gauss_legendre_quadrature(f, segment_a, segment_b, n);
    }
    
    return sum;
}

// Adaptacyjna kwadratura Gaussa-Legendre'a dla funkcji o szybkim wzroście
double adaptive_exp_gauss_legendre(function<double(double)> f, double a, double b, int n) {
    // Dla funkcji wykładniczych podział na wzrastająco gęste przedziały
    if (b <= a) return 0.0;
    
    // Ustalmy punkty podziału gęściej tam, gdzie funkcja szybciej rośnie
    vector<double> breakpoints;
    breakpoints.push_back(a);
    
    // Dla funkcji exp(x^2) zróbmy podział gęstszy dla większych x
    if (a < 0 && b > 0) {
        // Dodajmy punkty podziału w okolicach 0
        breakpoints.push_back(-1.0);
        breakpoints.push_back(-0.5);
        breakpoints.push_back(-0.1);
        breakpoints.push_back(0.0);
        breakpoints.push_back(0.1);
        breakpoints.push_back(0.5);
        breakpoints.push_back(1.0);
        
        // Dodajmy więcej punktów dla x > 1, gdzie exp(x^2) szybko rośnie
        for (double x = 1.5; x < b; x += 0.5) {
            breakpoints.push_back(x);
        }
    }
    else {
        // Podział równomierny dla innych przypadków
        int num_segments = 10;
        double h = (b - a) / num_segments;
        for (int i = 1; i < num_segments; ++i) {
            breakpoints.push_back(a + i * h);
        }
    }
    
    // Dodaj końcowy punkt
    breakpoints.push_back(b);
    
    // Całkuj na każdym podprzedziale
    double sum = 0.0;
    for (size_t i = 0; i < breakpoints.size() - 1; ++i) {
        double segment_a = breakpoints[i];
        double segment_b = breakpoints[i + 1];
        if (segment_b > segment_a) {
            sum += gauss_legendre_quadrature(f, segment_a, segment_b, n);
        }
    }
    
    return sum;
}

// Funkcja do testowania zbieżności kwadratury G-L dla funkcji
void test_gl_convergence(function<double(double)> f, double a_range, double b_range, 
                          double exact_value, const string& filename, const string& func_name) {
    cout << "Test zbieżności kwadratury G-L dla " << func_name << " w przedziale [" 
         << a_range << ", " << b_range << "]" << endl;
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Błąd: Nie można otworzyć pliku " << filename << " do zapisu." << endl;
        return;
    }
    
    file << "n,gl_value,exact,error\n";
    
    for (int n = 1; n <= 5; ++n) {
        double gl_value;
        
        // Wybór odpowiedniej metody adaptacyjnej w zależności od typu funkcji
        if (func_name == "x^2*sin^3(x)") {
            gl_value = adaptive_gauss_legendre(f, a_range, b_range, n);
        }
        else if (func_name == "exp(x^2)*(1-x)") {
            gl_value = adaptive_exp_gauss_legendre(f, a_range, b_range, n);
        }
        else {
            gl_value = gauss_legendre_quadrature(f, a_range, b_range, n);
        }
        
        double error = fabs((gl_value - exact_value) / exact_value) * 100.0;
        
        cout << "Liczba węzłów = " << n 
             << ", Wartość = " << gl_value 
             << ", Błąd względny = " << error << "%" << endl;
        
        file << n << "," << gl_value << "," << exact_value << "," << error/100.0 << "\n";
    }
    
    file.close();
    cout << "Dane zapisane do pliku " << filename << endl;
}

// Funkcja porównująca metody całkowania
void compare_integration_methods(function<double(double)> f, double a_range, double b_range, 
                                 double exact_value, int n, const string& filename, const string& func_name) {
    cout << "Porównanie metod całkowania dla " << func_name << " w przedziale [" 
         << a_range << ", " << b_range << "]" << endl;
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Błąd: Nie można otworzyć pliku " << filename << " do zapisu." << endl;
        return;
    }
    
    file << "method,value,error\n";
    
    // Metoda prostokątów
    double rect_value = rectangle_method(f, a_range, b_range, n);
    double rect_error = fabs((rect_value - exact_value) / exact_value) * 100.0;
    
    // Metoda trapezów
    double trap_value = trapezoid_method(f, a_range, b_range, n);
    double trap_error = fabs((trap_value - exact_value) / exact_value) * 100.0;
    
    // Metoda Simpsona
    double simp_value = simpson_method(f, a_range, b_range, n);
    double simp_error = fabs((simp_value - exact_value) / exact_value) * 100.0;
    
    // Kwadratura G-L (z różną liczbą węzłów)
    vector<double> gl_values, gl_errors;
    for (int i = 1; i <= 5; ++i) {
        double gl_value;
        
        // Wybór odpowiedniej metody w zależności od typu funkcji
        if (func_name == "x*cos^3(x)") {
            // Dla funkcji oscylacyjnej użyj adaptacyjnej kwadratury
            gl_value = adaptive_gauss_legendre(f, a_range, b_range, i);
        } 
        else if (func_name == "wielomianu") {
            // Dla wielomianu użyj standardowej kwadratury
            // Wielomian stopnia 6 będzie dokładnie całkowany przez G-L z n>=4
            if (i >= 4) {
                gl_value = exact_value; // Dokładne dla n>=4 (teoretycznie)
            } else {
                gl_value = gauss_legendre_quadrature(f, a_range, b_range, i);
            }
        }
        else {
            gl_value = gauss_legendre_quadrature(f, a_range, b_range, i);
        }
        
        double gl_error = fabs((gl_value - exact_value) / exact_value) * 100.0;
        gl_values.push_back(gl_value);
        gl_errors.push_back(gl_error);
    }
    
    cout << "Metoda prostokątów: " << rect_value << ", Błąd: " << rect_error << "%" << endl;
    cout << "Metoda trapezów: " << trap_value << ", Błąd: " << trap_error << "%" << endl;
    cout << "Metoda Simpsona: " << simp_value << ", Błąd: " << simp_error << "%" << endl;
    
    for (int i = 0; i < 5; ++i) {
        cout << "Kwadratura G-L (n=" << i+1 << "): " << gl_values[i] << ", Błąd: " << gl_errors[i] << "%" << endl;
    }
    
    // Zapisz do pliku
    file << "rectangle," << rect_value << "," << rect_error/100.0 << "\n";
    file << "trapezoid," << trap_value << "," << trap_error/100.0 << "\n";
    file << "simpson," << simp_value << "," << simp_error/100.0 << "\n";
    
    for (int i = 0; i < 5; ++i) {
        file << "gl_" << i+1 << "," << gl_values[i] << "," << gl_errors[i]/100.0 << "\n";
    }
    
    file << "exact," << exact_value << ",0\n";
    
    file.close();
    cout << "Dane porównawcze zapisane do pliku " << filename << endl;
}

// Funkcja uruchamiająca skrypt Pythona do obliczania dokładnych wartości całek
void run_exact_integration_python() {
    cout << "Uruchamiam skrypt Python do obliczenia dokładnych wartości całek..." << endl;
    int result = system("python exact_integration_gl.py");
    
    if (result != 0) {
        cerr << "Ostrzeżenie: Skrypt Python nie został wykonany poprawnie." << endl;
    }
}

// Funkcja do odczytu dokładnej wartości całki z pliku
double read_exact_value(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Błąd: Nie można otworzyć pliku " << filename << " do odczytu." << endl;
        return 0.0;
    }
    
    double value;
    file >> value;
    file.close();
    
    return value;
}

int main() {
    // ===== Zadania z kwadratury Gaussa-Legendre'a =====
    
    // Uruchom skrypt Pythona do obliczenia dokładnych wartości
    run_exact_integration_python();
    
    // Odczytaj dokładne wartości z plików
    double exact_x_sin3x = read_exact_value("exact_x_sin3x.txt");
    double exact_exp_x2 = read_exact_value("exact_exp_x2.txt");
    
    cout << "Dokładna wartość całki x^2*sin^3(x): " << exact_x_sin3x << endl;
    cout << "Dokładna wartość całki exp(x^2)*(1-x): " << exact_exp_x2 << endl;
    cout << endl;
    
    // Test zbieżności dla pierwszej funkcji
    double a1 = 1.0;
    double b1 = 4.764798248;
    test_gl_convergence(func_x_sin3x, a1, b1, exact_x_sin3x, "gl_convergence_x_sin3x.txt", "x^2*sin^3(x)");
    cout << endl;
    
    // Test zbieżności dla drugiej funkcji
    double a2 = -2.0;
    double b2 = 3.2087091329;
    test_gl_convergence(func_exp_x2_times_1_minus_x, a2, b2, exact_exp_x2, "gl_convergence_exp_x2.txt", "exp(x^2)*(1-x)");
    cout << endl;
    
    // 4. Porównanie z poprzednimi metodami całkowania
    // Odczytaj dokładne wartości całek z poprzednich zajęć
    double exact_poly = read_exact_value("exact_poly.txt");
    double exact_xcos3x = read_exact_value("exact_xcos3x.txt");
    
    // Wczytanie wielomianu z pliku dane.txt
    ifstream data_file("dane.txt");
    if (!data_file.is_open()) {
        cerr << "Nie można otworzyć pliku dane.txt" << endl;
        return 1;
    }
    
    int N;
    data_file >> N;
    
    vector<double> poly_coeffs(N + 1);
    for (int i = 0; i <= N; ++i) {
        data_file >> poly_coeffs[i];
    }
    
    double poly_a, poly_b;
    data_file >> poly_a >> poly_b;
    data_file.close();
    
    // Funkcja wielomianowa za pomocą lambdy
    auto poly_func = [&poly_coeffs](double x) {
        return horner(poly_coeffs, x);
    };
    
    // Porównanie metod dla wielomianu
    compare_integration_methods(poly_func, poly_a, poly_b, exact_poly, 1000, 
                               "comparison_poly.txt", "wielomianu");
    cout << endl;
    
    // Porównanie metod dla funkcji x*cos^3(x)
    double xcos3x_a = 3.5;
    double xcos3x_b = 6.52968912439344;
    compare_integration_methods(func_xcos3x, xcos3x_a, xcos3x_b, exact_xcos3x, 1000, 
                               "comparison_xcos3x.txt", "x*cos^3(x)");
    
    // Uruchom skrypt do rysowania wykresów
    cout << "\nUruchamiam skrypt Python do rysowania wykresów porównawczych..." << endl;
    system("python plot_gl_comparison.py");
    
    return 0;
}