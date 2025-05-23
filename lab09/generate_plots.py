#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Simplified visualization script for thermal conductivity simulation
Generates all required plots for Laboratory 9 report

Run this script after executing the C++ program to generate all visualizations.
"""

import numpy as np
import matplotlib.pyplot as plt
import os

def load_data(filename):
    """Load simulation data from file"""
    time, temp_num, temp_ana = [], [], []
    
    try:
        with open(filename, 'r') as file:
            for line in file:
                if not line.startswith('#') and line.strip():
                    data = line.split()
                    if len(data) >= 3:
                        time.append(float(data[0]))
                        temp_num.append(float(data[1]))
                        temp_ana.append(float(data[2]))
        return np.array(time), np.array(temp_num), np.array(temp_ana)
    except:
        return None, None, None

def generate_all_plots():
    """Generate all required plots"""
    
    # Set up matplotlib
    plt.style.use('default')
    plt.rcParams['figure.figsize'] = (10, 6)
    plt.rcParams['font.size'] = 12
    
    colors = ['red', 'blue', 'green']
    labels = ['h = 10.0 s', 'h = 1.0 s', 'h = 0.1 s']
    
    # 1. TEMPERATURE COMPARISON PLOT
    print("Generating plot 1/4: Temperature comparison...")
    plt.figure(figsize=(12, 8))
    
    analytical_plotted = False
    for i in range(3):
        time, temp_num, temp_ana = load_data(f'wyniki_h{i+1}.txt')
        if time is not None:
            plt.plot(time, temp_num, color=colors[i], linewidth=2, label=f'Euler {labels[i]}')
            if not analytical_plotted:
                plt.plot(time, temp_ana, 'k--', linewidth=2, label='Rozwiązanie analityczne')
                analytical_plotted = True
    
    plt.xlabel('Czas [s]')
    plt.ylabel('Temperatura [K]')
    plt.title('Chłodzenie kuli - Porównanie metody Eulera z rozwiązaniem analitycznym')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig('porownanie_rozwiazan.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("✓ Plot 1 completed: porownanie_rozwiazan.png")
    
    # 2. RELATIVE ERROR PLOT
    print("Generating plot 2/4: Relative error...")
    plt.figure(figsize=(12, 6))
    
    for i in range(3):
        time, temp_num, temp_ana = load_data(f'wyniki_h{i+1}.txt')
        if time is not None:
            relative_error = np.abs(temp_num - temp_ana) / (temp_ana + 1e-10) * 100
            plt.semilogy(time, relative_error, color=colors[i], linewidth=2, label=labels[i])
    
    plt.xlabel('Czas [s]')
    plt.ylabel('Błąd względny [%]')
    plt.title('Błąd względny metody Eulera w funkcji czasu')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig('blad_wzgledny.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("✓ Plot 2 completed: blad_wzgledny.png")
    
    # 3. CONVERGENCE ANALYSIS PLOT
    print("Generating plot 3/4: Convergence analysis...")
    try:
        # Load error data
        steps, rmse_values = [], []
        with open('blad_vs_krok.txt', 'r') as file:
            for line in file:
                if not line.startswith('#') and line.strip():
                    data = line.split()
                    if len(data) >= 2:
                        steps.append(float(data[0]))
                        rmse_values.append(float(data[1]))
        
        steps = np.array(steps)
        rmse_values = np.array(rmse_values)
        
        plt.figure(figsize=(10, 6))
        plt.loglog(steps, rmse_values, 'bo-', linewidth=2, markersize=6, label='RMSE')
        
        # Add theoretical trend line
        theoretical = steps * rmse_values[0] / steps[0]
        plt.loglog(steps, theoretical, 'r--', alpha=0.7, linewidth=2, label='Trend teoretyczny (RMSE ∝ h)')
        
        plt.xlabel('Krok całkowania [s]')
        plt.ylabel('Błąd średniokwadratowy (RMSE)')
        plt.title('Zależność błędu metody Eulera od kroku całkowania')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.savefig('mse_vs_krok.png', dpi=300, bbox_inches='tight')
        plt.close()
        print("✓ Plot 3 completed: mse_vs_krok.png")
    except:
        print("✗ Error generating convergence plot - check blad_vs_krok.txt file")
    
    # 4. TEMPERATURE PROFILE (Alternative to phase portrait)
    print("Generating plot 4/4: Detailed temperature profile...")
    plt.figure(figsize=(12, 8))
    
    # Focus on the most accurate solution (smallest step)
    time, temp_num, temp_ana = load_data('wyniki_h3.txt')
    if time is not None:
        plt.subplot(2, 1, 1)
        plt.plot(time, temp_num, 'g-', linewidth=2, label='Euler h = 0.1 s')
        plt.plot(time, temp_ana, 'k--', linewidth=2, label='Rozwiązanie analityczne')
        plt.xlabel('Czas [s]')
        plt.ylabel('Temperatura [K]')
        plt.title('Szczegółowy profil temperatury')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        # Plot the difference
        plt.subplot(2, 1, 2)
        diff = temp_num - temp_ana
        plt.plot(time, diff, 'r-', linewidth=2)
        plt.xlabel('Czas [s]')
        plt.ylabel('Różnica T_num - T_ana [K]')
        plt.title('Błąd bezwzględny w funkcji czasu')
        plt.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('profil_temperatury.png', dpi=300, bbox_inches='tight')
        plt.close()
        print("✓ Plot 4 completed: profil_temperatury.png")
    
    print("\n" + "="*50)
    print("WSZYSTKIE WYKRESY WYGENEROWANE POMYŚLNIE!")
    print("="*50)
    print("Pliki:")
    print("  1. porownanie_rozwiazan.png")
    print("  2. blad_wzgledny.png") 
    print("  3. mse_vs_krok.png")
    print("  4. profil_temperatury.png")

def check_files():
    """Check if required input files exist"""
    required_files = ['wyniki_h1.txt', 'wyniki_h2.txt', 'wyniki_h3.txt', 'blad_vs_krok.txt']
    missing = [f for f in required_files if not os.path.exists(f)]
    
    if missing:
        print("BŁĄD: Brakuje następujących plików:")
        for f in missing:
            print(f"  - {f}")
        print("\nPrzeprp w pierwszej kolejności program C++ (euler_thermal)!")
        return False
    return True

if __name__ == "__main__":
    if check_files():
        generate_all_plots()
    else:
        print("Nie można wygenerować wykresów bez plików danych.")