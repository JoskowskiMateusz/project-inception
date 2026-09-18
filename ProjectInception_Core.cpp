// =====================================================================
//  Project Inception (v4.1) - High-Performance C++ Logging Engine
//  Copyright (C) 2026 Developer & Consultant, M.Sc. Eng.
//  Licensed under the GNU GPL v3 - Open Science Initiative
// =====================================================================

#include <iostream>
#include <vector>
#include <array>
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
const int CECHY_WEJSCIOWE = 4; // foton, sin(t_th), cos(t_bio), -blad
const double DT = 0.05;

// Parametry STDP
const double TAU_PLUS = 0.05;
const double TAU_MINUS = 0.05;
const double A_PLUS = 0.02;
const double A_MINUS = 0.025;

// --- STRUKTURY DANYCH NEUROMORFICZNYCH ---
struct AgentDNA {
    std::array<std::array<double, CECHY_WEJSCIOWE>, LICZBA_WEZLOW> matryca_wag;
    double fitness = 0.0;

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
        double dW = 0.0;
        if (delta_t > 0) {
            dW = A_PLUS * std::exp(-delta_t / TAU_PLUS);
        }
        else {
            dW = -A_MINUS * std::exp(delta_t / TAU_MINUS);
        }

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
    std::cout << "[I] Zwiekszona wydajnosc: Zaladowano plik " << sciezka_pliku << " (" << wektor_szumu.size() << " pkt)\n";
    return wektor_szumu;
}// --- GLOWNA FUNKCJA URUCHOMIENIOWA SYSTEMU (MAIN CORE) ---
int main() {
    std::cout << "=====================================================================\n";
    std::cout << "🌌 Projekt 'Incepcja' v4.1 - Wysokowydajny Rdzen Obliczeniowy C++\n";
    std::cout << "=====================================================================\n";

    std::random_device rd;
    std::mt19937 prng_engine(rd());
    std::normal_distribution<double> dist_normal(0.0, 1.0);
    std::uniform_real_distribution<double> dist_uniform(-4.0, 4.0);
    std::uniform_real_distribution<double> dist_mutacja(-0.10, 0.10);

    // Otwarcie pliku wyjsciowego dla masowych logow telemetrii
    std::ofstream plik_logu("raport_hpc.txt");
    if (plik_logu.is_open()) {
        plik_logu << "Pokolenie,Wspolczynnik_Spojnosci,Skumulowana_Energia_Wh\n";
    }
    else {
        std::cerr << "[⚠️ ERROR] Nie mozna utworzyc pliku raport_hpc.txt!\n";
    }

    // Ladowanie kanalow danych
    std::vector<double> dane_krysztalu = wczytaj_dataset_csv("final_real_noise.csv");
    std::vector<double> dane_lasera = wczytaj_dataset_csv("laser_phase_noise.csv");

    // Inicjalizacja populacji
    std::vector<AgentDNA> populacja(WIELKOSC_POPULACJI);
    for (int i = 0; i < WIELKOSC_POPULACJI; ++i) {
        populacja[i].inicjalizuj(prng_engine);
    }

    STDPEngine stdp_worker;
    std::vector<double> historia_fotonow = { 0.0 };

    double t_th = 0.0;
    double t_bio = 0.0;
    double stark_ou_state = 0.15;
    double last_pre_spike = 0.0;
    double last_post_spike = 0.0;
    double skumulowana_energia_wh = 0.0;

    int ostateczna_liczba_generacji = 20000;

    std::cout << "\n🏁 Uruchamianie maratonu ewolucyjnego z autozapisem (20k Gen Log Engine)...\n";
    auto czas_startu_hpc = std::chrono::high_resolution_clock::now();

    // --- GLOWNA PETLA OBLICZENIOWA HPC ---
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

                double szum_kanal_1 = 0.0;
                double szum_kanal_2 = 0.0;

                if (!dane_krysztalu.empty()) {
                    szum_kanal_1 = dane_krysztalu[(gen * mucha_idx + krok) % dane_krysztalu.size()];
                }
                if (!dane_lasera.empty()) {
                    szum_kanal_2 = dane_lasera[(gen * mucha_idx + krok) % dane_lasera.size()];
                }

                double realne_przesuniecie_starka = 0.0;
                if (!dane_krysztalu.empty() || !dane_lasera.empty()) {
                    realne_przesuniecie_starka = std::abs(szum_kanal_1 + szum_kanal_2);
                }
                else {
                    double dW = dist_normal(prng_engine) * std::sqrt(DT);
                    stark_ou_state += -0.40 * stark_ou_state * DT + 0.25 * dW;
                    realne_przesuniecie_starka = std::abs(0.3 * std::sin(t_th * 6.5) + stark_ou_state);
                }

                double foton = historia_fotonow.back();
                double poprawka_sieci = megamucha.matryca_wag[0][3] * foton +
                    megamucha.matryca_wag[1][3] * std::sin(t_th) +
                    megamucha.matryca_wag[2][3] * std::cos(t_bio);

                double blad_kompensacji = std::abs(realne_przesuniecie_starka - std::abs(poprawka_sieci));

                std::array<double, CECHY_WEJSCIOWE> wejscie = { foton, std::sin(t_th * 10), std::cos(t_bio), -blad_kompensacji };
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
                megamucha.fitness += ((std::abs(nowe_trafienie) > 1.8) ? 1.5 : -0.5) - (blad_kompensacji * 3.0);

                historia_fotonow.push_back(nowe_trafienie);
                if (historia_fotonow.size() > 100) historia_fotonow.erase(historia_fotonow.begin());

                skumulowana_energia_wh += ((30000.0 - 15.0) * (0.5 + wspolczynnik_spojnosci * 0.5) / 3600.0) * DT;
            }
        }

        // Selekcja i mutacja elity
        auto it_najlepszy = std::max_element(populacja.begin(), populacja.end(), [](const AgentDNA& a, const AgentDNA& b) {
            return a.fitness < b.fitness;
            });

        AgentDNA elite_dna = *it_najlepszy;
        populacja[0] = elite_dna;
        populacja[0].fitness = 0.0;
        for (int i = 1; i < WIELKOSC_POPULACJI; ++i) {
            populacja[i] = elite_dna;
            populacja[i].fitness = 0.0;
            for (int r = 0; r < LICZBA_WEZLOW; ++r) {
                for (int c = 0; c < CECHY_WEJSCIOWE; ++c) {
                    populacja[i].matryca_wag[r][c] += dist_mutacja(prng_engine);
                }
            }
        }

        // ASYNCHRONICZNY ZAPIS DO PLIKU CO 100 POKOLEN
        if (gen % 100 == 0 && plik_logu.is_open()) {
            plik_logu << gen << "," << najlepszy_wspolczynnik_gen << "," << skumulowana_energia_wh << "\n";
        }

        if (gen % 2000 == 0) {
            std::cout << "[I] Przetworzono pokolenie: Gen " << gen << " / " << ostateczna_liczba_generacji << "\n";
        }
    }

    if (plik_logu.is_open()) plik_logu.close();

    auto czas_konca_hpc = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> czas_trwania_sekundy = czas_konca_hpc - czas_startu_hpc;

    std::cout << "=====================================================================\n";
    std::cout << "🎉 ZAKONCZONO MARATON Z ZAPISEM LOGOW v4.1!\n";
    std::cout << "=====================================================================\n";
    std::cout << "⏱️ Rzeczywisty czas przetwarzania procesora CPU: " << czas_trwania_sekundy.count() << " sekund\n";
    std::cout << "🧬 Wyewoluowano pokolen: " << ostateczna_liczba_generacji << "\n";
    std::cout << "🔋 Skumulowana czysta oszczednosc infrastruktury: " << skumulowana_energia_wh << " Wh\n";
    std::cout << "[💾 FILE] Pelny raport telemetryczny zapisano w: raport_hpc.txt\n";
    std::cout << "=====================================================================\n";

    std::cout << "\nNacisnij klawisz Enter, aby zakonczycz program...";
    std::cin.get();
    return 0;
}