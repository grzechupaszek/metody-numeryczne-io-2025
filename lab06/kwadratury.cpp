#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <string>
#include <cstdlib>
using namespace std;
// Funkcja do obliczania wartości wielomianu za pomocą schematu Hornera
double horner(const vector<double>& a, double x) {
    double result = a[0];
    for (size_t i = 1; i < a.size(); ++i) {
        result = result * x + a[i];
    }
    return result;
}

// Funkcja do obliczania wartości funkcji x*cos^3(x)
double func_xcos3x(double x) {
    return x * pow(cos(x), 3);
}

// Metoda prostokątów
double rectangle_method(const vector<double>& a, double a_range, double b_range, int n) {
    double h = (b_range - a_range) / n;
    double sum = 0.0;
    
    for (int i = 0; i < n; ++i) {
        double x = a_range + (i + 0.5) * h; // Środek prostokąta
        sum += horner(a, x);
    }
    
    return h * sum;
}

// Metoda prostokątów dla funkcji x*cos^3(x)
double rectangle_method_xcos3x(double a_range, double b_range, int n) {
    double h = (b_range - a_range) / n;
    double sum = 0.0;
    
    for (int i = 0; i < n; ++i) {
        double x = a_range + (i + 0.5) * h; // Środek prostokąta
        sum += func_xcos3x(x);
    }
    
    return h * sum;
}

// Metoda trapezów
double trapezoid_method(const vector<double>& a, double a_range, double b_range, int n) {
    double h = (b_range - a_range) / n;
    double sum = 0.5 * (horner(a, a_range) + horner(a, b_range));
    
    for (int i = 1; i < n; ++i) {
        double x = a_range + i * h;
        sum += horner(a, x);
    }
    
    return h * sum;
}

// Metoda trapezów dla funkcji x*cos^3(x)
double trapezoid_method_xcos3x(double a_range, double b_range, int n) {
    double h = (b_range - a_range) / n;
    double sum = 0.5 * (func_xcos3x(a_range) + func_xcos3x(b_range));
    
    for (int i = 1; i < n; ++i) {
        double x = a_range + i * h;
        sum += func_xcos3x(x);
    }
    
    return h * sum;
}

// Metoda Simpsona
double simpson_method(const vector<double>& a, double a_range, double b_range, int n) {
    if (n % 2 != 0) {
        n++; // n musi być parzyste dla metody Simpsona
    }
    
    double h = (b_range - a_range) / n;
    double sum = horner(a, a_range) + horner(a, b_range);
    
    for (int i = 1; i < n; ++i) {
        double x = a_range + i * h;
        sum += horner(a, x) * (i % 2 == 0 ? 2 : 4);
    }
    
    return h * sum / 3.0;
}

// Metoda Simpsona dla funkcji x*cos^3(x)
double simpson_method_xcos3x(double a_range, double b_range, int n) {
    if (n % 2 != 0) {
        n++; // n musi być parzyste dla metody Simpsona
    }
    
    double h = (b_range - a_range) / n;
    double sum = func_xcos3x(a_range) + func_xcos3x(b_range);
    
    for (int i = 1; i < n; ++i) {
        double x = a_range + i * h;
        sum += func_xcos3x(x) * (i % 2 == 0 ? 2 : 4);
    }
    
    return h * sum / 3.0;
}

// Funkcja do testowania zbieżności
void test_convergence(const vector<double>& a, double a_range, double b_range, int max_n, double exact_value, const string& filename = "convergence_data.txt") {
    cout << "Zapisuję dane zbieżności do pliku " << filename << "..." << endl;
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Błąd: Nie można otworzyć pliku " << filename << " do zapisu." << endl;
        return;
    }
    
    file << "n,rectangle,trapezoid,simpson,exact\n";
    
    for (int n = 2; n <= max_n; n *= 2) {
        double rect = rectangle_method(a, a_range, b_range, n);
        double trap = trapezoid_method(a, a_range, b_range, n);
        double simp = simpson_method(a, a_range, b_range, n);
        
        cout << "n = " << setw(6) << n 
                  << ", Prostokąty: " << setw(12) << rect 
                  << ", Trapezy: " << setw(12) << trap 
                  << ", Simpson: " << setw(12) << simp << endl;
        
        file << n << "," << rect << "," << trap << "," << simp << "," << exact_value << "\n";
    }
    
    file.close();
    cout << "Dane zbieżności zapisane do pliku " << filename << endl;
    
    // Zapisz dokładną wartość do oddzielnego pliku dla Pythona
    ofstream exact_file("exact_value.txt");
    if (exact_file.is_open()) {
        exact_file << exact_value;
        exact_file.close();
    }
}

// Funkcja do pomiaru czasu wykonania
template<typename Func, typename... Args>
double measure_time(Func&& func, Args&&... args) {
    auto start = chrono::high_resolution_clock::now();
    func(forward<Args>(args)...); // Wywołaj funkcję, ale zignoruj wynik (unikamy ostrzeżenia)
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    return duration.count();
}

