// =====================================================================
//  Project Inception (v4.2-MIMO) - High-Performance Stark-Zeeman Core
//  Copyright (C) 2026 Developer & Consultant, M.Sc. Eng.
//  Licensed under the GNU GPL v3 - Open Science Initiative
// =====================================================================

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

// --- STALE SYSTEMOWE ---
const int WIELKOSC_POPULACJI = 200;
const int DLUGOST_ZYCIA_POKOLENIA = 15;
const int LICZBA_WEZLOW = 3;
const int CECHY_WEJSCIOWE = 5; // foton, sin(t_th), cos(t_bio), -blad_stark, -blad_zeeman
const double DT = 0.05;

// Indeksy synaptyczne wektora sensorycznego
const int IDX_FOTON = 0;
const int IDX_SIN_T = 1;
const int IDX_COS_T = 2;
const int IDX_BLAD_STARK = 3;
const int IDX_BLAD_ZEEMAN = 4;

// Parametry STDP
const double TAU_PLUS = 0.05;
const double TAU_MINUS = 0.05;
const double A_PLUS = 0.02;
const double A_MINUS = 0.025;

// --- STRUKTURY DANYCH NEUROMORFICZNYCH (MIMO TENSOR 3x5) ---
struct AgentDNA {
    std::vector<std::vector<double>> matryca_wag;
    double fitness = 0.0;

    AgentDNA() {
        matryca_wag.resize(LICZBA_WEZLOW, std::vector<double>(CECHY_WEJSCIOWE, 0.0));
        fitness = 0.0;
    }

    void inicjalizuj(std::mt19937& gen) {
        std::uniform_real_distribution<double> dis(-0.5, 0.5);
        for (int i = 0; i < LICZBA_WEZLOW; ++i) {
            for (int j = 0; j < CECHY_WEJSCIOWE; ++j) {
                matryca_wag[i][j] = dis(gen);
            }
        }
        fitness = 0.0;
    }

    double oblicz_wspolczynnik_spojnosci() const {
        double norma_frobeniusa = 0.0;
        for (int i = 0; i < LICZBA_WEZLOW; ++i) {
            for (int j = 0; j < CECHY_WEJSCIOWE; ++j) {
                norma_frobeniusa += matryca_wag[i][j] * matryca_wag[i][j];
            }
        }
        return std::tanh(std::sqrt(norma_frobeniusa) / (LICZBA_WEZLOW * 1.5));
    }
};

class STDPEngine {
private:
    double last_pre_spike = 0.0;
    double last_post_spike = 0.0;

public:
    void aplikuj_plastycznosc(AgentDNA& agent, double t_bio, double delta_t) {
        double dW = (delta_t > 0) ? (A_PLUS * std::exp(-delta_t / TAU_PLUS)) : (-A_MINUS * std::exp(delta_t / TAU_MINUS));

        for (int i = 0; i < LICZBA_WEZLOW; ++i) {
            for (int j = 0; j < CECHY_WEJSCIOWE; ++j) {
                agent.matryca_wag[i][j] += dW * 0.05;
                agent.matryca_wag[i][j] = std::max(-1.5, std::min(1.5, agent.matryca_wag[i][j]));
            }
        }
    }
};

