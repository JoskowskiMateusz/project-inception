#  Project Inception (v4.4-QuantumCore) - Mössbauer Analytics Utility
#  Copyright (C) 2026 Developer & Consultant, M.Sc. Eng.
#  Licensed under the GNU GPL v3 - Open Science Initiative

import pandas as pd
import matplotlib.pyplot as plt
import os

# --- 1. WCZYTANIE SUROWEJ TELEMETRII KWANTOWEJ Z C++ ---
plik_danych = "raport_hpc.txt"

if not os.path.exists(plik_danych):
    print(f"❌ Blad: Nie znaleziono pliku {plik_danych} w biezacym katalogu!")
    print("ℹ️ Upewnij sie, ze najpierw uruchomiles program w C++ (v4.4) i wygenerowales logi.")
    exit()

df = pd.read_csv(plik_danych)
print(f"✅ Pomyslnie wczytano {len(df)} rekordow precyzyjnej telemetrii Mössbauera.")

# --- 2. INICJALIZACJA TRZYPANELOWEGO LABORATORIUM GRAFICZNEGO ---
fig, axes = plt.subplots(1, 3, figsize=(18, 5))

# Wykres 1: Zbieznosc wspolczynnika spojnosci (STDP Connectome)
axes[0].plot(df['Pokolenie'], df['Wspolczynnik_Spojnosci'], color='cyan', linewidth=1.5, label='Coherence Index')
axes[0].axhline(y=0.755482, color='magenta', linestyle='--', alpha=0.7, label='Target Homeostasis')
axes[0].set_title("🧬 Neuromorphic Core Convergence (STDP Loop)")
axes[0].set_xlabel("Generation (Gen)")
axes[0].set_ylabel("Synaptic Coherence Index (j.z.)")
axes[0].grid(True, linestyle=':', alpha=0.6)
axes[0].legend()

# Wykres 2: Skumulowany zysk infrastrukturalny (Wh -> MWh)
df['Skumulowana_Energia_MWh'] = df['Skumulowana_Energia_Wh'] / 1000000.0
axes[1].plot(df['Pokolenie'], df['Skumulowana_Energia_MWh'], color='lime', linewidth=2, label='Saved Power')
axes[1].set_title("🔋 Accumulated Net Infrastructure Power Savings")
axes[1].set_xlabel("Generation (Gen)")
axes[1].set_ylabel("Saved Infrastructure Power (MWh)")
axes[1].grid(True, linestyle=':', alpha=0.6)
axes[1].legend()

# Wykres 3: Krzywa ewolucji kwantowego czynnika Debye'a-Wallera (Mössbauera)
axes[2].plot(df['Pokolenie'], df['Temperatura_K'], color='gold', linewidth=1.8, label='Mössbauer Factor (f_DW)')
axes[2].axhline(y=1.0, color='red', linestyle=':', alpha=0.5, label='Ideal Recoil-free Limit (1.0)')
axes[2].set_title("🧘 Quantum Debye-Waller Coherence Profile")
axes[2].set_xlabel("Generation (Gen)")
axes[2].set_ylabel("Probability of Recoil-free Emission (f_DW)")
axes[2].grid(True, linestyle=':', alpha=0.6)
axes[2].legend()

# --- 3. EXPORT DO PLIKU PNG HIGH-DPI ---
plt.tight_layout()
output_image = "inception_v4_4_quantum_metrics.png"
plt.savefig(output_image, dpi=300) # Laboratoryjna jakosc 300 DPI dla rekruterow i recenzentow

print(f"🖼️ Sukces! Potrojny wykres analityczny Quantum Core zostal zapisany jako: {output_image}")
plt.show()
