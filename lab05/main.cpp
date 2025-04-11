// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <sstream>
// #include <string>

// using namespace std;

// // Funkcja do wczytywania danych z pliku
// void wczytaj_dane(const string& nazwa_pliku, int& N, vector<double>& b, vector<vector<double>>& A) {
//     ifstream plik(nazwa_pliku);
//     string linia;

//     // Wczytanie liczby równań (N)
//     if (getline(plik, linia)) {
//         stringstream(linia) >> N;
//     }

//     cout << "Liczba zmiennych: " << N << endl;

//     // Wczytanie wektora b
//     if (getline(plik, linia)) {
//         stringstream ss(linia);
//         double temp;
//         while (ss >> temp) {
//             b.push_back(temp);
//         }
//     }

//     cout << "Wektor b wczytany poprawnie: ";
//     for (const auto& el : b) {
//         cout << el << " ";
//     }
//     cout << endl;

//     // Wczytanie macierzy A
//     A.resize(N, vector<double>(N));
//     for (int i = 0; i < N; ++i) {
//         if (getline(plik, linia)) {
//             stringstream ss(linia);
//             double temp;
//             for (int j = 0; j < N; ++j) {
//                 ss >> temp;
//                 A[i][j] = temp;
//             }
//         }
//     }

//     cout << "Macierz A: " << endl;
//     for (const auto& row : A) {
//         for (const auto& el : row) {
//             cout << el << " ";
//         }
//         cout << endl;
//     }
// }

// // Rozkład LU
// void rozklad_LU(vector<vector<double>>& A, vector<vector<double>>& L, vector<vector<double>>& U) {
//     int N = A.size();
//     L.resize(N, vector<double>(N, 0));
//     U.resize(N, vector<double>(N, 0));

//     for (int i = 0; i < N; ++i) {
//         L[i][i] = 1.0;
//     }

//     for (int k = 0; k < N; ++k) {
//         for (int j = k; j < N; ++j) {
//             U[k][j] = A[k][j];
//             for (int r = 0; r < k; ++r) {
//                 U[k][j] -= L[k][r] * U[r][j];
//             }
//         }

//         for (int i = k + 1; i < N; ++i) {
//             L[i][k] = A[i][k];
//             for (int r = 0; r < k; ++r) {
//                 L[i][k] -= L[i][r] * U[r][k];
//             }
//             L[i][k] /= U[k][k];
//         }
//     }
// }

// // Rozwiązywanie układu równań Lz = b
// void rozwiaz_Lz(const vector<vector<double>>& L, const vector<double>& b, vector<double>& z) {
//     int N = L.size();
//     z.resize(N);

//     for (int i = 0; i < N; ++i) {
//         z[i] = b[i];
//         for (int j = 0; j < i; ++j) {
//             z[i] -= L[i][j] * z[j];
//         }
//     }
// }

// // Rozwiązywanie układu równań Ux = z
// void rozwiaz_Ux(const vector<vector<double>>& U, const vector<double>& z, vector<double>& x) {
//     int N = U.size();
//     x.resize(N);

//     for (int i = N - 1; i >= 0; --i) {
//         x[i] = z[i];
//         for (int j = i + 1; j < N; ++j) {
//             x[i] -= U[i][j] * x[j];
//         }
//         x[i] /= U[i][i];
//     }
// }

// // Funkcja do wypisania macierzy
// void wypisz_macierz(const vector<vector<double>>& M) {
//     for (const auto& wiersz : M) {
//         for (double el : wiersz) {
//             cout << el << " ";
//         }
//         cout << endl;
//     }
// }

// // Funkcja do obliczania A * x
// void oblicz_Ax(const vector<vector<double>>& A, const vector<double>& x, vector<double>& result) {
//     int N = A.size();
//     result.resize(N);
//     for (int i = 0; i < N; ++i) {
//         result[i] = 0;
//         for (int j = 0; j < N; ++j) {
//             result[i] += A[i][j] * x[j];
//         }
//     }
// }

