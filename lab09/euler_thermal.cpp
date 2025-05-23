#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;
/**
 * @class EulerSolver
 * @brief Implementation of Euler's method for solving thermal conductivity differential equation
 * 
 * This class implements the numerical solution for the differential equation:
 * dT/dt = -α * T^4
 * which describes the cooling of a sphere through radiation.
 */
class EulerSolver {
private:
    double alpha;           // Proportionality coefficient [1/(K³·s)]
    double T0;              // Initial temperature [K]
    double t_max;           // Maximum simulation time [s]
    
public:
    /**
     * @brief Constructor for EulerSolver
     * @param a Proportionality coefficient α
     * @param T_initial Initial temperature T₀
     * @param t_maximum Maximum time for simulation
     */
    EulerSolver(double a, double T_initial, double t_maximum) 
        : alpha(a), T0(T_initial), t_max(t_maximum) {}
    
    /**
     * @brief Right-hand side function of the differential equation
     * @param t Current time (unused in this autonomous system)
     * @param T Current temperature
     * @return dT/dt = -α * T^4
     */
    double f(double t, double T) {
        return -alpha * pow(T, 4);
    }
    
    /**
     * @brief Analytical solution for small times (linearization approximation)
     * @param t Time at which to evaluate the solution
     * @return Approximate analytical solution T(t)
     * 
     * For small times and small α, we can approximate:
     * T(t) ≈ T₀ * exp(-α * T₀³ * t)
     */
    double analyticalSolution(double t) {
        // For very small times, use linearization around T₀
        return T0 * exp(-alpha * pow(T0, 3) * t);
    }
    
    /**
     * @brief Solve the differential equation using Euler's method
     * @param h Integration step size
     * @return Vector of (time, temperature) pairs
     */
    vector<pair<double, double>> solveEuler(double h) {
        vector<pair<double, double>> results;
        
        double t = 0.0;
        double T = T0;
        
        // Store initial point
        results.push_back({t, T});
        
        // Euler's method iterations
        while (t < t_max) {
            // Euler step: T_new = T_old + h * f(t, T)
            T = T + h * f(t, T);
            t = t + h;
            
            // Store current point
            results.push_back({t, T});
            
            // Safety check for non-physical negative temperatures
            if (T < 0) {
                cerr << "Warning: Negative temperature reached at t = " << t << endl;
                T = 0;
                break;
            }
        }
        
        return results;
    }
    
    /**
     * @brief Calculate root mean square error between numerical and analytical solutions
     * @param numerical Vector of numerical solution points
     * @return RMSE value
     */
    double calculateRMSE(const vector<pair<double, double>>& numerical) {
        double mse = 0.0;
        int count = 0;
        
        for (const auto& point : numerical) {
            double t = point.first;
            double T_num = point.second;
            double T_ana = analyticalSolution(t);
            
            mse += pow(T_num - T_ana, 2);
            count++;
        }
        
        return sqrt(mse / count);
    }
    
    /**
     * @brief Calculate maximum absolute error
     * @param numerical Vector of numerical solution points
     * @return Maximum absolute error
     */
    double calculateMaxError(const vector<pair<double, double>>& numerical) {
        double max_error = 0.0;
        
        for (const auto& point : numerical) {
            double t = point.first;
            double T_num = point.second;
            double T_ana = analyticalSolution(t);
            
            double error = fabs(T_num - T_ana);
            if (error > max_error) {
                max_error = error;
            }
        }
        
        return max_error;
    }
};

/**
 * @brief Main function - executes the numerical experiment
 */
