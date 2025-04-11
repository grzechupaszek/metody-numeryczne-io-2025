#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
#include <string>
#include <algorithm>

// Zadania 1-4
std::vector<double> load_H_data(const std::string& filename, std::vector<double>& x_points);
double natural_form(const std::vector<double>& a, double x);
double horner(const std::vector<double>& a, double x);
void compare_methods(const std::vector<double>& a, double x);

// Zadanie 5
std::vector<std::vector<double>> compute_divided_differences(const std::vector<double>& xi, const std::vector<double>& fxi);
double newton_polynomial(const std::vector<double>& xi, const std::vector<std::vector<double>>& diffs, double x);
void generate_plot_data(const std::vector<double>& xi, const std::vector<std::vector<double>>& diffs, const std::string& filename);
void generate_horner_plot_data(const std::vector<double>& a, const std::vector<double>& x_range, const std::string& filename);

#endif