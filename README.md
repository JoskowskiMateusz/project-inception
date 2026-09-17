# 🌌 Project Inception: Nuclear-Neuromorphic Core Simulator (v2.8)

## 📌 Overview
**Project Inception** is an open-source high-level simulation framework designed to model and evaluate decentralized neuromorphic control loops optimized for solid-state nuclear clock transitions. 

The core software emulates a 3-node Cephalopoda-inspired neural topology executing a genetic optimization algorithm to stabilize quantum systems under simulated inhomogeneous crystal-field distortions (Stark Effect) . This framework functions as a room-temperature, low-power (<15W) algorithmic alternative to cryogenic quantum error correction loops.

---

## 🔬 Core Mathematical & Algorithmic Framework

### 1. Integrated Information Metric (Operational Consciousness Φ)
The structural coherence of the multi-core matrix is derived from the Frobenius norm of the synaptic weight matrices across N autonomous processing nodes (N = 3):

$$ \Phi = \tanh\left(\frac{\Vert{}W_{\text{synaptic}}\Vert{}}{N \cdot 1.5}\right)\ $$

### 2. Perceptual Time Dilation Factor
To dynamically adjust the operational step rate of the emulated neural connectome relative to the physical CPU clock, a linear-hyperbolic time dilation multiplier is introduced:

$$ \Delta t_{\text{biological}} = \Delta t_{\text{physical}} \cdot (1.0 + 4.0\Phi)\ $$

### 3. Active Crystal Field Feedback Compensation
The framework includes a real-time 4D structural array designed to counter simulated energy level shifting (Δ E) within the solid-state host crystal lattice. The neural network continuously applies a feed-forward phase correction vector to mitigate lineshape broadening.

---

## 🧹 Memory Management & Hardware Protection (v2.8 Upregulation)
To guarantee 100% scientific integrity during long-term simulation cycles, version 2.8 isolates the core evolutionary datasets from the visual rendering subsystem, introducing rigid hardware protection layers:

- **Rolling Buffer Window (FIFO):** The chaotic 3D coordinate trajectories plotted via Plotly are restricted to a strict limit of 30 historical coordinates. Old visual nodes are automatically popped out (`First-In, First-Out`), preventing session memory leaks in the browser web view.
- **Generational Garbage Collection:** At every evolutionary boundary (Gen increment), the system triggers an explicit `gc.collect()` sweep. This purges dead references of mutated individuals from the physical CPU RAM array (optimized for high-threaded platforms like AMD Ryzen 9).
- **Core Integrity:** Synaptic weight histories (`populacja_dna_rozproszona`) and adaptation maps (`populacja_fitness`) remain entirely uncompromised by the memory clearing routines.

---

## 🧱 Hardware Architecture: VUV Frequency Comb Stabilization Core

To transition the software control loops into a physical system, the framework interfaces with a dual-stage optoelectronic hardware stabilization layer to solve laser phase noise limits [arxiv:2507.01180v1].

### 1. High-Harmonic Generation (HHG) Source
- **Pump Laser:** Ytterbium-doped (Yb) fiber laser emitting ultrashort infrared pulses (≈ 1040 nm).
- **Frequency Multiplication:** Pulses are compressed and focused into a high-finesse Femtosecond Enhancement Cavity filled with an inert gas jet (Argon/Xenon) to drive High-Harmonic Generation. 
- **Output:** The process isolates the 7th harmonic, producing coherent Vacuum Ultraviolet (**VUV, λ ≈ 148.6 nm**) light at the targeted ≈ 8.35 eV nuclear isomer transition energy [arxiv:2507.01180v1].

### 2. Phase-Locking Mechanism
- **Cavity Design:** Multi-layer fluoride-coated dielectric mirrors inside a vacuum chamber to eliminate oxygen absorption.
- **Reference Lock:** The VUV comb lines are actively locked via the Pound-Drever-Hall (PDH) technique to an ultra-stable sub-femtosecond optical reference clock, freezing the phase profile before crystal interaction.

### 3. AI-Driven Feed-Forward Compensation
Instead of direct modulation of the high-energy laser source, the 4D neuromorphic core outputs real-time phase-correction vectors to a fast **Beta-Barium Borate (β-BBO)** Electro-Optic Modulator (EOM). The system shifts the refractive index in picoseconds, dynamically counteracting local phase drift before the photons hit the ²²⁹Th crystal matrix [arxiv:2507.01180v1].

---

## 🛠️ Features
- **Dynamic 3D Trajectory Rendering:** Real-time visualization of the nuclear state vector path utilizing asynchronous Plotly processing.
- **Genetic Optimization Engine:** Persistent population tracking (200 individuals per generation) with elite-parent cloning and uniform exploratory mutation operators.
- **Stark Mitigation Dashboard:** Live telemetry monitoring local noise, mitigation accuracy percentage, and cumulative simulated quantum efficiency.

---

## 💻 Installation & Setup

### Prerequisites
Ensure you have **Python 3.8+** installed along with `pip`. This simulator is highly optimized for multi-threaded modern CPU architectures with high-bandwidth memory arrays.

### 1. Clone the Repository
```bash
git clone https://github.com
cd project-inception
```

### 2. Install Required Dependencies
```bash
pip install streamlit numpy plotly
```

### 3. Run the Application
```bash
streamlit run app.py
```

---

## 📄 License
This project is licensed under the **GNU GPL v3 (General Public License)** - see the `LICENSE` file for details. 

*Commercial Note:* Under the GPL v3 terms, any commercial implementation or closed-source integration of this algorithmic framework requires a separate proprietary commercial license from the authors.

---

## 👥 Authors & Contact
- **Lead Architect:** [Mateusz Joskowski]
- email: mateusz.joskowski@protonmail.com
