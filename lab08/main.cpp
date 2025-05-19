#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <algorithm>
using namespace std;
// Function to approximate: f(x) = e^x · cos(6x) - x^3 + 5x^2 - 10
double f(double x) {
    return exp(x) * cos(6 * x) - pow(x, 3) + 5 * pow(x, 2) - 10;
}

// Function to evaluate a polynomial using Horner's method
double evaluatePolynomial(const vector<double>& coefficients, double x) {
    double result = coefficients[0];
    for (size_t i = 1; i < coefficients.size(); ++i) {
        result = result * x + coefficients[i];
    }
    return result;
}

// Function to solve a system of linear equations using Gaussian elimination
vector<double> solveLinearSystem(vector<vector<double>> A, vector<double> b) {
    int n = A.size();
    
    // Forward elimination with partial pivoting
    for (int i = 0; i < n; i++) {
        // Find pivot row
        int maxRow = i;
        double maxVal = abs(A[i][i]);
        for (int j = i + 1; j < n; j++) {
            if (abs(A[j][i]) > maxVal) {
                maxVal = abs(A[j][i]);
                maxRow = j;
            }
        }
        
        // Swap rows if needed
        if (maxRow != i) {
            swap(A[i], A[maxRow]);
            swap(b[i], b[maxRow]);
        }
        
        // Eliminate below
        for (int j = i + 1; j < n; j++) {
            double factor = A[j][i] / A[i][i];
            b[j] -= factor * b[i];
            for (int k = i; k < n; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
    }
    
    // Back substitution
    vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
    
    return x;
}

// Function to calculate the inner product of two monomials integrated over [a,b]
double innerProductMonomials(int i, int j, double a, double b) {
    // Inner product of x^i and x^j is the integral of x^(i+j) over [a,b]
    return (pow(
        
        b, i + j + 1) - pow(a, i + j + 1)) / (i + j + 1);
}

// Function to calculate the inner product of a monomial with the function f(x)
double innerProductWithFunction(int i, double a, double b, int numPoints) {
    // We'll use a numerical integration method (trapezoidal rule) for this
    double h = (b - a) / numPoints;
    double sum = 0;
    
    for (int j = 0; j <= numPoints; j++) {
        double x = a + j * h;
        double weight = (j == 0 || j == numPoints) ? 0.5 : 1.0;
        sum += weight * pow(x, i) * f(x);
    }
    
    return sum * h;
}

// Function to perform least squares approximation
vector<double> leastSquaresApproximation(double a, double b, int degree, int numPoints) {
    int n = degree + 1;
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> B(n);
    
    // Fill matrix A and vector B 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // A[i][j] is the inner product of x^i and x^j
            A[i][j] = innerProductMonomials(i, j, a, b);
        }
        // B[i] is the inner product of x^i and f(x)
        B[i] = innerProductWithFunction(i, a, b, numPoints);
    }
    
    // Solve the linear system
    vector<double> coefficients = solveLinearSystem(A, B);
    
    // The coefficients from the linear solver are in reversed order for the standard polynomial form
    // For a polynomial a0 + a1*x + a2*x^2 + ... + an*x^n, we want to return [an, ..., a2, a1, a0]
    reverse(coefficients.begin(), coefficients.end());
    
    return coefficients;
}

// Function to calculate the approximation error at a specific point
double approximationError(const vector<double>& coefficients, double x) {
    double approx = evaluatePolynomial(coefficients, x);
    return f(x) - approx;
}

// Function to calculate the root mean square error
double calculateRMSE(const vector<double>& coefficients, double a, double b, int numPoints) {
    double h = (b - a) / numPoints;
    double sumSquaredErrors = 0;
    
    for (int i = 0; i <= numPoints; i++) {
        double x = a + i * h;
        double error = approximationError(coefficients, x);
        sumSquaredErrors += error * error;
    }
    
    return sqrt(sumSquaredErrors / (numPoints + 1));
}

// Function to save data points to a file for plotting
void saveDataForPlotting(const vector<double>& coefficients, double a, double b, 
                        int numPoints, const string& filename) {
    ofstream outFile(filename);
    double h = (b - a) / numPoints;
    
    outFile << "# x\tf(x)\tF(x)\terror\n";
    
    for (int i = 0; i <= numPoints; i++) {
        double x = a + i * h;
        double exactValue = f(x);
        double approxValue = evaluatePolynomial(coefficients, x);
        double error = exactValue - approxValue;
        
        outFile << x << "\t" << exactValue << "\t" << approxValue << "\t" << error << "\n";
    }
    
    outFile.close();
}

int main() {
    // Range of approximation
    double a = 1.5;
    double b = 3.0;
    int numPoints = 1000; // Number of points for error calculation and plotting
    
    // Test different polynomial degrees
    vector<int> degrees = {2, 3, 4, 5, 6, 7, 8};
    vector<double> errors;
    
    cout << "Least Squares Approximation for f(x) = e^x · cos(6x) - x^3 + 5x^2 - 10\n";
    cout << "Over interval [" << a << ", " << b << "]\n\n";
    
    for (int degree : degrees) {
        auto start = chrono::high_resolution_clock::now();
        
        // Calculate approximation coefficients
        vector<double> coefficients = leastSquaresApproximation(a, b, degree, numPoints);
        
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;
        
        // Calculate RMSE
        double rmse = calculateRMSE(coefficients, a, b, numPoints);
        errors.push_back(rmse);
        
        // Print results
        cout << "Polynomial degree: " << degree << "\n";
        cout << "Coefficients (standard form a0 + a1*x + a2*x^2 + ... + an*x^n):\n";
        for (int i = 0; i < coefficients.size(); i++) {
            cout << "  a" << i << " = " << setprecision(8) << coefficients[coefficients.size() - 1 - i] << "\n";
        }
        cout << "RMSE: " << setprecision(8) << rmse << "\n";
        cout << "Computation time: " << duration.count() << " ms\n\n";
        
        // Save data for degree 6 (as specified in the assignment)
        if (degree == 6) {
            saveDataForPlotting(coefficients, a, b, numPoints, "approximation_data.txt");
        }
    }
    
    // Save error data for different polynomial degrees
    ofstream errorFile("error_by_degree.txt");
    errorFile << "# Degree\tRMSE\n";
    for (size_t i = 0; i < degrees.size(); i++) {
        errorFile << degrees[i] << "\t" << errors[i] << "\n";
    }
    errorFile.close();
    
    cout << "Data files have been created for plotting.\n";
    cout << "- approximation_data.txt: Contains function values and approximation for degree 6\n";
    cout << "- error_by_degree.txt: Contains RMSE values for different polynomial degrees\n";
    
    return 0;
}