// --- FUNKCJA PARSERA PLIKOW CSV ---
std::vector<double> wczytaj_dataset_csv(const std::string& sciezka_pliku) {
    std::vector<double> wektor_szumu;
    std::ifstream plik(sciezka_pliku);

    if (!plik.is_open()) {
        std::cerr << "[W] Nie znaleziono pliku: " << sciezka_pliku << ". Fallback aktywny.\n";
        return wektor_szumu;
    }

    std::string linia;
    std::getline(plik, linia); // Pomin naglowek

    while (std::getline(plik, linia)) {
        std::stringstream ss(linia);
        std::string cell_idx, cell_szum;
        if (std::getline(ss, cell_idx, ',') && std::getline(ss, cell_szum, ',')) {
            try {
                wektor_szumu.push_back(std::stod(cell_szum));
            }
            catch (...) {}
        }
    }
    plik.close();
    std::cout << "[I] Ingestion MIMO: Zaladowano plik " << sciezka_pliku << " (" << wektor_szumu.size() << " pkt)\n";
    return wektor_szumu;
}// --- GLOWNA FUNKCJA URUCHOMIENIOWA SYSTEMU (MAIN CORE) ---
int main() {
    std::cout << "=====================================================================\n";
    std::cout << "🌌 Projekt 'Incepcja' v4.2-MIMO - Silnik Stark-Zeeman C++\n";
    std::cout << "=====================================================================\n";

    std::random_device rd;
    std::mt19937 prng_engine(rd());
    std::normal_distribution<double> dist_normal(0.0, 1.0);
    std::uniform_real_distribution<double> dist_uniform(-4.0, 4.0);
    std::uniform_real_distribution<double> dist_mutacja(-0.10, 0.10);

    // Rozbudowana naglowkowa struktura logu pod Python Analytics
    std::ofstream plik_logu("raport_hpc.txt");
    if (plik_logu.is_open()) {
        plik_logu << "Pokolenie,Wspolczynnik_Spojnosci,Skumulowana_Energia_Wh,Temperatura_K,Blad_Zeeman\n";
    }

    std::vector<double> dane_krysztalu = wczytaj_dataset_csv("final_real_noise.csv");
    std::vector<double> dane_lasera = wczytaj_dataset_csv("laser_phase_noise.csv");

    std::vector<AgentDNA> populacja(WIELKOSC_POPULACJI);
    for (int i = 0; i < WIELKOSC_POPULACJI; ++i) {
        populacja[i].inicjalizuj(prng_engine);
    }

    STDPEngine stdp_worker;
    std::vector<double> historia_fotonow = { 0.0 };

    double t_th = 0.0;
    double t_bio = 0.0;
    double stark_ou_state = 0.15;
    double zeeman_ou_state = 0.10;
    double last_pre_spike = 0.0;
    double last_post_spike = 0.0;
    double skumulowana_energia_wh = 0.0;

    double temperatura_K = 293.15;
    const double BAZOWA_TEMPERATURA = 293.15;

    int ostateczna_liczba_generacji = 100000;
    double monitor_ostatni_blad_zeeman = 0.20;

    std::cout << "\n🏁 Uruchamianie maratonu MIMO 100k Gen (Stark + Zeeman Adaptive Loop)...\n";
    auto czas_startu_hpc = std::chrono::high_resolution_clock::now();

    for (int gen = 1; gen <= ostateczna_liczba_generacji; ++gen) {
        double najlepszy_wspolczynnik_gen = 0.0;

        for (int mucha_idx = 0; mucha_idx < WIELKOSC_POPULACJI; ++mucha_idx) {
            AgentDNA& megamucha = populacja[mucha_idx];
            double wspolczynnik_spojnosci = megamucha.oblicz_wspolczynnik_spojnosci();
            if (wspolczynnik_spojnosci > najlepszy_wspolczynnik_gen) {
                najlepszy_wspolczynnik_gen = wspolczynnik_spojnosci;
            }
            double dynamiczne_skalowanie = 1.0 + (wspolczynnik_spojnosci * 4.0);

            for (int krok = 0; krok < DLUGOST_ZYCIA_POKOLENIA; ++krok) {
                t_th += DT;
                t_bio += DT * dynamiczne_skalowanie;

                // Termodynamika kryształu
                temperatura_K += 0.00002 * (30.0 - (wspolczynnik_spojnosci * 10.0)) - 0.00001 * (temperatura_K - BAZOWA_TEMPERATURA);
                double mnoznik_termiczny = std::sqrt(temperatura_K / BAZOWA_TEMPERATURA);

                // Import szumu elektrycznego (Stark)
                double szum_stark_1 = 0.0;
                double szum_stark_2 = 0.0;
                if (!dane_krysztalu.empty()) szum_stark_1 = dane_krysztalu[(gen * mucha_idx + krok) % dane_krysztalu.size()] * mnoznik_termiczny;
                if (!dane_lasera.empty()) szum_stark_2 = dane_lasera[(gen * mucha_idx + krok) % dane_lasera.size()];
                double realny_stark = std::abs(szum_stark_1 + szum_stark_2);

                // Dryf magnetyczny (Efekt Zeemana)
                double dW_magnetyczne = dist_normal(prng_engine) * std::sqrt(DT);
                zeeman_ou_state += -0.25 * zeeman_ou_state * DT + 0.15 * dW_magnetyczne;
                double realny_zeeman = std::abs(0.2 * std::cos(t_th * 4.5) + zeeman_ou_state);

                // Korekcja wielokanalowa MIMO
                double foton = historia_fotonow.back();
                double poprawka_stark = 0.0;
                double poprawka_zeeman = 0.0;

                for (int i = 0; i < LICZBA_WEZLOW; ++i) {
                    poprawka_stark += megamucha.matryca_wag[i][IDX_FOTON] * foton + megamucha.matryca_wag[i][IDX_SIN_T] * std::sin(t_th);
                    poprawka_zeeman += megamucha.matryca_wag[i][IDX_COS_T] * std::cos(t_bio);
                }

                double blad_stark = std::abs(realny_stark - std::abs(poprawka_stark));
                double blad_zeeman = std::abs(realny_zeeman - std::abs(poprawka_zeeman));
                monitor_ostatni_blad_zeeman = blad_zeeman;

                // Ładowanie zintegrowanego wektora sensorycznego 5D
                std::vector<double> wejscie(CECHY_WEJSCIOWE);
                wejscie[IDX_FOTON] = foton;
                wejscie[IDX_SIN_T] = std::sin(t_th * 10);
                wejscie[IDX_COS_T] = std::cos(t_bio);
                wejscie[IDX_BLAD_STARK] = -blad_stark;
                wejscie[IDX_BLAD_ZEEMAN] = -blad_zeeman;

                if (std::abs(foton) > 1.5) last_pre_spike = t_bio;

                int aktywne_wezly = 0;
                for (int i = 0; i < LICZBA_WEZLOW; ++i) {
                    double potencjal = 0.0;
                    for (int j = 0; j < CECHY_WEJSCIOWE; ++j) {
                        potencjal += wejscie[j] * megamucha.matryca_wag[i][j];
                    }
                    if ((1.0 / (1.0 + std::exp(-potencjal))) > 0.5) aktywne_wezly++;
                }

                if (aktywne_wezly >= 2) {
                    last_post_spike = t_bio;
                    stdp_worker.aplikuj_plastycznosc(megamucha, t_bio, last_post_spike - last_pre_spike);
                }

                double nowe_trafienie = dist_uniform(prng_engine);
                megamucha.fitness += ((std::abs(nowe_trafienie) > 1.8) ? 1.5 : -0.5) - (blad_stark * 2.5) - (blad_zeeman * 3.5);

                historia_fotonow.push_back(nowe_trafienie);
                if (historia_fotonow.size() > 100) historia_fotonow.erase(historia_fotonow.begin());

                skumulowana_energia_wh += ((30000.0 - 15.0) * (0.5 + wspolczynnik_spojnosci * 0.5) / 3600.0) * DT;
            }
        }

        // Ewolucyjny dobor naturalny elity (Bezpieczne głębokie kopiowanie wektorów)
        auto it_najlepszy = std::max_element(populacja.begin(), populacja.end(), [](const AgentDNA& a, const AgentDNA& b) {
            return a.fitness < b.fitness;
            });

        AgentDNA elite_dna = *it_najlepszy;

        for (int i = 0; i < WIELKOSC_POPULACJI; ++i) {
            populacja[i].matryca_wag = elite_dna.matryca_wag;
            populacja[i].fitness = 0.0;

            if (i > 0) { // Mutacja dla potomstwa
                for (int r = 0; r < LICZBA_WEZLOW; ++r) {
                    for (int c = 0; c < CECHY_WEJSCIOWE; ++c) {
                        populacja[i].matryca_wag[r][c] += dist_mutacja(prng_engine);
                    }
                }
            }
        }

        if (gen % 200 == 0 && plik_logu.is_open()) {
            plik_logu << gen << "," << najlepszy_wspolczynnik_gen << "," << skumulowana_energia_wh << "," << temperatura_K << "," << monitor_ostatni_blad_zeeman << "\n";
        }

        if (gen % 10000 == 0) {
            std::cout << "[I] Maraton MIMO: Gen " << gen << " / " << ostateczna_liczba_generacji << " | Blad Zeeman: " << monitor_ostatni_blad_zeeman << "\n";
        }
    }

    if (plik_logu.is_open()) plik_logu.close();

    auto czas_konca_hpc = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> czas_trwania_sekundy = czas_konca_hpc - czas_startu_hpc;

    std::cout << "=====================================================================\n";
    std::cout << "🎉 ZAKONCZONO MARATON MIMO (STARK + ZEEMAN FULL PHYSICS) v4.2!\n";
    std::cout << "=====================================================================\n";
    std::cout << "⏱️ Rzeczywisty czas przetwarzania procesora CPU: " << czas_trwania_sekundy.count() << " sekund\n";
    std::cout << "🧬 Wyewoluowano pokolen: " << ostateczna_liczba_generacji << "\n";
    std::cout << "🔋 Skumulowana czysta oszczednosc infrastruktury: " << skumulowana_energia_wh << " Wh\n";
    std::cout << "🌡️ Koncowa temperatura ukladu: " << temperatura_K << " K\n";
    std::cout << "🧲 Ostatni blad tarczy magnetycznej (Zeeman): " << monitor_ostatni_blad_zeeman << " j.z.\n";
    std::cout << "[💾 FILE] Zapisano pelny raport wielokanalowy w: raport_hpc.txt\n";
    std::cout << "=====================================================================\n";

    std::cout << "\nNacisnij klawisz Enter, aby zakonczyc program...";
    std::cin.get();
    return 0;
}
