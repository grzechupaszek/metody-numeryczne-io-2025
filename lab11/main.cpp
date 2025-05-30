#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
#include <fstream>

using namespace std;

class RootFinder {
private:
    double tolerance = 1e-10;
    int maxIterations = 1000;
    
public:
    struct Result {
        double root;
        int iterations;
        vector<double> approximations;
        bool converged;
        double error;
    };
    
    // Funkcje do analizy
    static double function1(double x) {
        if (abs(x - 1) < 1e-15) return 1e15; // unikanie ln(0)
        if (abs(x) < 1e-15) return 1e15; // unikanie dzielenia przez 0
        return log(1 - x) + 1.0 / (x*x + 3);
    }
    
    static double function2(double x) {
        if (abs(x) < 1e-15) return 1e15; // unikanie dzielenia przez 0
        return x*x*x + 30 * sin(x) - 12.0/x - 28;
    }
    
    static double function3(double x) {
        if (abs(x + 2) < 1e-15 || abs(x + 4) < 1e-15) return 1e15;
        return cos(3 * M_PI * x) / (x + 2) - 1.0 / (x + 4);
    }
    
    // Pochodne analityczne
    static double derivative1(double x) {
        if (abs(x - 1) < 1e-15 || abs(x) < 1e-15) return 1e15;
        return -1.0 / (1 - x) - 2*x / pow(x*x + 3, 2);
    }
    
    static double derivative2(double x) {
        if (abs(x) < 1e-15) return 1e15;
        return 3*x*x + 30 * cos(x) + 12.0 / (x*x);
    }
    
    static double derivative3(double x) {
        if (abs(x + 2) < 1e-15 || abs(x + 4) < 1e-15) return 1e15;
        return -3 * M_PI * sin(3 * M_PI * x) / (x + 2) - 
               cos(3 * M_PI * x) / pow(x + 2, 2) + 1.0 / pow(x + 4, 2);
    }
    
    // Pochodna numeryczna
    double numericalDerivative(function<double(double)> f, double x, double h = 1e-8) {
        return (f(x + h) - f(x - h)) / (2 * h);
    }
    
    // Metoda bisekcji
    Result bisection(function<double(double)> f, double a, double b) {
        Result result;
        result.converged = false;
        result.iterations = 0;
        
        if (f(a) * f(b) > 0) {
            cout << "Brak zmiany znaku na końcach przedziału!" << endl;
            return result;
        }
        
        double c;
        for (int i = 0; i < maxIterations; i++) {
            c = (a + b) / 2;
            result.approximations.push_back(c);
            result.iterations++;
            
            if (abs(f(c)) < tolerance || abs(b - a) < tolerance) {
                result.converged = true;
                result.root = c;
                result.error = abs(b - a) / 2;
                break;
            }
            
            if (f(a) * f(c) < 0) {
                b = c;
            } else {
                a = c;
            }
        }
        
        return result;
    }
    
    // Metoda Newtona (z pochodną analityczną)
    Result newtonAnalytical(function<double(double)> f, function<double(double)> df, double x0) {
        Result result;
        result.converged = false;
        result.iterations = 0;
        
        double x = x0;
        double prev_x;
        
        for (int i = 0; i < maxIterations; i++) {
            result.approximations.push_back(x);
            result.iterations++;
            
            double fx = f(x);
            double dfx = df(x);
            
            if (abs(dfx) < 1e-15) {
                cout << "Pochodna zbyt bliska zeru!" << endl;
                break;
            }
            
            prev_x = x;
            x = x - fx / dfx;
            
            if (abs(x - prev_x) < tolerance) {
                result.converged = true;
                result.root = x;
                result.error = abs(x - prev_x);
                break;
            }
        }
        
        return result;
    }
    
    // Metoda Newtona (z pochodną numeryczną)
    Result newtonNumerical(function<double(double)> f, double x0) {
        Result result;
        result.converged = false;
        result.iterations = 0;
        
        double x = x0;
        double prev_x;
        
        for (int i = 0; i < maxIterations; i++) {
            result.approximations.push_back(x);
            result.iterations++;
            
            double fx = f(x);
            double dfx = numericalDerivative(f, x);
            
            if (abs(dfx) < 1e-15) {
                cout << "Pochodna zbyt bliska zeru!" << endl;
                break;
            }
            
            prev_x = x;
            x = x - fx / dfx;
            
            if (abs(x - prev_x) < tolerance) {
                result.converged = true;
                result.root = x;
                result.error = abs(x - prev_x);
                break;
            }
        }
        
        return result;
    }
    
