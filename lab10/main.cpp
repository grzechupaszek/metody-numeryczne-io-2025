#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;
// Parametry zadania
const double T_AMBIENT = 20.0;      // Temperatura otoczenia [°C]
const double T_INITIAL = 100.0;     // Temperatura początkowa kuli [°C]
const double K = 0.001;             // Współczynnik chłodzenia [1/s]
const double TIME_END = 2374.0;     // Czas końcowy [s]

// Rozwiązanie analityczne
double analyticalSolution(double t) {
    return T_AMBIENT + (T_INITIAL - T_AMBIENT) * exp(-K * t);
}

// Równanie różniczkowe dT/dt = -k(T - T_ambient)
double f(double t, double T) {
    return -K * (T - T_AMBIENT);
}

// Metoda Heuna
vector<pair<double, double>> heunMethod(double h) {
    vector<pair<double, double>> results;
    double t = 0.0;
    double T = T_INITIAL;
    
    while (t <= TIME_END) {
        results.push_back({t, T});
        
        double T_tilde = T + h * f(t, T);
        T = T + (h / 2.0) * (f(t, T) + f(t + h, T_tilde));
        t += h;
    }
    
    return results;
}

// Metoda punktu środkowego
vector<pair<double, double>> midpointMethod(double h) {
    vector<pair<double, double>> results;
    double t = 0.0;
    double T = T_INITIAL;
    
    while (t <= TIME_END) {
        results.push_back({t, T});
        
        double T_mid = T + (h / 2.0) * f(t, T);
        T = T + h * f(t + h/2.0, T_mid);
        t += h;
    }
    
    return results;
}

// Metoda Rungego-Kutty 4 rzędu
vector<pair<double, double>> rungeKutta4Method(double h) {
    vector<pair<double, double>> results;
    double t = 0.0;
    double T = T_INITIAL;
    
    while (t <= TIME_END) {
        results.push_back({t, T});
        
        double k1 = h * f(t, T);
        double k2 = h * f(t + h/2.0, T + k1/2.0);
        double k3 = h * f(t + h/2.0, T + k2/2.0);
        double k4 = h * f(t + h, T + k3);
        
        T = T + (k1 + 2*k2 + 2*k3 + k4) / 6.0;
        t += h;
    }
    
    return results;
}

// Obliczanie średniego błędu kwadratowego
double calculateMSE(const vector<pair<double, double>>& numerical) {
    double mse = 0.0;
    int n = 0;
    
    for (const auto& point : numerical) {
        double t = point.first;
        double T_num = point.second;
        double T_ana = analyticalSolution(t);
        mse += pow(T_num - T_ana, 2);
        n++;
    }
    
    return sqrt(mse / n);
}

// Zapisywanie wyników do pliku
void saveResults(const string& filename, 
                 const vector<pair<double, double>>& data) {
    ofstream file(filename);
    file << fixed << setprecision(6);
    
    for (const auto& point : data) {
        file << point.first << " " << point.second << endl;
    }
    
    file.close();
}

// Zapisywanie błędów MSE
void saveMSE(const string& filename,
             const vector<double>& steps,
             const vector<double>& mse_heun,
             const vector<double>& mse_mid,
             const vector<double>& mse_rk4) {
    ofstream file(filename);
    file << fixed << setprecision(10);
    
    file << "h Heun Midpoint RK4" << endl;
    for (size_t i = 0; i < steps.size(); i++) {
        file << steps[i] << " " << mse_heun[i] << " " 
             << mse_mid[i] << " " << mse_rk4[i] << endl;
    }
    
    file.close();
}

int main() {
    // Różne kroki czasowe
    vector<double> time_steps = {10.0, 5.0, 2.0, 1.0, 0.5};
    
    vector<double> mse_heun_vec, mse_mid_vec, mse_rk4_vec;
    
    cout << "Metody numeryczne - Lab 10" << endl;
    cout << "=========================" << endl;
    cout << "Czas symulacji: " << TIME_END << " s" << endl;
    cout << "Temperatura początkowa: " << T_INITIAL << " °C" << endl;
    cout << "Temperatura otoczenia: " << T_AMBIENT << " °C" << endl;
    cout << "Współczynnik chłodzenia: " << K << " 1/s" << endl;
    cout << endl;
    
    // Obliczenia dla różnych kroków czasowych
    for (size_t i = 0; i < time_steps.size(); i++) {
        double h = time_steps[i];
        
        cout << "Krok czasowy h = " << h << " s:" << endl;
        
        // Metoda Heuna
        auto heun_results = heunMethod(h);
        double mse_heun = calculateMSE(heun_results);
        mse_heun_vec.push_back(mse_heun);
        stringstream heun_file;
        heun_file << "heun_h" << fixed << setprecision(1) << h << ".dat";
        saveResults(heun_file.str(), heun_results);
        cout << "  Heun - Temperatura końcowa: " 
                  << heun_results.back().second << " °C, MSE: " << mse_heun << endl;
        
        // Metoda punktu środkowego
        auto mid_results = midpointMethod(h);
        double mse_mid = calculateMSE(mid_results);
        mse_mid_vec.push_back(mse_mid);
        stringstream mid_file;
        mid_file << "midpoint_h" << fixed << setprecision(1) << h << ".dat";
        saveResults(mid_file.str(), mid_results);
        cout << "  Punkt środkowy - Temperatura końcowa: " 
                  << mid_results.back().second << " °C, MSE: " << mse_mid << endl;
        
        // Metoda RK4
        auto rk4_results = rungeKutta4Method(h);
        double mse_rk4 = calculateMSE(rk4_results);
        mse_rk4_vec.push_back(mse_rk4);
        stringstream rk4_file;
        rk4_file << "rk4_h" << fixed << setprecision(1) << h << ".dat";
        saveResults(rk4_file.str(), rk4_results);
        cout << "  RK4 - Temperatura końcowa: " 
                  << rk4_results.back().second << " °C, MSE: " << mse_rk4 << endl;
        
        cout << endl;
    }
    
    // Zapisywanie MSE do pliku
    saveMSE("mse_results.dat", time_steps, mse_heun_vec, mse_mid_vec, mse_rk4_vec);
    
    // Generowanie rozwiązania analitycznego
    ofstream analytical_file("analytical.dat");
    for (double t = 0; t <= TIME_END; t += 1.0) {
        analytical_file << t << " " << analyticalSolution(t) << endl;
    }
    analytical_file.close();
    
    cout << "Temperatura analityczna po " << TIME_END << " s: " 
              << analyticalSolution(TIME_END) << " °C" << endl;
    
    // Wywołanie skryptu Python do generowania wykresów
    cout << "\nGenerowanie wykresów..." << endl;
    system("python3 generate_plots.py");
    
    return 0;
}