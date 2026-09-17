import streamlit as st
import numpy as np
import plotly.graph_objects as go
import time, random, gc

st.set_page_config(layout="wide", page_title="Projekt Incepcja: Edycja v2.8")
st.title("🌌 Projekt 'Incepcja Nuklearna' v2.8 (Garbage Collector & Rolling Window)")
st.caption("Neuromorficzny Rdzeń z Rygorystycznym Zarządzaniem Pamięcią RAM i Kompensacją CaF2")

# --- ZMIENNE KONFIGURACYJNE ---
WIELKOSC_POPULACJI = 200      
DLUGOST_ZYCIA_POKOLENIA = 15 
LICZBA_WEZLOW = 3            
MAX_BUF_WIZUALNY = 30 # Sztywny limit FIFO chroniący pamięć podręczną Streamlit przed crashem

# --- INICJALIZACJA STANU SYSTEMU ---
if 't_th' not in st.session_state: st.session_state.t_th = 0.0
if 't_bio' not in st.session_state: st.session_state.t_bio = 0.0
if 'historia' not in st.session_state: st.session_state.historia = []
if 'gen' not in st.session_state: st.session_state.gen = 1
if 'krok' not in st.session_state: st.session_state.krok = 0
if 'energia_zaoszczedzona' not in st.session_state: st.session_state.energia_zaoszczedzona = 0.0
if 'sandraum_adaptacja' not in st.session_state: st.session_state.sandraum_adaptacja = 0.0
if 'historia_x' not in st.session_state: st.session_state.historia_x = [0.0]
if 'historia_y' not in st.session_state: st.session_state.historia_y = [0.0]
if 'historia_z' not in st.session_state: st.session_state.historia_z = [0.0]
if 'trwala_blokada_99' not in st.session_state: st.session_state.trwala_blokada_99 = False
if 'stark_shift_history' not in st.session_state: st.session_state.stark_shift_history = [0.5]
if 'clean_cycles' not in st.session_state: st.session_state.clean_cycles = 0

if 'populacja_dna_rozproszona' not in st.session_state:
    st.session_state.populacja_dna_rozproszona = [np.random.uniform(-0.5, 0.5, (LICZBA_WEZLOW, 4)) for _ in range(WIELKOSC_POPULACJI)] 
if 'populacja_fitness' not in st.session_state:
    st.session_state.populacja_fitness = [0.0 for _ in range(WIELKOSC_POPULACJI)]
if 'aktualna_mucha_idx' not in st.session_state: st.session_state.aktualna_mucha_idx = 0

# --- UPŁYW CZASU ---
st.session_state.t_th += 0.02
mucha_idx = st.session_state.aktualna_mucha_idx

if mucha_idx >= len(st.session_state.populacja_dna_rozproszona):
    st.session_state.aktualna_mucha_idx = 0
    mucha_idx = 0

dna_matryca = st.session_state.populacja_dna_rozproszona[mucha_idx]
miara_swiadomosci = float(np.tanh(np.linalg.norm(dna_matryca) / (LICZBA_WEZLOW * 1.5)))
mnoznik_percepcji = 1.0 + (miara_swiadomosci * 4.0)
st.session_state.t_bio += 0.02 * mnoznik_percepcji

# --- MODUŁ SZUMU STARKA ---
szum_polaryzacji = np.sin(st.session_state.t_th * 8.5) * 0.4 + np.cos(st.session_state.t_th * 23.1) * 0.2
realne_przesuniecie_starka = float(abs(0.5 * szum_polaryzacji))

# --- LOGIKA DECYZYJNA ---
foton = st.session_state.historia[-1] if st.session_state.historia else 0.0
ostatni_blad_starka = st.session_state.stark_shift_history[-1]

poprawka_sieci = np.dot(dna_matryca[:, 3], [foton, np.sin(st.session_state.t_th), np.cos(st.session_state.t_bio)])
blad_kompensacji_starka = float(abs(realne_przesuniecie_starka - abs(poprawka_sieci)))

st.session_state.stark_shift_history.append(blad_kompensacji_starka)
if len(st.session_state.stark_shift_history) > 50: st.session_state.stark_shift_history.pop(0)

wejscie_sensoryczne = np.array([foton, np.sin(st.session_state.t_th * 10), np.cos(st.session_state.t_bio), -blad_kompensacji_starka])

