#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
#include <string>
using namespace std;
// Zadania 1-4: Postać naturalna i Horner
vector<double> load_H_data(const string& filename, vector<double>& x_points);
double natural_form(const vector<double>& a, double x);
double horner(const vector<double>& a, double x);
void compare_methods(const vector<double>& a, double x);

// Zadanie 5: Interpolacja Newtona
vector<vector<double>> compute_divided_differences(
    const vector<double>& xi, 
    const vector<double>& fxi
);
double newton_polynomial(
    const vector<double>& xi, 
    const vector<vector<double>>& diffs, 
    double x
);
void generate_plot_data(
    const vector<double>& xi, 
    const vector<vector<double>>& diffs,
    const string& filename = "plot_data.csv"
);

#endif