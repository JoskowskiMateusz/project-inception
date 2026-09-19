// =====================================================================
//  Project Inception (v4.7-LindbladianCore) - Pure ASCII Edition
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
#include <complex>

// Definiujemy jawny typ zespolony dla rygoru obliczen numerycznych
using cplx = std::complex<double>;

// --- STALE SYSTEMOWE I KWANTOWE ---
const int WIELKOSC_POPULACJI = 200;
const int DLUGOST_ZYCIA_POKOLENIA = 15;
const int LICZBA_WEZLOW = 3;
const int CECHY_WEJSCIOWE = 6; // foton, sin(t_th), cos(t_bio), -blad_stark, -blad_zeeman, last_dd_t

// Indeksy wektora sensorycznego MIMO 3x6
const int IDX_FOTON = 0;
const int IDX_SIN_T = 1;
const int IDX_COS_T = 2;
const int IDX_BLAD_STARK = 3;
const int IDX_BLAD_ZEEMAN = 4;
const int IDX_LAST_DD_INTERVAL = 5;

// Parametry STDP
const double TAU_PLUS = 0.05;
const double TAU_MINUS = 0.05;
const double A_PLUS = 0.02;
const double A_MINUS = 0.025;
// --- RZETELNY MECHANIZM KWANTOWY (LINDBlAD MASTER EQUATION ENGINE) ---
struct QuantumKubit {
    std::array<std::array<cplx, 2>, 2> m_rho;

    void ustaw_stan_superpozycji() {
        m_rho[0][0] = cplx(0.5, 0.0);
        m_rho[0][1] = cplx(0.5, 0.0);
        m_rho[1][0] = cplx(0.5, 0.0);
        m_rho[1][1] = cplx(0.5, 0.0);
    }

    double oblicz_koherencje_fazowa_t2() const {
        return 2.0 * std::abs(m_rho[0][1]);
    }

    // Wyznaczenie pochodnej d_rho/dt = -i[H, rho] + L*rho*L^dagger - 0.5*{L^dagger*L, rho}
    std::array<std::array<cplx, 2>, 2> oblicz_lindbladian(double omega, const std::array<std::array<cplx, 2>, 2>& rho_stan) const {
        std::array<std::array<cplx, 2>, 2> d_rho;
        cplx I_UNIT(0.0, 1.0);

        // 1. Czlon Hamiltonianu: H = 0.5 * omega * sigma_z
        double h00 = 0.5 * omega;
        double h11 = -0.5 * omega;

        cplx komutator_00 = cplx(0.0, 0.0);
        cplx komutator_01 = (h00 - h11) * rho_stan[0][1];
        cplx komutator_10 = (h11 - h00) * rho_stan[1][0];
        cplx komutator_11 = cplx(0.0, 0.0);

        cplx hamiltonian_part_00 = -I_UNIT * komutator_00;
        cplx hamiltonian_part_01 = -I_UNIT * komutator_01;
        cplx hamiltonian_part_10 = -I_UNIT * komutator_10;
        cplx hamiltonian_part_11 = -I_UNIT * komutator_11;

        // 2. Czlon Dyssypacji Lindblada (Zanik fazy poprzecznej T2 - Gamma = 1.0 / 0.85)
        double gamma = 1.0 / 0.85;
        cplx l_part_00 = cplx(0.0, 0.0);
        cplx l_part_01 = -0.5 * gamma * rho_stan[0][1];
        cplx l_part_10 = -0.5 * gamma * rho_stan[1][0];
        cplx l_part_11 = cplx(0.0, 0.0);

        // Sumowanie pochodnych
        d_rho[0][0] = hamiltonian_part_00 + l_part_00;
        d_rho[0][1] = hamiltonian_part_01 + l_part_01;
        d_rho[1][0] = hamiltonian_part_10 + l_part_10;
        d_rho[1][1] = hamiltonian_part_11 + l_part_11;

        return d_rho;
    }