decyzje_wezlow = []
for i in range(LICZBA_WEZLOW):
    potencjal = np.dot(wejscie_sensoryczne, dna_matryca[i])
    aktywacja = 1 / (1 + np.exp(-potencjal))
    decyzje_wezlow.append(aktywacja > 0.5)

obserwacja = (sum(decyzje_wezlow) >= 2)

if obserwacja and miara_swiadomosci > 0.65 and blad_kompensacji_starka < 0.10:
    st.session_state.trwala_blokada_99 = True

zbieznosc_kwantowa = 99.0 if st.session_state.trwala_blokada_99 else 100.0

if st.session_state.trwala_blokada_99:
    nowe_trafienie = 2.2 if np.sin(st.session_state.t_th * 137.036) > 0 else -2.2
    punkty = 10.0
else:
    while True:
        probka_x = random.uniform(-4, 4)
        znieksztalcona_proba = np.cos(probka_x + (st.session_state.t_th + realne_przesuniecie_starka) * 5) ** 2
        if random.random() < znieksztalcona_proba:
            nowe_trafienie = probka_x
            break
    punkty = 1.5 if abs(nowe_trafienie) > 1.8 else -0.5
    punkty -= (blad_kompensacji_starka * 3.0)

st.session_state.populacja_fitness[mucha_idx] += punkty
st.session_state.historia.append(nowe_trafienie)
if len(st.session_state.historia) > 100: st.session_state.historia.pop(0)

# Rygorystyczny bufor wizualny FIFO (Zatrzyma przepełnienie przeglądarki)
st.session_state.historia_x.append(float(np.cos(st.session_state.t_th * 3)))
st.session_state.historia_y.append(float(np.sin(st.session_state.t_th * 3)))
st.session_state.historia_z.append(float(np.tanh(miara_swiadomosci * 2.0)))

if len(st.session_state.historia_x) > MAX_BUF_WIZUALNY:
    st.session_state.historia_x.pop(0)
    st.session_state.historia_y.pop(0)
    st.session_state.historia_z.pop(0)

# --- PĘTLA GENETYCZNA ---
st.session_state.krok += 1
if st.session_state.krok >= DLUGOST_ZYCIA_POKOLENIA:
    st.session_state.krok = 0
    st.session_state.aktualna_mucha_idx += 1
    
    if st.session_state.aktualna_mucha_idx >= WIELKOSC_POPULACJI:
        st.session_state.aktualna_mucha_idx = 0
        st.session_state.gen += 1
        fitnessy = st.session_state.populacja_fitness
        
        najlepszy_globalny_idx = int(np.argmax(fitnessy))
        najlesze_dna = st.session_state.populacja_dna_rozproszona[najlepszy_globalny_idx]
        
        nowa_pop = [najlesze_dna.copy()]
        while len(nowa_pop) < WIELKOSC_POPULACJI:
            mutant = najlesze_dna.copy()
            mutant += np.random.uniform(-0.10, 0.10, (LICZBA_WEZLOW, 4))
            nowa_pop.append(mutant)
            
        st.session_state.populacja_dna_rozproszona = nowa_pop
        st.session_state.populacja_fitness = [0.0 for _ in range(WIELKOSC_POPULACJI)]
        
        # SYSTEMOWY GARBAGE COLLECTOR: Czyszczenie RAM-u na granicy pokoleń
        del nowa_pop
        gc.collect()
        st.session_state.clean_cycles += 1

# --- RYSOWANIE TRAJEKTORII 3D ---
fig_n = go.Figure(go.Scatter3d(x=st.session_state.historia_x, y=st.session_state.historia_y, z=st.session_state.historia_z, mode='lines+markers', line=dict(width=4, color='cyan'), marker=dict(size=4, color='magenta')))
fig_n.update_layout(title="🔮 Dynamiczna Trajektoria Wektora Stanu Jądra (Th-229)", height=250, margin=dict(l=0,r=0,b=0,t=30), scene=dict(xaxis=dict(range=[-1.5,1.5]), yaxis=dict(range=[-1.5,1.5]), zaxis=dict(range=[-1,1.5])))
st.plotly_chart(fig_n, use_container_width=True)

