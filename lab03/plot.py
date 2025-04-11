import matplotlib.pyplot as plt
import pandas as pd

def load_data(filename):
    xi, fxi = [], []
    with open(filename, 'r') as file:
        lines = file.readlines()
        xi_line = lines[0].replace("xi:", "").strip().split()
        xi = [float(x) for x in xi_line if x]
        fxi_line = lines[1].replace("f(xi):", "").strip().split()
        fxi = [float(fx) for fx in fxi_line if fx]
    return xi, fxi

# Wczytaj dane
df_newton = pd.read_csv("plot_data.csv", names=["x", "y"])
xi_N, fxi_N = load_data("interpolacja_N_gr_03.txt")
selected_xi = xi_N[::5]

# Generuj wykres
plt.figure(figsize=(14, 7))
plt.plot(df_newton["x"], df_newton["y"], label="Interpolacja Newtona", linewidth=1.5)
plt.scatter(selected_xi, fxi_N[::5], color="red", s=40, label="Węzły (co 5.)")
plt.title("Interpolacja Newtona")
plt.xlabel("x")
plt.ylabel("W(x)")
plt.ylim(-50000, 55000)
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()
plt.savefig("wykres_newton.png", dpi=300)
plt.show()