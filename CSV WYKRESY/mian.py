import pandas as pd
import matplotlib.pyplot as plt

# Słownik z nazwami plików i etykietami (np. "2", "4", "8", "16", "100")
files = {
    'Dla_2.csv': '2',
    'Dla_4.csv': '4',
    'Dla_8.csv': '8',
    'Dla_16.csv': '16',
    'Dla_100.csv': '100'
}

# Ustawienie rozmiaru wykresu
plt.figure(figsize=(10, 6))

# Iteracja po plikach CSV
for filename, label in files.items():
    # Wczytanie danych z pliku CSV
    df = pd.read_csv(filename)
    
    # Pierwsza kolumna jako x (indeks 0), trzecia kolumna jako y (indeks 2)
    x = df.iloc[:, 0]
    y = df.iloc[:, 2]
    
    # Rysowanie wykresu dla danej serii danych
    plt.plot(x, y, label=f'Funkcja {label}')

# Dodanie tytułu, etykiet osi, legendy i siatki
plt.title('Wykres funkcji z plików CSV')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)

# Wyświetlenie wykresu
plt.show()