    // Metoda siecznych
    Result secant(function<double(double)> f, double x0, double x1) {
        Result result;
        result.converged = false;
        result.iterations = 0;
        
        double x_prev = x0;
        double x_curr = x1;
        double x_next;
        
        result.approximations.push_back(x_prev);
        result.approximations.push_back(x_curr);
        
        for (int i = 0; i < maxIterations; i++) {
            result.iterations++;
            
            double f_prev = f(x_prev);
            double f_curr = f(x_curr);
            
            if (abs(f_curr - f_prev) < 1e-15) {
                cout << "Różnica wartości funkcji zbyt mała!" << endl;
                break;
            }
            
            x_next = x_curr - f_curr * (x_curr - x_prev) / (f_curr - f_prev);
            result.approximations.push_back(x_next);
            
            if (abs(x_next - x_curr) < tolerance) {
                result.converged = true;
                result.root = x_next;
                result.error = abs(x_next - x_curr);
                break;
            }
            
            x_prev = x_curr;
            x_curr = x_next;
        }
        
        return result;
    }
    
    // Znajdowanie wszystkich pierwiastków w przedziale
    vector<double> findAllRoots(function<double(double)> f, double start, double end, double step = 0.1) {
        vector<double> roots;
        
        for (double x = start; x < end; x += step) {
            if (f(x) * f(x + step) < 0) {
                Result result = bisection(f, x, x + step);
                if (result.converged) {
                    // Sprawdź czy pierwiastek już nie został znaleziony
                    bool found = false;
                    for (double root : roots) {
                        if (abs(root - result.root) < 0.01) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        roots.push_back(result.root);
                    }
                }
            }
        }
        
        return roots;
    }
    
    // Zapisz wyniki do pliku CSV
    void saveToCSV(const string& filename, const vector<double>& data, const string& header) {
        ofstream file(filename);
        file << header << endl;
        for (size_t i = 0; i < data.size(); i++) {
            file << i << "," << fixed << setprecision(10) << data[i] << endl;
        }
        file.close();
    }
};

int main() {
    RootFinder finder;
    
    cout << fixed << setprecision(10);
    cout << "=== LABORATORIUM 11: ROZWIĄZYWANIE RÓWNAŃ NIELINIOWYCH ===" << endl << endl;
    
    // Funkcje do analizy
    vector<function<double(double)>> functions = {
        RootFinder::function1,
        RootFinder::function2,
        RootFinder::function3
    };
    
    vector<function<double(double)>> derivatives = {
        RootFinder::derivative1,
        RootFinder::derivative2,
        RootFinder::derivative3
    };
    
    vector<string> functionNames = {
        "ln(1-x) + 1/(x²+3)",
        "x³ + 30·sin(x) - 12/x - 28",
        "cos(3πx)/(x+2) - 1/(x+4)"
    };
    
    // Znajdź wszystkie pierwiastki dla każdej funkcji
    for (int i = 0; i < 3; i++) {
        cout << "=== FUNKCJA " << (i+1) << ": " << functionNames[i] << " ===" << endl;
        
        vector<double> roots = finder.findAllRoots(functions[i], -3.0, 4.0, 0.05);
        
        cout << "Znalezione pierwiastki w przedziale [-3, 4]:" << endl;
        for (size_t j = 0; j < roots.size(); j++) {
            cout << "Pierwiastek " << (j+1) << ": " << roots[j] << endl;
        }
        cout << endl;
        
        // Analiza pierwszego pierwiastka (jeśli istnieje)
        if (!roots.empty()) {
            double root = roots[0];
            cout << "Analiza pierwiastka: " << root << endl;
            
            // Metoda bisekcji
            RootFinder::Result bisResult = finder.bisection(functions[i], root - 0.5, root + 0.5);
            cout << "Bisekcja: pierwiastek = " << bisResult.root 
                 << ", iteracje = " << bisResult.iterations 
                 << ", błąd = " << bisResult.error << endl;
            
            // Metoda Newtona (analityczna)
            RootFinder::Result newtonAnalResult = finder.newtonAnalytical(functions[i], derivatives[i], root + 0.1);
            cout << "Newton (analityczna): pierwiastek = " << newtonAnalResult.root 
                 << ", iteracje = " << newtonAnalResult.iterations 
                 << ", błąd = " << newtonAnalResult.error << endl;
            
            // Metoda Newtona (numeryczna)
            RootFinder::Result newtonNumResult = finder.newtonNumerical(functions[i], root + 0.1);
            cout << "Newton (numeryczna): pierwiastek = " << newtonNumResult.root 
                 << ", iteracje = " << newtonNumResult.iterations 
                 << ", błąd = " << newtonNumResult.error << endl;
            
            // Metoda siecznych
            RootFinder::Result secantResult = finder.secant(functions[i], root - 0.1, root + 0.1);
            cout << "Sieczne: pierwiastek = " << secantResult.root 
                 << ", iteracje = " << secantResult.iterations 
                 << ", błąd = " << secantResult.error << endl;
            
            // Zapisz przybliżenia do plików CSV
            string filename = "function" + to_string(i+1) + "_newton_approx.csv";
            finder.saveToCSV(filename, newtonAnalResult.approximations, "iteration,approximation");
            
            filename = "function" + to_string(i+1) + "_secant_approx.csv";
            finder.saveToCSV(filename, secantResult.approximations, "iteration,approximation");
            
            cout << endl;
        }
    }
    
    return 0;
}