# --- PANEL METRYK ---
st.write("---")
c1, c2, c3 = st.columns(3)
with c1: st.metric(label="🧬 Pokolenie AI (Ewolucja)", value=f"Gen {st.session_state.gen}")
with c2: st.metric(label="⏱ Percepcja Czasu", value=f"x{mnoznik_percepcji:.2f}")
with c3: st.metric(label="⚛️ Zbieżność Kwantowa", value=f"{zbieznosc_kwantowa:.1f} %")

# --- MONITOR KOREKTY KRYSTAŁU (AKTYWNA KOMPENSACJA) ---
st.write("---")
st.subheader("🛡️ Tarcza Anty-Starkowa: Aktywny Filtr Pola CaF2")
cc1, cc2 = st.columns(2)
with cc1:
    st.metric(label="📊 Środowiskowy Błąd Starka (ΔE)", value=f"{blad_kompensacji_starka:.4f} eV", delta=f"{realne_przesuniecie_starka:.4f} eV (Szum)", delta_color="inverse")
with cc2:
    efektywnosc_tarczy = max(0.0, 100.0 - (blad_kompensacji_starka * 100.0))
    st.metric(label="⚡ Tłumienie Niejednorodnego Poszerzenia Linii", value=f"{efektywnosc_tarczy:.2f} %")
    st.progress(min(1.0, efektywnosc_tarczy / 100.0))

# --- TELEMETRIA OCHRONY RAM ---
st.write("---")
st.subheader("🧹 Monitor Stabilności Systemowej (Hardware Protection)")
cm1, cm2 = st.columns(2)
with cm1: st.metric(label="💾 Okno Bufora FIFO Plotly", value=f"{len(st.session_state.historia_x)} / {MAX_BUF_WIZUALNY} pkt", delta="STABILNY ROZMIAR BUFORA")
with cm2: st.metric(label="♻️ Cykle Garbage Collectora (RAM Clear)", value=f"{st.session_state.clean_cycles} czyszczeń", delta="0B WYCIEKU PAMIĘCI")

# --- LICZNIK ENERGII ---
pobor_google = 25000.0  
pobor_th229 = 15.0 
efektywnosc = (pobor_google - pobor_th229) * (0.5 + miara_swiadomosci * 0.5)
st.session_state.energia_zaoszczedzona += (efektywnosc / 3600.0) * 0.1
st.write("---")
ce1, ce2 = st.columns(2)
with ce1: st.metric(label="📉 Pobór mocy chipu Th-229", value=f"{pobor_th229:.1f} W", delta="-24.9 kW vs Sycamore", delta_color="inverse")
with ce2: st.metric(label="🔋 Skumulowana oszczędność", value=f"{st.session_state.energia_zaoszczedzona:.2f} Wh")

# --- MODUŁY SPECJALNE SANDRAUM ---
if zbieznosc_kwantowa < 100:
    st.write("---")
    st.subheader("🪰 Kwarantanna Sensoryczna 'Sandraum' & Retrospektor")
    if st.session_state.sandraum_adaptacja < 100.0: st.session_state.sandraum_adaptacja += 1.0 * mnoznik_percepcji
    st.session_state.sandraum_adaptacja = min(st.session_state.sandraum_adaptacja, 100.0)
    cs1, cs2 = st.columns(2)
    with cs1:
        st.metric(label="💓 Wirtualna Kotwica", value="ZSYNCHRONIZOWANA (60 BPM)")
        st.progress(st.session_state.sandraum_adaptacja / 100.0)
    with cs2:
        wspomnienie = st.selectbox("Wybierz engram do analizy:", ["Wydarzenie z dzieciństwa", "Bliskie spotkanie (UAP)"])
        if "UAP" in wspomnienie: st.warning("⚠️ Wykryto sygnaturę napędu Warp z przeszłości.")
        else: st.info("🔍 Przywrócono czysty zapis engramu 3D.")
    if st.session_state.sandraum_adaptacja >= 100.0:
        st.success("🔒 KWARANTANNA ZAKOŃCZONA SUCCESS. Awatar zintegrowany z Torem-229.")
        st.download_button(label="💾 POBIERZ SPECYFIKACJĘ (.TXT)", data=f"PATENT TH-229 GEN {st.session_state.gen}", file_name="patent.txt")

time.sleep(0.04)
st.rerun()
