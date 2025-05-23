#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Visualization script for thermal conductivity simulation results
Euler's Method - Laboratory 9: Differential Equations

This script generates comprehensive visualizations for the analysis of 
Euler's method applied to thermal conductivity problem.

Author: [Your Name]
Course: Numerical Methods - AGH University of Science and Technology
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from scipy import stats
import os
import sys

# Configure matplotlib for better output
plt.style.use('seaborn-v0_8-whitegrid')
plt.rcParams.update({
    'font.size': 12,
    'font.family': 'serif',
    'figure.figsize': (10, 6),
    'savefig.dpi': 300,
    'savefig.bbox': 'tight',
    'axes.labelsize': 12,
    'axes.titlesize': 14,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 11,
    'text.usetex': False  # Set to True if LaTeX is available
})

class ThermalVisualization:
    """
    Class for generating visualizations of thermal conductivity simulation results
    """
    
    def __init__(self):
        self.colors = ['#d62728', '#1f77b4', '#2ca02c']  # Red, Blue, Green
        self.step_labels = ['h = 10.0 s', 'h = 1.0 s', 'h = 0.1 s']
        self.linestyles = ['-', '-', '-']
        self.markers = ['o', 's', '^']
        
    def load_simulation_data(self, filename):
        """
        Load simulation data from text file
        
        Parameters:
        -----------
        filename : str
            Path to the data file
            
        Returns:
        --------
        tuple : (time_array, numerical_temp, analytical_temp)
        """
        try:
            time = []
            temp_numerical = []
            temp_analytical = []
            
            with open(filename, 'r', encoding='utf-8') as file:
                for line in file:
                    line = line.strip()
                    if not line.startswith('#') and line:
                        data = line.split()
                        if len(data) >= 3:
                            time.append(float(data[0]))
                            temp_numerical.append(float(data[1]))
                            temp_analytical.append(float(data[2]))
            
            return np.array(time), np.array(temp_numerical), np.array(temp_analytical)
        
        except FileNotFoundError:
            print(f"Error: File '{filename}' not found.")
            return None, None, None
        except Exception as e:
            print(f"Error reading file '{filename}': {e}")
            return None, None, None
    
    def load_error_data(self, filename):
        """
        Load error analysis data
        
        Parameters:
        -----------
        filename : str
            Path to the error data file
            
        Returns:
        --------
        tuple : (step_sizes, rmse_values, max_errors)
        """
        try:
            steps = []
            rmse_values = []
            max_errors = []
            
            with open(filename, 'r', encoding='utf-8') as file:
                for line in file:
                    line = line.strip()
                    if not line.startswith('#') and line:
                        data = line.split()
                        if len(data) >= 3:
                            steps.append(float(data[0]))
                            rmse_values.append(float(data[1]))
                            max_errors.append(float(data[2]))
            
            return np.array(steps), np.array(rmse_values), np.array(max_errors)
        
        except FileNotFoundError:
            print(f"Error: File '{filename}' not found.")
            return None, None, None
    
    def plot_temperature_comparison(self, save_path='porownanie_rozwiazan.png'):
        """
        Generate comparison plot of numerical solutions with analytical solution
        """
        fig, ax = plt.subplots(figsize=(12, 8))
        
        # Track if analytical solution has been plotted
        analytical_plotted = False
        
        # Plot solutions for each step size
        for i in range(3):
            filename = f'wyniki_h{i+1}.txt'
            time, temp_num, temp_ana = self.load_simulation_data(filename)
            
            if time is not None:
                # Plot numerical solution
                ax.plot(time, temp_num, 
                       color=self.colors[i], 
                       linestyle=self.linestyles[i],
                       linewidth=2.5, 
                       label=f'Euler {self.step_labels[i]}',
                       alpha=0.9)
                
                # Plot analytical solution only once
                if not analytical_plotted:
                    ax.plot(time, temp_ana, 
                           'k--', 
                           linewidth=2, 
                           label='Rozwiązanie analityczne',
                           alpha=0.8)
                    analytical_plotted = True
        
        # Customize plot
        ax.set_xlabel('Czas [s]')
        ax.set_ylabel('Temperatura [K]')
        ax.set_title('Chłodzenie kuli - Porównanie metody Eulera z rozwiązaniem analitycznym')
        ax.legend(loc='upper right')
        ax.grid(True, alpha=0.3)
        ax.set_xlim(left=0)
        ax.set_ylim(bottom=0)
        
        # Add text box with initial conditions
        textstr = f'$T_0$ = 1234 K\n$α$ = 1234×10⁻¹² K⁻³s⁻¹'
        props = dict(boxstyle='round', facecolor='wheat', alpha=0.8)
        ax.text(0.02, 0.98, textstr, transform=ax.transAxes, fontsize=10,
                verticalalignment='top', bbox=props)
        
        plt.tight_layout()
        plt.savefig(save_path, dpi=300, bbox_inches='tight')
        plt.show()
        print(f"Temperature comparison plot saved as '{save_path}'")
    
    def plot_relative_error(self, save_path='blad_wzgledny.png'):
        """
        Generate plot of relative error as a function of time
        """
        fig, ax = plt.subplots(figsize=(12, 6))
        
        for i in range(3):
            filename = f'wyniki_h{i+1}.txt'
            time, temp_num, temp_ana = self.load_simulation_data(filename)
            
            if time is not None:
                # Calculate relative error (avoid division by zero)
                relative_error = np.abs(temp_num - temp_ana) / (temp_ana + 1e-10) * 100
                
                ax.semilogy(time, relative_error, 
                           color=self.colors[i], 
                           linewidth=2.5, 
                           label=self.step_labels[i],
                           marker=self.markers[i],
                           markersize=4,
                           markevery=len(time)//10)
        
        ax.set_xlabel('Czas [s]')
        ax.set_ylabel('Błąd względny [%]')
        ax.set_title('Błąd względny metody Eulera w funkcji czasu')
        ax.legend()
        ax.grid(True, alpha=0.3)
        ax.set_xlim(left=0)
        
        plt.tight_layout()
        plt.savefig(save_path, dpi=300, bbox_inches='tight')
        plt.show()
        print(f"Relative error plot saved as '{save_path}'")
    
    def plot_convergence_analysis(self, save_path='mse_vs_krok.png'):
        """
        Generate convergence analysis plot (RMSE vs step size)
        """
        steps, rmse_values, max_errors = self.load_error_data('blad_vs_krok.txt')
        
        if steps is not None:
            fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
            
            # RMSE plot
            ax1.loglog(steps, rmse_values, 'bo-', linewidth=2, markersize=6, label='RMSE')
            
            # Add theoretical trend line (RMSE ∝ h)
            theoretical_rmse = steps * rmse_values[0] / steps[0]
            ax1.loglog(steps, theoretical_rmse, 'r--', alpha=0.7, linewidth=2, 
                      label='Trend teoretyczny (RMSE ∝ h)')
            
            ax1.set_xlabel('Krok całkowania [s]')
            ax1.set_ylabel('Błąd średniokwadratowy (RMSE)')
            ax1.set_title('Zbieżność metody Eulera - RMSE')
            ax1.grid(True, alpha=0.3)
            ax1.legend()
            
            # Maximum error plot
            ax2.loglog(steps, max_errors, 'go-', linewidth=2, markersize=6, label='Błąd maksymalny')
            
            # Add theoretical trend line for max error
            theoretical_max = steps * max_errors[0] / steps[0]
            ax2.loglog(steps, theoretical_max, 'r--', alpha=0.7, linewidth=2, 
                      label='Trend teoretyczny (Błąd ∝ h)')
            
            ax2.set_xlabel('Krok całkowania [s]')
            ax2.set_ylabel('Błąd maksymalny')
            ax2.set_title('Zbieżność metody Eulera - Błąd maksymalny')
            ax2.grid(True, alpha=0.3)
            ax2.legend()
            
            plt.tight_layout()
            plt.savefig(save_path, dpi=300, bbox_inches='tight')
            plt.show()
            print(f"Convergence analysis plot saved as '{save_path}'")
    
    def plot_phase_portrait(self, save_path='portret_fazowy.png'):
        """
        Generate phase portrait showing dT/dt vs T
        """
        fig, ax = plt.subplots(figsize=(10, 6))
        
        # Load data for smallest step size (most accurate)
        time, temp_num, temp_ana = self.load_simulation_data('wyniki_h3.txt')
        
        if time is not None:
            # Calculate derivative using numerical differentiation
            dt = np.diff(time)
            dT_dt = np.diff(temp_num) / dt
            T_mid = (temp_num[:-1] + temp_num[1:]) / 2
            
            # Plot phase portrait
            ax.plot(T_mid, dT_dt, 'b-', linewidth=2, label='Metoda Eulera')
            
            # Plot theoretical curve
            T_theory = np.linspace(T_mid.min(), T_mid.max(), 100)
            alpha = 1234e-12
            dT_dt_theory = -alpha * T_theory**4
            ax.plot(T_theory, dT_dt_theory, 'r--', linewidth=2, label='Teoretyczne (−αT⁴)')
            
            ax.set_xlabel('Temperatura T [K]')
            ax.set_ylabel('dT/dt [K/s]')
            ax.set_title('Portret fazowy - Szybkość chłodzenia w funkcji temperatury')
            ax.legend()
            ax.grid(True, alpha=0.3)
            
            plt.tight_layout()
            plt.savefig(save_path, dpi=300, bbox_inches='tight')
            plt.show()
            print(f"Phase portrait saved as '{save_path}'")
    
    def generate_error_statistics(self):
        """
        Calculate and display detailed error statistics
        """
        print("\n" + "="*60)
        print("ANALIZA BŁĘDÓW METODY EULERA")
        print("="*60)
        
        for i in range(3):
            filename = f'wyniki_h{i+1}.txt'
            time, temp_num, temp_ana = self.load_simulation_data(filename)
            
            if time is not None:
                # Calculate various error metrics
                absolute_error = np.abs(temp_num - temp_ana)
                relative_error = absolute_error / (temp_ana + 1e-10) * 100
                
                rmse = np.sqrt(np.mean(absolute_error**2))
                mae = np.mean(absolute_error)
                max_error = np.max(absolute_error)
                max_rel_error = np.max(relative_error)
                
                step_size = (time[-1] - time[0]) / (len(time) - 1)
                
                print(f"\nKrok całkowania: {step_size:.1f} s")
                print(f"  RMSE:                   {rmse:.6e}")
                print(f"  Błąd średni (MAE):      {mae:.6e}")
                print(f"  Błąd maksymalny:        {max_error:.6e}")
                print(f"  Błąd względny maks.:    {max_rel_error:.3f}%")
                print(f"  Liczba punktów:         {len(time)}")
    
    def create_comprehensive_report(self):
        """
        Generate all visualizations and analysis
        """
        print("Generating comprehensive thermal conductivity analysis...")
        print("-" * 60)
        
        # Check if input files exist
        required_files = ['wyniki_h1.txt', 'wyniki_h2.txt', 'wyniki_h3.txt', 'blad_vs_krok.txt']
        missing_files = [f for f in required_files if not os.path.exists(f)]
        
        if missing_files:
            print("Error: The following required files are missing:")
            for file in missing_files:
                print(f"  - {file}")
            print("\nPlease run the C++ simulation first to generate these files.")
            return
        
        # Generate all plots individually with error handling for each
        plots_generated = []
        
        # Plot 1: Temperature comparison
        try:
            print("Generating temperature comparison plot...")
            self.plot_temperature_comparison()
            plots_generated.append("porownanie_rozwiazan.png")
            print("✓ Temperature comparison plot completed")
        except Exception as e:
            print(f"✗ Error generating temperature comparison: {e}")
        
        # Plot 2: Relative error
        try:
            print("Generating relative error plot...")
            self.plot_relative_error()
            plots_generated.append("blad_wzgledny.png")
            print("✓ Relative error plot completed")
        except Exception as e:
            print(f"✗ Error generating relative error plot: {e}")
        
        # Plot 3: Convergence analysis
        try:
            print("Generating convergence analysis plot...")
            self.plot_convergence_analysis()
            plots_generated.append("mse_vs_krok.png")
            print("✓ Convergence analysis plot completed")
        except Exception as e:
            print(f"✗ Error generating convergence analysis: {e}")
        
        # Plot 4: Phase portrait
        try:
            print("Generating phase portrait...")
            self.plot_phase_portrait()
            plots_generated.append("portret_fazowy.png")
            print("✓ Phase portrait completed")
        except Exception as e:
            print(f"✗ Error generating phase portrait: {e}")
        
        # Generate error statistics
        try:
            print("Generating error statistics...")
            self.generate_error_statistics()
            print("✓ Error statistics completed")
        except Exception as e:
            print(f"✗ Error generating statistics: {e}")
        
        print("\n" + "="*60)
        print("ANALIZA ZAKOŃCZONA")
        print("="*60)
        print(f"Pomyślnie wygenerowano {len(plots_generated)} wykresów:")
        for plot in plots_generated:
            print(f"  ✓ {plot}")
        
        if len(plots_generated) < 4:
            print("\nUwaga: Nie wszystkie wykresy zostały wygenerowane.")
            print("Sprawdź komunikaty błędów powyżej.")

def main():
    """
    Main function - execute complete visualization analysis
    """
    visualizer = ThermalVisualization()
    visualizer.create_comprehensive_report()

if __name__ == "__main__":
    main()