// // Funkcja do obliczania L * U
// void oblicz_LU(const vector<vector<double>>& L, const vector<vector<double>>& U, vector<vector<double>>& result) {
//     int N = L.size();
//     result.resize(N, vector<double>(N, 0));
//     for (int i = 0; i < N; ++i) {
//         for (int j = 0; j < N; ++j) {
//             for (int k = 0; k < N; ++k) {
//                 result[i][j] += L[i][k] * U[k][j];
//             }
//         }
//     }
// }

// int main() {
//     int N;
//     vector<double> b;
//     vector<vector<double>> A;

//     // Wczytanie danych z pliku
//     wczytaj_dane("LU_gr3_2.txt", N, b, A);

//     // Rozkład LU
//     vector<vector<double>> L, U;
//     rozklad_LU(A, L, U);

//     // Wypisanie macierzy L i U
//     cout << "Macierz L:" << endl;
//     wypisz_macierz(L);
//     cout << "Macierz U:" << endl;
//     wypisz_macierz(U);

//     // Rozwiązywanie układu równań Lz = b
//     vector<double> z;
//     rozwiaz_Lz(L, b, z);

//     // Rozwiązywanie układu równań Ux = z
//     vector<double> x;
//     rozwiaz_Ux(U, z, x);

//     // Wypisanie wyników
//     cout << "Rozwiązanie x:" << endl;
//     for (double el : x) {
//         cout << el << " ";
//     }
//     cout << endl;

//     // Sprawdzanie poprawności A * x = b
//     vector<double> Ax;
//     oblicz_Ax(A, x, Ax);
//     cout << "\nSprawdzanie poprawności (A * x = b):" << endl;
//     for (int i = 0; i < N; ++i) {
//         cout << "b[" << i << "] = " << b[i] << ", Ax[" << i << "] = " << Ax[i] << endl;
//     }

//     // Sprawdzanie poprawności L * U = A
//     vector<vector<double>> LU;
//     oblicz_LU(L, U, LU);
//     cout << "\nSprawdzanie poprawności (L * U = A):" << endl;
//     for (int i = 0; i < N; ++i) {
//         for (int j = 0; j < N; ++j) {
//             cout << "A[" << i << "][" << j << "] = " << A[i][j] << ", LU[" << i << "][" << j << "] = " << LU[i][j] << endl;
//         }
//     }

//     return 0;
// }
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

// Funkcja do wczytywania danych z pliku
void wczytaj_dane(const string& nazwa_pliku, int& N, vector<double>& b, vector<vector<double>>& A) {
    ifstream plik(nazwa_pliku);
    string linia;

    // Wczytanie liczby równań (N)
    if (getline(plik, linia)) {
        stringstream(linia) >> N;
    }

    cout << "Liczba zmiennych: " << N << endl;

    // Wczytanie wektora b
    if (getline(plik, linia)) {
        stringstream ss(linia);
        double temp;
        while (ss >> temp) {
            b.push_back(temp);
        }
    }

    cout << "Wektor b wczytany poprawnie: ";
    for (const auto& el : b) {
        cout << el << " ";
    }
    cout << endl;

    // Wczytanie macierzy A
    A.resize(N, vector<double>(N));
    for (int i = 0; i < N; ++i) {
        if (getline(plik, linia)) {
            stringstream ss(linia);
            double temp;
            for (int j = 0; j < N; ++j) {
                ss >> temp;
                A[i][j] = temp;
            }
        }
    }

    cout << "Macierz A: " << endl;
    for (const auto& row : A) {
        for (const auto& el : row) {
            cout << el << " ";
        }
        cout << endl;
    }
}

