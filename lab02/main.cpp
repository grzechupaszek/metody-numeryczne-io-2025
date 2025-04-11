#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

// Funkcja interpolacji Lagrange’a
double lagrangeInterpolation(const vector<double>& nodes, const vector<double>& values, double x) {
    double result = 0.0;
    int n = nodes.size();
    for (int i = 0; i < n; i++) {
        double term = values[i];
        for (int j = 0; j < n; j++) {
            if (j != i)
                term *= (x - nodes[j]) / (nodes[i] - nodes[j]);
        }
        result += term;
    }
    return result;
}
//wczytanie danych z pliku
bool readData(const string& filename, vector<double>& xs, vector<double>& ys) {
    ifstream infile(filename);
    if (!infile) {
        cout << "Nie mogę otworzyć pliku: " << filename << endl;
        return false;
    }
    string line;
    // Wczytanie współrzędnych x
    if(getline(infile, line)) {
        istringstream iss(line);
        string label;
        iss >> label; // pomijamy "xi:"
        double val;
        while (iss >> val) {
            xs.push_back(val);
        }
    }
    // Wczytanie wartości f(x)
    if(getline(infile, line)) {
        istringstream iss(line);
        string label;
        iss >> label; // pomijamy "f(xi):"
        double val;
        while (iss >> val) {
            ys.push_back(val);
        }
    }
    if (xs.size() != ys.size() || xs.empty()) {
        cerr << "Błąd: niezgodna liczba danych lub puste dane." << endl;
        return false;
    }
    return true;
}

// Funkcja wybierająca co 'step' punkt jako węzeł interpolacji.
void selectNodes(const vector<double>& xs, const vector<double>& ys, int step,
                 vector<double>& nodes, vector<double>& nodeValues) {
    nodes.clear();
    nodeValues.clear();
    for (size_t i = 0; i < xs.size(); i++) {
        if (i % step == 0) {
            nodes.push_back(xs[i]);
            nodeValues.push_back(ys[i]);
        }
    }
}

// Funkcja obliczająca średni błąd kwadratowy dla punktów, które nie są węzłami.
double computeMSE(const vector<double>& xs, const vector<double>& ys, 
                    const vector<double>& nodes, const vector<double>& nodeValues) {
    double errorSum = 0.0;
    int count = 0;
    for (size_t i = 0; i < xs.size(); i++) {
        bool isNode = false;
        for (double xnode : nodes) {
            if (fabs(xs[i] - xnode) < 1e-6) {
                isNode = true;
                break;
            }
        }
        if (!isNode) {
            double interpVal = lagrangeInterpolation(nodes, nodeValues, xs[i]);
            double diff = interpVal - ys[i];
            errorSum += diff * diff;
            count++;
        }
    }
    return (count > 0) ? errorSum / count : 0.0;
}

int main() {
    vector<double> xs, ys;
    string filename = "interpolacja_gr_3_INO.txt";
    if (!readData(filename, xs, ys))
        return 1;

    // Wybieramy co 5-ty punkt jako węzeł interpolacji.
    vector<double> nodes, nodeValues;
    int step = 5;
    selectNodes(xs, ys, step, nodes, nodeValues);
    cout << "Wybrano " << nodes.size() << " węzłów do interpolacji." << endl;

    // Pobranie punktu od użytkownika, dla którego obliczymy interpolację.
    double x_input;
    cout << "Podaj wartość x, dla której chcesz obliczyć interpolację: ";
    cin >> x_input;
    double interpValue = lagrangeInterpolation(nodes, nodeValues, x_input);
    cout << "Interpolowana wartość w punkcie x = " << x_input << " wynosi: " << interpValue << endl;

    // Obliczenie średniego błędu kwadratowego (MSE) dla punktów spoza węzłów.
    double mse = computeMSE(xs, ys, nodes, nodeValues);
    cout << "Średni błąd kwadratowy (dla punktów spoza węzłów) wynosi: " << mse << endl;

    // Zapis wyników interpolacji dla szeregu punktów do pliku tekstowego.
    ofstream outfile("output.txt");
    if (!outfile) {
        cerr << "Nie mogę otworzyć pliku output.txt do zapisu." << endl;
        return 1;
    }
    outfile << "x,interpolated_y" << endl;
    // Ustalamy zakres na podstawie najmniejszego i największego x z danych.
    double x_min = xs.front();
    double x_max = xs.back();
    int numPoints = 1000;
    double dx = (x_max - x_min) / (numPoints - 1);
    for (int i = 0; i < numPoints; i++) {
        double x_val = x_min + i * dx;
        double y_val = lagrangeInterpolation(nodes, nodeValues, x_val);
        outfile << x_val << "," << y_val << endl;
    }
    outfile.close();
    cout << "Wyniki interpolacji zapisane do pliku output.txt." << endl;
    // ...

    int bestNodes = 0;
    double bestMSE = numeric_limits<double>::max();
    int total = xs.size();

    // Testujemy liczby węzłów od, powiedzmy, -9 do 10:
    for (int numNodes = -9; numNodes <= 10; numNodes += 2) {
        vector<double> nodes_temp, nodeValues_temp;
        // Obliczamy krok, aby równomiernie wybrać numNodes węzłów z pełnego zbioru:
        double indexStep = (total - 1) / static_cast<double>(numNodes - 1);
        for (int i = 0; i < numNodes; i++) {
            int idx = static_cast<int>(i * indexStep + 0.5); // zaokrąglenie do najbliższego indeksu
            if (idx >= total) idx = total - 1;
            nodes_temp.push_back(xs[idx]);
            nodeValues_temp.push_back(ys[idx]);
        }
        double mse_temp = computeMSE(xs, ys, nodes_temp, nodeValues_temp);
        cout << "Liczba węzłów: " << numNodes << " - MSE: " << mse_temp << endl;
        if (mse_temp < bestMSE) {
            bestMSE = mse_temp;
            bestNodes = numNodes;
        }
    }

    cout << "Najmniejszy błąd osiągamy dla " << bestNodes << " węzłów, MSE = " << bestMSE << endl;

    return 0;
}