int main() {
    // Problem parameters (replace 1234 with last 4 digits of your student ID)
    const int student_id = 1234;
    const double alpha = student_id * 1e-12;  // [1/(K³·s)]
    const double T0 = student_id;             // Initial temperature [K]
    const double t_max = student_id;          // Simulation time [s]
    
    // Different integration step sizes for convergence analysis
    vector<double> steps = {10.0, 1.0, 0.1};
    
    // Initialize solver
    EulerSolver solver(alpha, T0, t_max);
    
    cout << "Thermal Conductivity Problem - Euler's Method Analysis" << endl;
    cout << "=====================================================" << endl;
    cout << "Initial temperature: " << T0 << " K" << endl;
    cout << "Alpha coefficient: " << scientific << alpha << endl;
    cout << "Simulation time: " << t_max << " s" << endl;
    cout << endl;
    
    // Solve for different steps and save results
    for (size_t i = 0; i < steps.size(); i++) {
        double h = steps[i];
        auto results = solver.solveEuler(h);
        
        // Generate output filename
        string filename = "wyniki_h" + to_string(i+1) + ".txt";
        ofstream file(filename);
        
        // Write header
        file << "# Thermal conductivity simulation results" << endl;
        file << "# Integration step h = " << h << " s" << endl;
        file << "# Initial temperature = " << T0 << " K" << endl;
        file << "# Alpha = " << scientific << alpha << endl;
        file << "# Format: Time[s] Temperature_Numerical[K] Temperature_Analytical[K]" << endl;
        
        // Write data points
        for (const auto& point : results) {
            double t = point.first;
            double T_num = point.second;
            double T_ana = solver.analyticalSolution(t);
            
            file << fixed << setprecision(6)
                 << t << " " << T_num << " " << T_ana << endl;
        }
        
        file.close();
        
        // Calculate and display error metrics
        double rmse = solver.calculateRMSE(results);
        double max_error = solver.calculateMaxError(results);
        
        cout << "Step size h = " << setw(8) << h << " s: "
                  << "RMSE = " << scientific << setprecision(3) << rmse
                  << ", Max Error = " << max_error
                  << ", Points = " << results.size() << endl;
    }
    
    // Error analysis as a function of step size
    cout << endl << "Generating convergence analysis..." << endl;
    ofstream error_file("blad_vs_krok.txt");
    error_file << "# Error analysis for Euler's method" << endl;
    error_file << "# Format: Step_size[s] RMSE Max_Error" << endl;
    
    // Test various step sizes for convergence study
    for (double h = 0.01; h <= 10.0; h *= 1.5) {
        auto results = solver.solveEuler(h);
        double rmse = solver.calculateRMSE(results);
        double max_error = solver.calculateMaxError(results);
        
        error_file << fixed << setprecision(6)
                   << h << " " << scientific << rmse << " " << max_error << endl;
    }
    
    error_file.close();
    
    // Generate summary table
    ofstream summary_file("summary_table.txt");
    summary_file << "# Summary of results at selected time points" << endl;
    summary_file << "# Format: Time[s] h=10.0[K] h=1.0[K] h=0.1[K] Analytical[K]" << endl;
    
    // Select representative time points
    vector<double> time_points = {0, 100, 200, 300, 500, 1000, student_id};
    
    for (double target_time : time_points) {
        if (target_time <= t_max) {
            summary_file << fixed << setprecision(0) << target_time;
            
            // Find values for each step size
            for (double h : steps) {
                auto results = solver.solveEuler(h);
                
                // Find closest time point
                double closest_temp = T0;
                double min_diff = abs(target_time);
                
                for (const auto& point : results) {
                    double diff = abs(point.first - target_time);
                    if (diff < min_diff) {
                        min_diff = diff;
                        closest_temp = point.second;
                    }
                }
                
                summary_file << " " << fixed << setprecision(3) << closest_temp;
            }
            
            // Add analytical solution
            double analytical_temp = solver.analyticalSolution(target_time);
            summary_file << " " << fixed << setprecision(3) << analytical_temp << endl;
        }
    }
    
    summary_file.close();
    
    cout << "Calculations completed successfully." << endl;
    cout << "Generated files:" << endl;
    cout << "  - wyniki_h1.txt, wyniki_h2.txt, wyniki_h3.txt (solution data)" << endl;
    cout << "  - blad_vs_krok.txt (error analysis)" << endl;
    cout << "  - summary_table.txt (results summary)" << endl;
    cout << endl;
    cout << "Run the Python visualization script to generate graphs." << endl;
    
    return 0;
}