#  Project Inception (v4.1) - Analytics & Plotting Utility
#  Copyright (C) 2026 Developer & Consultant, M.Sc. Eng.
#  Licensed under the GNU GPL v3 - Open Science Initiative

import pandas as pd
import matplotlib.pyplot as plt
import os

# --- 1. WCZYTANIE SUROWEJ TELEMETRII Z C++ ---
plik_danych = "raport_hpc.txt"

if not os.path.exists(plik_danych):
    print(f"❌ Blad: Nie znaleziono pliku {plik_danych} in the current directory!")
    print("ℹ️ Upewnij sie, ze najpierw uruchomiles program w C++ i wygenerowales logi.")
    exit()

# Odczyt danych przy uzyciu biblioteki Pandas
df = pd.read_csv(plik_danych)
print(f"✅ Pomyslnie wczytano {len(df)} rekordow telemetrii ewolucyjnej.")

# --- 2. GENEROWANIE WYKRESU 1: ZBIEZNOSC WSPOLCZYNNIKA SPOJNOSCI ---
plt.figure(figsize=(12, 5))

plt.subplot(1, 2, 1)
plt.plot(df['Pokolenie'], df['Wspolczynnik_Spojnosci'], color='cyan', linewidth=2, label='Coherence Index')
plt.axhline(y=0.755482, color='magenta', linestyle='--', alpha=0.7, label='Target Homeostasis (~75.5%)')
plt.title("🧬 Neuromorphic Core Convergence (STDP Loop)")
plt.xlabel("Generation (Gen)")
plt.ylabel("Synaptic Coherence Index (j.z.)")
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()

# --- 3. GENEROWANIE WYKRESU 2: DYNAMICZNY ZYSK INFRASTRUKTURALNY ---
# Konwertujemy watogodziny (Wh) na megawatogodziny (MWh) dla zwiekszenia czytelnosci biznesowej
df['Skumulowana_Energia_MWh'] = df['Skumulowana_Energia_Wh'] / 1000000.0

plt.subplot(1, 2, 2)
plt.plot(df['Pokolenie'], df['Skumulowana_Energia_MWh'], color='lime', linewidth=2, label='Energy Saved')
plt.title("🔋 Accumulated Net Infrastructure Energy Savings")
plt.xlabel("Generation (Gen)")
plt.ylabel("Saved Infrastructure Power (MWh)")
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()

# --- 4. EXPORT I WYSWIETLENIE ---
plt.tight_layout()
output_image = "inception_v4_metrics.png"
plt.savefig(output_image, dpi=300) # Zapis do pliku PNG w wysokiej jakosci laboratoryjnej (300 DPI)

print(f"🖼️ Sukces! Profesjonalny wykres analityczny zostal zapisany jako: {output_image}")
plt.show()