    // Zapewnienie rygoru hermitowskiego macierzy po operacjach calkowania
    void wymus_hermitowskosc() {
        m_rho[0][0] = cplx(std::real(m_rho[0][0]), 0.0);
        m_rho[1][1] = cplx(std::real(m_rho[1][1]), 0.0);
        m_rho[1][0] = std::conj(m_rho[0][1]);
    }
};
struct KwantowyAktuatorDD {
    // Transformacja unitarna Bramki NOT (Pauliego X): rho_nowe = X * rho * X^dagger
    void zaaplikuj_impuls_pi(QuantumKubit& kubit) {
        std::array<std::array<cplx, 2>, 2> stare = kubit.m_rho;

        // Pelna zamiana populacji miejscami (inwersja stanow jadrowych)
        kubit.m_rho[0][0] = stare[1][1];
        kubit.m_rho[1][1] = stare[0][0];

        // Inwersja fazowa koherencji z zachowaniem hermitowskosci
        kubit.m_rho[0][1] = stare[1][0];
        kubit.m_rho[1][0] = stare[0][1];
    }
};
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
public:
    void aplikuj_plastycznosc(AgentDNA& agent, double delta_t) {
        double dW = (delta_t > 0) ? (A_PLUS * std::exp(-delta_t / TAU_PLUS)) : (-A_MINUS * std::exp(delta_t / TAU_MINUS));
        for (int i = 0; i < LICZBA_WEZLOW; ++i) {
            for (int j = 0; j < CECHY_WEJSCIOWE; ++j) {
                agent.matryca_wag[i][j] += dW * 0.05;
                agent.matryca_wag[i][j] = std::max(-1.5, std::min(1.5, agent.matryca_wag[i][j]));
            }
        }
    }
};
std::vector<double> wczytaj_dataset_csv(const std::string& sciezka_pliku) {
    std::vector<double> wektor_szumu;
    std::ifstream plik(sciezka_pliku);
    if (!plik.is_open()) {
        std::cerr << "[W] Fallback: Brak pliku " << sciezka_pliku << "\n";
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
    return wektor_szumu;
}
// --- GLOWNA FUNKCJA URUCHOMIENIOWA SYSTEMU (MAIN LINDBlAD CORE) ---
int main() {
    std::cout << "=====================================================================\n";
    std::cout << "Projekt Incepcja v4.7 - Rdzen Rownania Master Lindblada (RK4) C++\n";
    std::cout << "=====================================================================\n";

    std::random_device rd;
    std::mt19937 prng_engine(rd());
    std::normal_distribution<double> dist_normal(0.0, 1.0);
    std::uniform_real_distribution<double> dist_uniform(-4.0, 4.0);
    std::uniform_real_distribution<double> dist_mutacja(-0.10, 0.10);

    // Struktura logu pod zaawansowana tomografie stanu kwantowego w Pythonie
    std::ofstream plik_logu("raport_hpc.txt");
    if (plik_logu.is_open()) {
        plik_logu << "Pokolenie,Wspolczynnik_Spojnosci,Skumulowana_Energia_Wh,Kwantowa_Koherencja_T2,Blad_Zeeman,Liczba_Impulsow\n";
    }

    std::vector<double> dane_krysztalu = wczytaj_dataset_csv("final_real_noise.csv");
    std::vector<double> dane_lasera = wczytaj_dataset_csv("laser_phase_noise.csv");

    std::vector<AgentDNA> populacja(WIELKOSC_POPULACJI);
    for (int i = 0; i < WIELKOSC_POPULACJI; ++i) {
        populacja[i].inicjalizuj(prng_engine);
    }

    STDPEngine stdp_worker;
    KwantowyAktuatorDD dd_actuator;
    std::vector<double> historia_fotonow = { 0.0 };

    double t_th = 0.0; double t_bio = 0.0;
    double stark_ou_state = 0.15; double zeeman_ou_state = 0.10;
    double last_pre_spike = 0.0; double last_post_spike = 0.0;
    double skumulowana_energia_wh = 0.0;

    double temperatura_K = 293.15;
    const double BAZOWA_TEMPERATURA = 293.15;
    const double TEMPERATURA_DEBYEA = 410.0;
    const double ENERGIA_ODRZUTU_ER = 0.05;
    double f_dw_mossbauer = 0.50;

    double czas_od_ostatniego_impulsu_dd = 0.0;
    long long skumulowana_liczba_impulsow_dd = 0;
    double wygladzone_dt = 0.05;
    const double ALFA_EMA = 0.04;

    int ostateczna_liczba_generacji = 100000;
    double monitor_ostatni_blad_zeeman = 0.20;
    double monitor_koherencja_t2 = 1.0;

    std::cout << "\nUruchamianie maratonu Lindblad-RK4 Master Equation (100k Gen)...\n";
    auto czas_startu_hpc = std::chrono::high_resolution_clock::now();
    auto punkt_odniesienia_kroku = std::chrono::high_resolution_clock::now();
    for (int gen = 1; gen <= ostateczna_liczba_generacji; ++gen) {
        double najlepszy_wspolczynnik_gen = 0.0;

        for (int mucha_idx = 0; mucha_idx < WIELKOSC_POPULACJI; ++mucha_idx) {
            AgentDNA& megamucha = populacja[mucha_idx];
            double wspolczynnik_spojnosci = megamucha.oblicz_wspolczynnik_spojnosci();
            if (wspolczynnik_spojnosci > najlepszy_wspolczynnik_gen) {
                najlepszy_wspolczynnik_gen = wspolczynnik_spojnosci;
            }
            double dynamiczne_skalowanie = 1.0 + (wspolczynnik_spojnosci * 4.0);

            QuantumKubit thor_cubit;
            thor_cubit.ustaw_stan_superpozycji();

            for (int krok = 0; krok < DLUGOST_ZYCIA_POKOLENIA; ++krok) {
                auto teraz_sprzetowo = std::chrono::high_resolution_clock::now();
                long long nanosekundy = std::chrono::duration_cast<std::chrono::nanoseconds>(teraz_sprzetowo - punkt_odniesienia_kroku).count();
                punkt_odniesienia_kroku = teraz_sprzetowo;

                double surowe_dt = (nanosekundy > 0) ? (static_cast<double>(nanosekundy) / 1e9) : 0.000001;
                surowe_dt *= 2500.0;
                wygladzone_dt = (ALFA_EMA * surowe_dt) + (1.0 - ALFA_EMA) * wygladzone_dt;

                t_th += wygladzone_dt;
                t_bio += wygladzone_dt * dynamiczne_skalowanie;
                czas_od_ostatniego_impulsu_dd += wygladzone_dt;

                temperatura_K += 0.00002 * (30.0 - (wspolczynnik_spojnosci * 10.0)) - 0.00001 * (temperatura_K - BAZOWA_TEMPERATURA);
                double wykladnik_debyea = (-3.0 * ENERGIA_ODRZUTU_ER) / (TEMPERATURA_DEBYEA) * (1.0 + 4.0 * (temperatura_K / TEMPERATURA_DEBYEA));
                f_dw_mossbauer = std::exp(wykladnik_debyea);
                double mnoznik_termiczny_kwantowy = std::sqrt((1.0 - f_dw_mossbauer) * (temperatura_K / BAZOWA_TEMPERATURA));

                double szum_stark_1 = 0.0; double szum_stark_2 = 0.0;
                if (!dane_krysztalu.empty()) szum_stark_1 = dane_krysztalu[(gen * mucha_idx + krok) % dane_krysztalu.size()] * mnoznik_termiczny_kwantowy;
                if (!dane_lasera.empty()) szum_stark_2 = dane_lasera[(gen * mucha_idx + krok) % dane_lasera.size()];
                double realny_stark = std::abs(szum_stark_1 + szum_stark_2);

                double dW_magnetyczne = dist_normal(prng_engine) * std::sqrt(wygladzone_dt);
                zeeman_ou_state += -0.25 * zeeman_ou_state * wygladzone_dt + 0.15 * dW_magnetyczne;
                double realny_zeeman = std::abs(0.2 * std::cos(t_th * 4.5) + zeeman_ou_state);

                // --- INTEGRACJA SOLWERA LINDBlADA RUNGEGO-KUTTY 4. RZEDU (RK4) ---
                double hamiltonian_omega = realny_stark + realny_zeeman;

                std::array<std::array<cplx, 2>, 2> k1 = thor_cubit.oblicz_lindbladian(hamiltonian_omega, thor_cubit.m_rho);

                std::array<std::array<cplx, 2>, 2> rho_k2;
                for (int r = 0; r < 2; ++r) {
                    for (int c = 0; c < 2; ++c) rho_k2[r][c] = thor_cubit.m_rho[r][c] + 0.5 * wygladzone_dt * k1[r][c];
                }
                std::array<std::array<cplx, 2>, 2> k2 = thor_cubit.oblicz_lindbladian(hamiltonian_omega, rho_k2);

                std::array<std::array<cplx, 2>, 2> rho_k3;
                for (int r = 0; r < 2; ++r) {
                    for (int c = 0; c < 2; ++c) rho_k3[r][c] = thor_cubit.m_rho[r][c] + 0.5 * wygladzone_dt * k2[r][c];
                }
                std::array<std::array<cplx, 2>, 2> k3 = thor_cubit.oblicz_lindbladian(hamiltonian_omega, rho_k3);

                std::array<std::array<cplx, 2>, 2> rho_k4;
                for (int r = 0; r < 2; ++r) {
                    for (int c = 0; c < 2; ++c) rho_k4[r][c] = thor_cubit.m_rho[r][c] + wygladzone_dt * k3[r][c];
                }
                std::array<std::array<cplx, 2>, 2> k4 = thor_cubit.oblicz_lindbladian(hamiltonian_omega, rho_k4);

                for (int r = 0; r < 2; ++r) {
                    for (int c = 0; c < 2; ++c) {
                        thor_cubit.m_rho[r][c] += (wygladzone_dt / 6.0) * (k1[r][c] + 2.0 * k2[r][c] + 2.0 * k3[r][c] + k4[r][c]);
                    }
                }
                thor_cubit.wymus_hermitowskosc();

                double foton = historia_fotonow.back();
                double poprawka_stark = 0.0; double poprawka_zeeman = 0.0;

                for (int i = 0; i < LICZBA_WEZLOW; ++i) {
                    poprawka_stark += megamucha.matryca_wag[i][IDX_FOTON] * foton + megamucha.matryca_wag[i][IDX_SIN_T] * std::sin(t_th);
                    poprawka_zeeman += megamucha.matryca_wag[i][IDX_COS_T] * std::cos(t_bio);
                }

                double blad_stark = std::abs(realny_stark - std::abs(poprawka_stark));
                double blad_zeeman = std::abs(realny_zeeman - std::abs(poprawka_zeeman));

                std::vector<double> wejscie(CECHY_WEJSCIOWE);
                wejscie[IDX_FOTON] = foton; wejscie[IDX_SIN_T] = std::sin(t_th * 10); wejscie[IDX_COS_T] = std::cos(t_bio);
                wejscie[IDX_BLAD_STARK] = -blad_stark; wejscie[IDX_BLAD_ZEEMAN] = -blad_zeeman; wejscie[IDX_LAST_DD_INTERVAL] = -czas_od_ostatniego_impulsu_dd;

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
                    stdp_worker.aplikuj_plastycznosc(megamucha, last_post_spike - last_pre_spike);

                    if (blad_zeeman > 0.85 && czas_od_ostatniego_impulsu_dd > 0.10) {
                        dd_actuator.zaaplikuj_impuls_pi(thor_cubit);
                        zeeman_ou_state *= -1.0;
                        realny_zeeman = std::abs(0.2 * std::cos(t_th * 4.5) + zeeman_ou_state);
                        blad_zeeman = std::abs(realny_zeeman - std::abs(poprawka_zeeman));
                        czas_od_ostatniego_impulsu_dd = 0.0;
                        skumulowana_liczba_impulsow_dd++;
                    }
                }

                monitor_ostatni_blad_zeeman = blad_zeeman;
                monitor_koherencja_t2 = thor_cubit.oblicz_koherencje_fazowa_t2();
                megamucha.fitness += (monitor_koherencja_t2 * 5.0) - (blad_stark * 2.0) - (blad_zeeman * 2.0);

                double nowe_trafienie = dist_uniform(prng_engine);
                historia_fotonow.push_back(nowe_trafienie);
                if (historia_fotonow.size() > 100) historia_fotonow.erase(historia_fotonow.begin());
                skumulowana_energia_wh += ((30000.0 - 15.0) * (0.5 + wspolczynnik_spojnosci * 0.5) / 3600.0) * wygladzone_dt;
            }
        }
        auto it_najlepszy = std::max_element(populacja.begin(), populacja.end(), [](const AgentDNA& a, const AgentDNA& b) {
            return a.fitness < b.fitness;
            });

        AgentDNA elite_dna = *it_najlepszy;
        for (int i = 0; i < WIELKOSC_POPULACJI; ++i) {
            populacja[i].matryca_wag = elite_dna.matryca_wag;
            populacja[i].fitness = 0.0;
            if (i > 0) {
                for (int r = 0; r < LICZBA_WEZLOW; ++r) {
                    for (int c = 0; c < CECHY_WEJSCIOWE; ++c) {
                        populacja[i].matryca_wag[r][c] += dist_mutacja(prng_engine);
                    }
                }
            }
        }

        if (gen % 200 == 0 && plik_logu.is_open()) {
            plik_logu << gen << "," << najlepszy_wspolczynnik_gen << "," << skumulowana_energia_wh << "," << monitor_koherencja_t2 << "," << monitor_ostatni_blad_zeeman << "," << skumulowana_liczba_impulsow_dd << "\n";
        }

        if (gen % 10000 == 0) {
            std::cout << "[I] Lindblad Master: Gen " << gen << " / " << ostateczna_liczba_generacji << " | Lindblad T2: " << monitor_koherencja_t2 << "\n";
        }
    }

    if (plik_logu.is_open()) plik_logu.close();

    auto czas_konca_hpc = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> czas_trwania_sekundy = czas_konca_hpc - czas_startu_hpc;

    std::cout << "=====================================================================\n";
    std::cout << "ZAKONCZONO DEFINITYWNY MARATON LINDBlAD RK4 v4.7!\n";
    std::cout << "=====================================================================\n";
    std::cout << "Rzeczywisty czas przetwarzania procesora CPU: " << czas_trwania_sekundy.count() << " sekund\n";
    std::cout << "Wyewoluowano pokolen: " << ostateczna_liczba_generacji << "\n";
    std::cout << "Skumulowana czysta oszczednosc infrastruktury: " << skumulowana_energia_wh << " Wh\n";
    std::cout << "Wypracowana Koherencja Lindblada T2: " << monitor_koherencja_t2 << " (j.z.)\n";
    std::cout << "Laczna liczba wyemitowanych impulsow PI: " << skumulowana_liczba_impulsow_dd << " strzalow\n";
    std::cout << "[💾 FILE] Nieuproszczony, rygorystyczny log Lindblada w: raport_hpc.txt\n";
    std::cout << "=====================================================================\n";

    std::cout << "\nNacisnij klawisz Enter, aby zakonczyc program...";
    std::cin.get();
    return 0;
}
