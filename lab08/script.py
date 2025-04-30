import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Read the approximation data
data = pd.read_csv('approximation_data.txt', sep='\t', comment='#',
                  names=['x', 'f_x', 'F_x', 'error'])

# Read the error by degree data
error_data = pd.read_csv('error_by_degree.txt', sep='\t', comment='#',
                        names=['degree', 'rmse'])

# Create a figure with the function and its approximation
plt.figure(figsize=(10, 6))
plt.plot(data['x'], data['f_x'], 'b-', label='Exact function $f(x)$', linewidth=2)
plt.plot(data['x'], data['F_x'], 'r--', label='Approximation $F(x)$', linewidth=2)
plt.xlabel('x')
plt.ylabel('Function value')
plt.title('Approximation of $f(x) = e^x \\cdot \\cos(6x) - x^3 + 5x^2 - 10$')
plt.grid(True)
plt.legend()
plt.savefig('approximation_plot.png', dpi=300, bbox_inches='tight')

# Create a figure with the approximation error
plt.figure(figsize=(10, 6))
plt.plot(data['x'], data['error'], 'g-', linewidth=2)
plt.xlabel('x')
plt.ylabel('Error: $f(x) - F(x)$')
plt.title('Approximation Error')
plt.grid(True)
plt.savefig('error_plot.png', dpi=300, bbox_inches='tight')

# Create a figure with the error vs polynomial degree
plt.figure(figsize=(10, 6))
plt.semilogy(error_data['degree'], error_data['rmse'], 'bo-', linewidth=2, markersize=8)
plt.xlabel('Polynomial degree')
plt.ylabel('Root Mean Square Error (log scale)')
plt.title('Approximation Error vs Polynomial Degree')
plt.grid(True)
plt.xticks(error_data['degree'])
plt.savefig('error_vs_degree_plot.png', dpi=300, bbox_inches='tight')

print("Plots have been created:")
print("- approximation_plot.png: Shows the original function and its approximation")
print("- error_plot.png: Shows the approximation error")
print("- error_vs_degree_plot.png: Shows how the error decreases with polynomial degree")