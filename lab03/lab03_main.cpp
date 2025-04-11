#include <iostream>
#include <vector>
#include "interpolation.h"
using namespace std;
void load_N_data(const string& filename, vector<double>& xi, vector<double>& fxi) {
    ifstream file(filename);
    string line;
    
    // Wczytaj xi
    getline(file, line);
    stringstream ss_xi(line.substr(line.find("xi:")+4));
    double val;
    while (ss_xi >> val) xi.push_back(val);

    // Wczytaj fxi
    getline(file, line);
    stringstream ss_fxi(line.substr(line.find("f(xi):")+6));
    while (ss_fxi >> val) fxi.push_back(val);
}

int main() {
    // Zadanie 1-4
    vector<double> x_points_H;
    vector<double> a = load_H_data("interpolacja_H_gr_03.txt", x_points_H);
    
    double test_x = 2.0;
    compare_methods(a, test_x);

    // Zadanie 5-7
    vector<double> xi_N, fxi_N;
    load_N_data("interpolacja_N_gr_03.txt", xi_N, fxi_N);

    // Wybierz co piąty punkt
    vector<double> selected_xi, selected_fxi;
    for (size_t i = 0; i < xi_N.size(); i += 5) {
        selected_xi.push_back(xi_N[i]);
        selected_fxi.push_back(fxi_N[i]);
    }

    auto diffs = compute_divided_differences(selected_xi, selected_fxi);
    generate_plot_data(selected_xi, diffs);

    // Zadanie 6
    cout << "Podaj x: ";
    double user_x;
    cin >> user_x;
    cout << "Wartosc: " << newton_polynomial(selected_xi, diffs, user_x) << "\n";

    return 0;
}