// Rozkład LU
void rozklad_LU(vector<vector<double>>& A, vector<vector<double>>& L, vector<vector<double>>& U) {
    int N = A.size();
    L.resize(N, vector<double>(N, 0));
    U.resize(N, vector<double>(N, 0));

    for (int i = 0; i < N; ++i) {
        L[i][i] = 1.0;
    }

    for (int k = 0; k < N; ++k) {
        for (int j = k; j < N; ++j) {
            U[k][j] = A[k][j];
            for (int r = 0; r < k; ++r) {
                U[k][j] -= L[k][r] * U[r][j];
            }
        }

        for (int i = k + 1; i < N; ++i) {
            L[i][k] = A[i][k];
            for (int r = 0; r < k; ++r) {
                L[i][k] -= L[i][r] * U[r][k];
            }
            L[i][k] /= U[k][k];
        }
    }
}

// Rozwiązywanie układu równań Lz = b
void rozwiaz_Lz(const vector<vector<double>>& L, const vector<double>& b, vector<double>& z) {
    int N = L.size();
    z.resize(N);

    for (int i = 0; i < N; ++i) {
        z[i] = b[i];
        for (int j = 0; j < i; ++j) {
            z[i] -= L[i][j] * z[j];
        }
    }
}

// Rozwiązywanie układu równań Ux = z
void rozwiaz_Ux(const vector<vector<double>>& U, const vector<double>& z, vector<double>& x) {
    int N = U.size();
    x.resize(N);

    for (int i = N - 1; i >= 0; --i) {
        x[i] = z[i];
        for (int j = i + 1; j < N; ++j) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
}

// Funkcja do wypisania macierzy
void wypisz_macierz(const vector<vector<double>>& M) {
    for (const auto& wiersz : M) {
        for (double el : wiersz) {
            cout << el << " ";
        }
        cout << endl;
    }
}

// Funkcja do obliczania A * x
void oblicz_Ax(const vector<vector<double>>& A, const vector<double>& x, vector<double>& result) {
    int N = A.size();
    result.resize(N);
    for (int i = 0; i < N; ++i) {
        result[i] = 0;
        for (int j = 0; j < N; ++j) {
            result[i] += A[i][j] * x[j];
        }
    }
}

// Funkcja do obliczania L * U
void oblicz_LU(const vector<vector<double>>& L, const vector<vector<double>>& U, vector<vector<double>>& result) {
    int N = L.size();
    result.resize(N, vector<double>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                result[i][j] += L[i][k] * U[k][j];
            }
        }
    }
}

int main() {
    int N;
    vector<double> b;
    vector<vector<double>> A;

    // Wczytanie danych z pliku
    wczytaj_dane("LU_gr3_2.txt", N, b, A);

    // Rozkład LU
    vector<vector<double>> L, U;
    rozklad_LU(A, L, U);

    // Wypisanie macierzy L i U
    cout << "Macierz L:" << endl;
    wypisz_macierz(L);
    cout << "Macierz U:" << endl;
    wypisz_macierz(U);

    // Rozwiązywanie układu równań Lz = b
    vector<double> z;
    rozwiaz_Lz(L, b, z);

    // Rozwiązywanie układu równań Ux = z
    vector<double> x;
    rozwiaz_Ux(U, z, x);

    // Wypisanie wyników
    cout << "Rozwiązanie x:" << endl;
    for (double el : x) {
        cout << el << " ";
    }
    cout << endl;

    // Sprawdzanie poprawności A * x = b
    vector<double> Ax;
    oblicz_Ax(A, x, Ax);
    cout << "\nSprawdzanie poprawności (A * x = b):" << endl;
    for (int i = 0; i < N; ++i) {
        cout << "b[" << i << "] = " << b[i] << ", Ax[" << i << "] = " << Ax[i] << endl;
    }

    // Sprawdzanie poprawności L * U = A
    vector<vector<double>> LU;
    oblicz_LU(L, U, LU);
    cout << "\nSprawdzanie poprawności (L * U = A):" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << "A[" << i << "][" << j << "] = " << A[i][j] << ", LU[" << i << "][" << j << "] = " << LU[i][j] << endl;
        }
    }

    return 0;
}
