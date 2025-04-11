#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>

using namespace std;

// Funkcja wypisująca macierz rozszerzoną [A | b]
void printAugmentedMatrix(const vector<vector<double>> &A, const vector<double> &b, ofstream &outFile) {
    int N = b.size();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            outFile << setw(10) << A[i][j] << " ";
        }
        outFile << " | " << setw(10) << b[i] << endl;
    }
    outFile << endl;
}

// Funkcja do eliminacji Gaussa z częściowym pivotingiem
void gaussElimination(vector<vector<double>> &A, vector<double> &b, int N, ofstream &outFile) {
    for (int k = 0; k < N; k++) {
        int maxRow = k;
        double maxVal = fabs(A[k][k]);
        for (int i = k + 1; i < N; i++) {
            if (fabs(A[i][k]) > maxVal) {
                maxVal = fabs(A[i][k]);
                maxRow = i;
            }
        }
        if (fabs(A[maxRow][k]) < 1e-9) {
            outFile << "Brak unikalnego rozwiązania, macierz może być osobliwa." << endl;
            return;
        }
        if (maxRow != k) {
            swap(A[k], A[maxRow]);
            swap(b[k], b[maxRow]);
        }

        for (int i = k + 1; i < N; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < N; j++) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }

        outFile << "Po eliminacji dla zmiennej x" << k + 1 << ":" << endl;
        printAugmentedMatrix(A, b, outFile);
    }
}

// Funkcja do podstawiania wstecznego
vector<double> backSubstitution(const vector<vector<double>> &A, const vector<double> &b, int N) {
    vector<double> x(N, 0.0);
    for (int i = N - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < N; j++) {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }
    return x;
}

// Funkcja do wczytywania danych
bool loadData(const string &filename, vector<vector<double>> &A, vector<double> &b, int &N) {
    ifstream infile(filename);
    if (!infile) {
        cout << "Nie mozna otworzyc pliku " << filename << endl;
        return false;
    }

    string line;
    bool nDefined = false;
    while (getline(infile, line)) {
        if (line.find("N") != string::npos) {
            size_t pos = line.find("=");
            N = stoi(line.substr(pos + 1));
            nDefined = true;
            continue;
        }
        if (line.find("b:") != string::npos) {
            getline(infile, line);
            stringstream ss(line);
            double num;
            while (ss >> num) {
                b.push_back(num);
            }
            continue;
        }
        if (line.find("A:") != string::npos) {
            A.resize(N, vector<double>(N, 0.0));
            for (int i = 0; i < N; i++) {
                getline(infile, line);
                stringstream ss(line);
                for (int j = 0; j < N; j++) {
                    ss >> A[i][j];
                }
            }
            break;
        }
    }
    infile.close();
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uzycie: " << argv[0] << " plik_wejsciowy.txt" << endl;
        return 1;
    }

    vector<vector<double>> A;
    vector<double> b;
    int N = 0;

    if (!loadData(argv[1], A, b, N)) {
        return 1;
    }

    ofstream outFile("wyniki.txt");

    outFile << "Macierz rozszerzona początkowa:" << endl;
    printAugmentedMatrix(A, b, outFile);

    gaussElimination(A, b, N, outFile);

    vector<double> x = backSubstitution(A, b, N);

    outFile << "Rozwiązanie układu:" << endl;
    for (int i = 0; i < N; i++) {
        outFile << "x" << i + 1 << " = " << x[i] << endl;
    }

    outFile << "Sprawdzenie rozwiązania (Ax):" << endl;
    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        for (int j = 0; j < N; j++) {
            sum += A[i][j] * x[j];
        }
        outFile << "Wiersz " << i + 1 << ": " << sum << " (oryginalne b: " << b[i] << ")" << endl;
    }

    outFile.close();
    return 0;
}
