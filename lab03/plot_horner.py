import matplotlib.pyplot as plt
import pandas as pd

df_horner = pd.read_csv("horner_data.csv", names=["x", "y"])
plt.figure(figsize=(14, 7))
plt.plot(df_horner["x"], df_horner["y"], label="Horner", linewidth=1.5, color="green")
plt.title("Wartości wielomianu (Horner)")
plt.xlabel("x")
plt.ylabel("W(x)")
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()
plt.savefig("wykres_horner.png", dpi=300)
plt.show()