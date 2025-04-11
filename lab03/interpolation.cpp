#include "interpolation.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>
std::vector<double> load_H_data(const std::string& filename, std::vector<double>& x_points) {
    std::ifstream file(filename);
    std::vector<double> coefficients;
    std::string line;

    std::getline(file, line);
    std::stringstream ss(line);
    std::string token;
    while (ss >> token) {
        if (token.find("a") != std::string::npos) {
            coefficients.push_back(std::stod(token.substr(token.find("=") + 1)));
        }
    }

    std::getline(file, line);
    std::stringstream ss_xi(line.substr(line.find("xi:") + 4));
    double val;
    while (ss_xi >> val) x_points.push_back(val);

    return coefficients;
}

double natural_form(const std::vector<double>& a, double x) {
    double result = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] * std::pow(x, i);
    }
    return result;
}

double horner(const std::vector<double>& a, double x) {
    double result = a.back();
    for (int i = a.size() - 2; i >= 0; --i) {
        result = result * x + a[i];
    }
    return result;
}

void compare_methods(const std::vector<double>& a, double x) {
    clock_t start = clock();
    double natural_result = natural_form(a, x);
    double natural_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;

    start = clock();
    double horner_result = horner(a, x);
    double horner_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;

    std::cout << "Porównanie metod:\n"
              << "Naturalna: " << natural_result << " (" << natural_time << "s)\n"
              << "Horner:    " << horner_result << " (" << horner_time << "s)\n";
}

std::vector<std::vector<double>> compute_divided_differences(const std::vector<double>& xi, const std::vector<double>& fxi) {
    size_t n = xi.size();
    std::vector<std::vector<double>> table(n, std::vector<double>(n));

    for (size_t i = 0; i < n; ++i) table[i][0] = fxi[i];
    for (size_t j = 1; j < n; ++j) {
        for (size_t i = 0; i < n - j; ++i) {
            table[i][j] = (table[i + 1][j - 1] - table[i][j - 1]) / (xi[i + j] - xi[i]);
        }
    }
    return table;
}

double newton_polynomial(const std::vector<double>& xi, const std::vector<std::vector<double>>& diffs, double x) {
    double result = diffs[0][0];
    double product = 1.0;
    for (size_t i = 1; i < xi.size(); ++i) {
        product *= (x - xi[i - 1]);
        result += diffs[0][i] * product;
    }
    return result;
}

void generate_plot_data(const std::vector<double>& xi, const std::vector<std::vector<double>>& diffs, const std::string& filename) {
    std::ofstream file(filename);
    double x_start = *std::min_element(xi.begin(), xi.end());
    double x_end = *std::max_element(xi.begin(), xi.end());
    for (double x = x_start; x <= x_end; x += 0.1) {
        file << x << "," << newton_polynomial(xi, diffs, x) << "\n";
    }
}

void generate_horner_plot_data(const std::vector<double>& a, const std::vector<double>& x_range, const std::string& filename) {
    std::ofstream file(filename);
    for (double x : x_range) {
        file << x << "," << horner(a, x) << "\n";
    }
}