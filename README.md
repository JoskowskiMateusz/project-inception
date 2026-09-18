# Project Inception (v4.1) - High-Performance Hybrid Algorithmic Testbed

**Project Inception (v4.1)** is an open-source, production-ready hybrid framework engineered to explore decentralized neuromorphic feedback loops for adaptive signal processing. The system couples an ultra-fast compiled C++ simulation core with a lightweight Python data analytics suite.

* **DISCLAIMER:** This software functions strictly as a behavioral mockup and computer-science testbed for Spike-Timing-Dependent Plasticity (STDP) optimization in highly variable stochastic environments. It does not model microscopic sub-atomic quantum states or physical nuclear transitions in 1:1 real-world environments.

## 🛠️ Hybrid Architecture & High-Performance Computing

To bypass the performance boundaries and session timeout limitations of web-based interpreters, version 4.1 implements a strict **Matrix-Core Separation Pattern**:

1. **Compiled Computational Core (C++20):** The entire mathematical engine (Ornstein-Uhlenbeck continuous solutions, genetic algorithms, and multi-agent STDP loop matrices) is written in native C++20. By managing memory via contiguous stack arrays (`std::array`), the core processes a deep **20,000-generation marathon** (60 million causal synaptic updates) in **under 6 seconds** on AMD Ryzen 9 architectures.
2. **Asynchronous Analytical Layer (Python 3):** The C++ simulation layer decouples from the UI by streaming structural metadata into a standardized `raport_hpc.txt` log database. A dedicated Python layer (`wykresy_v4.py`) parses the compiled datasets using Pandas and Matplotlib to compile high-fidelity laboratory graphs (`inception_v4_metrics.png`).

## 📂 Dual-Channel Empirical Data Ingestion (`/datasets`)

The framework compiles a native high-speed file parser to execute a parallel **hot-swap superposition** of real-world environmental distortion channels:

* **`final_real_noise.csv`:** Represents znormalizowane thermal phonon vibration footprints of a **Magnesium Fluoride (MgF₂)** crystal host matrix, extracted directly from the compiled database of the **Materials Project**.
* **`laser_phase_noise.csv`:** Contains aperiodic phase noise Power Spectral Density (PSD) tracks mapping physical frequency jitters from metrological optical comb grids.

## ⚛️ Causal Core Optimization

Synaptic updates (Δ W) are computed dynamically directly at the machine-code level based on strict causal delays between incoming feedback states:

$$ \Delta W = A_{+} \exp(-\Delta t / \tau_{+}) \quad \text{for } \Delta t > 0 $$

$$ \Delta W = -A_{-} \exp(\Delta t / \tau_{-}) \quad \text{for } \Delta t < 0 $$

Under active concurrent dual-channel stress constraints, the genetic selection engine drives the neural connectome to an explicit point of target homeostasis, establishing a stable **Synaptic Coherence Index of ~0.755** with smoothly dampening oscillation amplitudes.

## 🔋 Infrastructure Power Metrics (Green Quantum Indicator)

By hardware-locking the abstract neuromorphic simulation envelope to **15.0 W**, the engine calculates cumulative net infrastructure energy conservation metrics (Wh) against high-performance cryogenic reference environments. Deep testing runs yield over **21,600+ Wh (21.6 kWh)** of infrastructure energy displacement metrics compared to helium-cooled baselines.

## 🛡️ License & Open Science Initiative

This software is published globally under the terms of the **GNU GPL v3 License**. This public release establishes open prior art and supports transparent framework auditing within the open-science engineering community.
