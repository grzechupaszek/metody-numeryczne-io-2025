#include "interpolation.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <algorithm>
using namespace std;
// -------------------------------------------------------------------
// Zadania 1-4: Wczytywanie danych i metody obliczeniowe
// -------------------------------------------------------------------
vector<double> load_H_data(const string& filename, vector<double>& x_points) {
    ifstream file(filename);
    vector<double> coefficients;
    string line;

    // Wczytaj współczynniki
    getline(file, line);
    stringstream ss(line);
    string token;
    while (ss >> token) {
        if (token.find("a") != string::npos) {
            coefficients.push_back(stod(token.substr(token.find("=")+1));
        }
    }

    // Wczytaj punkty xi
    getline(file, line);
    stringstream ss_xi(line.substr(line.find("xi:")+4));
    double val;
    while (ss_xi >> val) x_points.push_back(val);

    return coefficients;
}

double natural_form(const vector<double>& a, double x) {
    double result = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] * pow(x, i);
    }
    return result;
}

double horner(const vector<double>& a, double x) {
    double result = a.back();
    for (int i = a.size()-2; i >= 0; --i) {
        result = result * x + a[i];
    }
    return result;
}

void compare_methods(const vector<double>& a, double x) {
    clock_t start = clock();
    double natural_result = natural_form(a, x);
    double natural_time = static_cast<double>(clock() - start)/CLOCKS_PER_SEC;

    start = clock();
    double horner_result = horner(a, x);
    double horner_time = static_cast<double>(clock() - start)/CLOCKS_PER_SEC;

    cout << "Porównanie metod:\n"
              << "Naturalna: " << natural_result << " (" << natural_time << "s)\n"
              << "Horner:    " << horner_result << " (" << horner_time << "s)\n";
}

// -------------------------------------------------------------------
// Zadanie 5: Interpolacja Newtona
// -------------------------------------------------------------------
vector<vector<double>> compute_divided_differences(
    const vector<double>& xi, 
    const vector<double>& fxi
) {
    size_t n = xi.size();
    vector<vector<double>> table(n, vector<double>(n));
    
    for (size_t i = 0; i < n; ++i) table[i][0] = fxi[i];
    
    for (size_t j = 1; j < n; ++j) {
        for (size_t i = 0; i < n - j; ++i) {
            table[i][j] = (table[i+1][j-1] - table[i][j-1]) / (xi[i+j] - xi[i]);
        }
    }
    return table;
}

double newton_polynomial(
    const vector<double>& xi, 
    const vector<vector<double>>& diffs, 
    double x
) {
    double result = diffs[0][0];
    double product = 1.0;
    
    for (size_t i = 1; i < xi.size(); ++i) {
        product *= (x - xi[i-1]);
        result += diffs[0][i] * product;
    }
    return result;
}

void generate_plot_data(
    const vector<double>& xi, 
    const vector<vector<double>>& diffs,
    const string& filename
) {
    ofstream file(filename);
    double x_start = *min_element(xi.begin(), xi.end());
    double x_end = *max_element(xi.begin(), xi.end());
    
    for (double x = x_start; x <= x_end; x += 0.1) {
        file << x << "," << newton_polynomial(xi, diffs, x) << "\n";
    }
}