# 🌌 Project Inception: Nuclear-Neuromorphic Core Simulator (v2.9.1-OU)

## 📌 Overview
**Project Inception** is an open-source, high-level simulation framework designed to model and evaluate decentralized neuromorphic control loops optimized for solid-state nuclear clock transitions. 

The core software emulates a 3-node Cephalopoda-inspired Spiking Neural Network (SNN) executing a hybrid genetic optimization engine coupled with real-time **Spike-Timing-Dependent Plasticity (STDP)** to dynamically mitigate room-temperature thermal phonon scattering and inhomogeneous crystal-field splitting (Stark Effect) within a solid-state $^{229}\text{Th:CaF}_2$ host crystal matrix.

---

## 🔬 Core Mathematical & Algorithmic Framework

### 1. Integrated Information Metric (Operational Consciousness Φ)
The structural coherence of the multi-core matrix is derived from the Frobenius norm of the 4D synaptic weight matrices across $N$ autonomous processing nodes ($N = 3$):

$$ \Phi = \tanh\left(\frac{\Vert{}W_{\text{synaptic}}\Vert{}}{N \cdot 1.5}\right) $$

### 2. Perceptual Time Dilation Factor
To dynamically adjust the operational step rate of the emulated spiking connectome relative to the physical CPU clock, a linear-hyperbolic time dilation multiplier regulates the biological timeline ($\Delta t_{\text{bio}}$):

$$ \Delta t_{\text{biological}} = \Delta t_{\text{physical}} \cdot (1.0 + 4.0\Phi) $$

### 3. Crystalline Thermal Noise Emulation via Ornstein-Uhlenbeck SDE
Rather than using basic Gaussian models, version 2.9.1 introduces a continuous stochastic differential equation (SDE) solved via the Euler-Maruyama method to realistically emulate thermal phonon interactions inside the crystal lattice:

$$ dx_t = -\theta x_t dt + \sigma dW_t $$

Where:
- $\theta = 0.40$ represents the mean-reversion stiffness (crystal potential bounding).
- $\sigma = 0.25$ represents the volatility coefficient (thermal phonon energy intensity).
- $dW_t$ represents the standard Brownian motion increment.

This stochastic framework ensures time-correlated, continuous noise trajectories, creating a rigorous testing ground for phase-matching validation.
### 4. Spike-Timing-Dependent Plasticity (STDP) Engine
To achieve sub-femtosecond phase synchronization, the network adjusts its synaptic weights in real-time based on the strict causal delay based on the strict causal delay $\Delta t = t_{\text{post}} - t_{\text{pre}}$ between incoming quantum feedback between incoming quantum feedback and node activation events:

$$ \Delta W(\Delta t) = \begin{cases} 
A_+ \cdot e^{-\frac{\Delta t}{\tau_+}} & \text{for } \Delta t > 0 \quad \text{ (LTP - Potentiation)} \\ 
-A_- \cdot e^{\frac{\Delta t}{\tau_-}} & \text{for } \Delta t \le 0 \quad \text{ (LTD - Depression)} 
\end{cases} $$

This continuous causal adaptation allows the 4D synaptic arrays to accurately predict and counter the crystal-field drift before the laser pulses hit the host matrix.

---

## 🧹 Memory Management & Hardware Protection
To prevent browser session instability during massive multi-generational ewolucyjne loops, the simulator isolates core datasets from the rendering graphics engine:
- **Rolling Visual Buffer Window (FIFO):** The chaotic 3D Plotly trajectory rendering is tightly bound to a maximum limit of 30 historical coordinates, preventing web-view memory leaks.
- **Generational Garbage Collection:** At every generational boundary, an explicit `gc.collect()` sweep is executed to purge dead memory references of mutated configurations from the CPU RAM cache (highly optimized for multi-threaded architectures like AMD Ryzen 9).

---

## 🧱 Hardware Architecture: VUV Frequency Comb Stabilization Core
To ground the software control loops in a physical framework, the system interfaces with a dual-stage optoelectronic layer:
1. **High-Harmonic Generation (HHG) Source:** An Ytterbium-doped fiber laser (≈ 1040 nm) driven into a high-finesse enhancement cavity to isolate the 7th harmonic, producing coherent Vacuum Ultraviolet (**VUV, λ ≈ 148.6 nm**) light at ≈ 8.35 eV.
2. **Phase-Locking:** Actively stabilized via the Pound-Drever-Hall (PDH) technique locked to a sub-femtosecond optical reference clock.
3. **Feed-Forward EOM Control:** The neuromorphic core outputs correction vectors directly to a fast Beta-Barium Borate (β-BBO) Electro-Optic Modulator (EOM) to correct phase shifts in flight.

---

## 💻 Installation & Setup

### Prerequisites
- Python 3.8+
- Active internet browser (Edge/Chrome/Firefox)

### 1. Clone the Repository
```bash
git clone https://github.com
cd project-inception
```

### 2. Install Dependencies
```bash
pip install streamlit numpy plotly
```

### 3. Run the Simulator
```bash
streamlit run app.py
```

---

## 📄 License
This project is licensed under the **GNU GPL v3 (General Public License)** - see the `LICENSE` file for details. 

*Commercial Note:* Under the GPL v3 copyleft provisions, any closed-source proprietary distribution or industrial integration of this framework requires a separate commercial license from the authors.

---

## 👥 Authors
- **Lead Architect:** [Mateusz Joskowski]
- email: mateusz.joskowski@protonmail.com
