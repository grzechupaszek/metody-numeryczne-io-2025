#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "interpolation.h"

void load_N_data(const std::string& filename, std::vector<double>& xi, std::vector<double>& fxi) {
    std::ifstream file(filename);
    std::string line;
    
    std::getline(file, line);
    std::stringstream ss_xi(line.substr(line.find("xi:") + 4));
    double val;
    while (ss_xi >> val) xi.push_back(val);

    std::getline(file, line);
    std::stringstream ss_fxi(line.substr(line.find("f(xi):") + 6));
    while (ss_fxi >> val) fxi.push_back(val);
}

int main() {
    // Zadania 1-4
    std::vector<double> x_points_H;
    std::vector<double> a = load_H_data("interpolacja_H_gr_03.txt", x_points_H);
    double test_x = 2.0;
    compare_methods(a, test_x);

    // Zadanie 5
    std::vector<double> xi_N, fxi_N;
    load_N_data("interpolacja_N_gr_03.txt", xi_N, fxi_N);

    // Sortowanie i wybór co 5. punktu
    // Wybierz co piąty punkt
std::vector<double> selected_xi, selected_fxi;
for (size_t i = 0; i < xi_N.size(); i += 10) {
    selected_xi.push_back(xi_N[i]);
    selected_fxi.push_back(fxi_N[i]);
}

// Sortuj węzły i odpowiadające im wartości
std::vector<std::pair<double, double>> paired_data;
for (size_t i = 0; i < selected_xi.size(); ++i) {
    paired_data.emplace_back(selected_xi[i], selected_fxi[i]);
}
std::sort(paired_data.begin(), paired_data.end());

// Rozdziel posortowane dane
selected_xi.clear();
selected_fxi.clear();
for (const auto& pair : paired_data) {
    selected_xi.push_back(pair.first);
    selected_fxi.push_back(pair.second);
}

    auto diffs = compute_divided_differences(selected_xi, selected_fxi);
    generate_plot_data(selected_xi, diffs, "plot_data.csv");

    // Zadanie 6
    std::cout << "Podaj x: ";
    double user_x;
    std::cin >> user_x;
    std::cout << "Wartosc: " << newton_polynomial(selected_xi, diffs, user_x) << "\n";

    // Generowanie danych dla Hornera
    std::vector<double> x_range;
    double x_start = *std::min_element(x_points_H.begin(), x_points_H.end());
    double x_end = *std::max_element(x_points_H.begin(), x_points_H.end());
    for (double x = x_start; x <= x_end; x += 0.1) x_range.push_back(x);
    generate_horner_plot_data(a, x_range, "horner_data.csv");

    return 0;
}