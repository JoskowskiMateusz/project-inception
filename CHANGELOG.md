# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com),
and this project adheres to Semantic Versioning.

## - 2026-09-19

### Added
- Integrated full **Lindblad Master Equation (GKSL)** inside a continuous 2x2 complex Hilbert space to model open quantum system dynamics and dissipation.
- Implemented a rigorous **4th-order Runge-Kutta (RK4)** numerical differential equation solver executed at every causal iteration step.
- Uploaded high-resolution quantum tomographic plot data (`inception_v4_7_lindblad_metrics.png`) capturing phase coherence behavior.

### Changed
- Refactored the core density matrix representation from a flattened 1D array back to a mathematically sound 2x2 matrix topology utilizing `std::complex<double>`.
- Enforced unconditional Hermitian matrix properties across all time-evolution steps to guarantee positive-definite quantum state probabilities.
- Re-routed the neural network fitness evaluation function to directly reward the maximization of the true Lindblad Phase Coherence (\(T_2\)).
- Cleaned the entire codebase of non-ASCII characters to fully resolve compiler encoding mismatches (MSVC `E0135` and `C4566`).

### Fixed
- Fixed an `AttributeError` in the Matplotlib plotting pipeline caused by improper handling of the `numpy.ndarray` axis subplots object.
- Fixed a fundamental quantum simulation invalidation where non-Hermitian state tracking generated mathematically impossible negative state probabilities.
## - 2026-09-18

### Added
- Integrated the non-linear **Debye-Waller Factor** (\(f_{DW}\)) based on the characteristic Debye temperature of the Magnesium Fluoride (\(MgF_2\)) host matrix (\(\Theta_D \approx 410\text{ K}\)).
- Modeled recoil-free emission probabilities (Mössbauer Effect) under dynamic thermal dissipation environments.
- Embedded an **Exponential Moving Average (EMA) filter** (\(\alpha = 0.04\)) at the hardware clock abstraction layer to neutralize OS timing jitter.

### Changed
- Upgraded the system architecture from a passive observer to an active quantum state manipulator by implementing **Active Dynamical Decoupling (DD)** actuator subroutines.
- Expanded the multi-channel sensory vector space to a 6D tensor layout (3x6 matrix) driving real-time \(\pi\)-pulse inversions via Pauli-X quantum gates.
- Updated telemetry logging mechanisms to output detailed operational saving trends in `raport_hpc.txt`.

## - 2026-09-18

### Added
- Replaced static artificial simulation steps with a **Nanosecond Hardware-Linked Clock** utilizing `std::chrono::high_resolution_clock`.
- Ported the computational core from interpreted Python loops to a high-performance compiled native C++20 engine (`ProjectInception_Core.cpp`).
- Added real-world field perturbation dataset pipelines (`final_real_noise.csv`) to introduce true asynchronous environmental stress testing.

### Changed
- Transitioned the framework into a native hybrid system, decoupling raw mathematical tensor generation (C++) from data visualization (Python Matplotlib).

## - 2026-09-17

### Added
- Initial public release of Project Inception.
- Established basic neuromorphic simulation scripts, genetic selection operators, and dataset ingestion pipelines using the Ornstein-Uhlenbeck (OU) formulation.
- Published open framework architecture under the **GNU GPL v3 License**.