// Funkcja uruchamiająca skrypt Pythona do obliczania dokładnych wartości całek
void run_exact_integration() {
    cout << "Uruchamiam skrypt Python do obliczenia dokładnych wartości całek..." << endl;
    int result = system("python exact_integration.py");
    
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
    // Wczytanie danych z pliku
    ifstream file("dane.txt");
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku dane.txt" << endl;
        return 1;
    }
    
    int N;
    file >> N;
    
    vector<double> a(N + 1);
    for (int i = 0; i <= N; ++i) {
        file >> a[i];
    }
    
    double a_range, b_range;
    file >> a_range >> b_range;
    file.close();
    
    // Wyświetlenie wczytanych danych
    cout << "Stopień wielomianu N: " << N << endl;
    cout << "Współczynniki wielomianu a_i: ";
    for (double coeff : a) {
        cout << coeff << " ";
    }
    cout << endl;
    cout << "Przedział całkowania [a, b]: [" << a_range << ", " << b_range << "]" << endl;
    cout << endl;
    
    // Uruchom skrypt Pythona do obliczenia dokładnych wartości całek
    run_exact_integration();
    
    // Odczytaj dokładne wartości z plików wygenerowanych przez Pythona
    double exact_poly = read_exact_value("exact_poly.txt");
    double exact_xcos3x = read_exact_value("exact_xcos3x.txt");
    
    cout << "Dokładna wartość całki wielomianu: " << exact_poly << endl;
    cout << "Dokładna wartość całki x*cos^3(x): " << exact_xcos3x << endl;
    cout << endl;
    
    // 2. Obliczanie całki wielomianu różnymi metodami
    cout << "=== Całkowanie wielomianu ===" << endl;
    
    const int n = 1000; // Liczba podziałów
    
    // Wyniki dla wielomianu
    double rect_result = rectangle_method(a, a_range, b_range, n);
    double trap_result = trapezoid_method(a, a_range, b_range, n);
    double simp_result = simpson_method(a, a_range, b_range, n);
    
    cout << "Metoda prostokątów: " << rect_result << endl;
    cout << "Metoda trapezów: " << trap_result << endl;
    cout << "Metoda Simpsona: " << simp_result << endl;
    cout << "Wartość dokładna: " << exact_poly << endl;
    cout << endl;
    
    // 2.2 Testowanie zbieżności algorytmu (wykresy zapisywane do pliku)
    cout << "=== Testowanie zbieżności ===" << endl;
    test_convergence(a, a_range, b_range, 4096, exact_poly);
    cout << endl;
    
    // 3. Obliczanie całki x*cos^3(x) i czasu obliczeń
    cout << "=== Całkowanie funkcji x*cos^3(x) w przedziale [3.5, 6.52968912439344] ===" << endl;
    
    double a_xcos3x = 3.5;
    double b_xcos3x = 6.52968912439344;
    
    // Pomiar czasu dla różnych metod
    double rect_time = measure_time(rectangle_method_xcos3x, a_xcos3x, b_xcos3x, n);
    double trap_time = measure_time(trapezoid_method_xcos3x, a_xcos3x, b_xcos3x, n);
    double simp_time = measure_time(simpson_method_xcos3x, a_xcos3x, b_xcos3x, n);
    
    // Wyniki dla x*cos^3(x)
    double rect_xcos3x = rectangle_method_xcos3x(a_xcos3x, b_xcos3x, n);
    double trap_xcos3x = trapezoid_method_xcos3x(a_xcos3x, b_xcos3x, n);
    double simp_xcos3x = simpson_method_xcos3x(a_xcos3x, b_xcos3x, n);
    
    cout << "Metoda prostokątów: " << rect_xcos3x << " (czas: " << rect_time << " ms)" << endl;
    cout << "Metoda trapezów: " << trap_xcos3x << " (czas: " << trap_time << " ms)" << endl;
    cout << "Metoda Simpsona: " << simp_xcos3x << " (czas: " << simp_time << " ms)" << endl;
    cout << "Wartość dokładna: " << exact_xcos3x << endl;
    cout << endl;
    
    // 4. Obliczanie błędów
    cout << "=== Błędy względne ===" << endl;
    cout << "Dla wielomianu:" << endl;
    cout << "Metoda prostokątów: " << abs((rect_result - exact_poly) / exact_poly) * 100 << "%" << endl;
    cout << "Metoda trapezów: " << abs((trap_result - exact_poly) / exact_poly) * 100 << "%" << endl;
    cout << "Metoda Simpsona: " << abs((simp_result - exact_poly) / exact_poly) * 100 << "%" << endl;
    cout << endl;
    
    cout << "Dla funkcji x*cos^3(x):" << endl;
    cout << "Metoda prostokątów: " << abs((rect_xcos3x - exact_xcos3x) / exact_xcos3x) * 100 << "%" << endl;
    cout << "Metoda trapezów: " << abs((trap_xcos3x - exact_xcos3x) / exact_xcos3x) * 100 << "%" << endl;
    cout << "Metoda Simpsona: " << abs((simp_xcos3x - exact_xcos3x) / exact_xcos3x) * 100 << "%" << endl;
    
    // Uruchom skrypt do rysowania wykresów
    cout << "\nUruchamiam skrypt Python do rysowania wykresów..." << endl;
    system("python plot_convergence.py");
    
    return 0;
}