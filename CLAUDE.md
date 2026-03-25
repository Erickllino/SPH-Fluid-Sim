# CUDA SPH Fluid Simulation — Project Guide for Claude Code

## Project Overview

Real-time 3D fluid simulation using Smoothed Particle Hydrodynamics (SPH) solved on the GPU
with CUDA, rendered with OpenGL. Target: 100k+ particles at 60fps on an RTX 5070.

This is a portfolio project demonstrating low-level GPU programming, parallel algorithms,
and numerical simulation — built from scratch in C++/CUDA with no high-level physics
abstractions.

---

## Owner

- **Erick Vinicius Rebouças Cruz** — github.com/Erickllino
- Computer Engineering student at UFPE, Recife, Brazil
- Background: CV (YOLO/MONAI), RL (Isaac Lab / RoboCIn Sim2Real), RAG agents

---

## Tech Stack

| Layer           | Technology                                            |
| --------------- | ----------------------------------------------------- |
| Simulation      | CUDA C++ (`.cu` / `.cuh`)                             |
| Rendering       | OpenGL 4.5 + GLFW + GLAD                              |
| Math            | Custom header-only lib (`math/vec3.h`, `math/mat4.h`) |
| Build           | CMake 3.20+ with CUDA language support                |
| OS / Platform   | Ubuntu 24.04, CUDA Toolkit 12.x, RTX 5070             |
| Debug / Profile | Nsight Compute, `cuda-memcheck`, ImGui overlay        |

---

## Repository Structure

```
sph-fluid-cuda/
├── CLAUDE.md                  ← this file
├── CMakeLists.txt
├── README.md
│
├── src/
│   ├── main.cpp               ← app entry, GLFW loop
│   ├── renderer/
│   │   ├── Renderer.cpp/.h    ← OpenGL draw calls, VAO/VBO management
│   │   ├── Shader.cpp/.h      ← GLSL shader loading/compilation
│   │   └── Camera.cpp/.h      ← arcball / fly camera
│   ├── simulation/
│   │   ├── SPHSystem.cu/.cuh  ← host-side sim orchestration
│   │   ├── kernels/
│   │   │   ├── density.cu     ← density + pressure kernels
│   │   │   ├── forces.cu      ← viscosity + pressure force kernels
│   │   │   ├── integrate.cu   ← symplectic Euler integration
│   │   │   └── spatial.cu     ← uniform grid hashing, sort, neighbor search
│   │   └── Particle.cuh       ← particle struct (SoA layout)
│   └── ui/
│       └── DebugUI.cpp/.h     ← ImGui overlay (fps, particle count, params)
│
├── include/
│   └── math/
│       ├── vec3.h             ← CUDA-compatible float3 wrapper
│       ├── mat4.h             ← 4x4 matrix, projection/view helpers
│       └── quaternion.h       ← for camera rotation
│
├── shaders/
│   ├── particle.vert          ← point sprite or sphere impostor
│   ├── particle.frag          ← depth-correct sphere shading
│   └── surface.frag           ← (later) marching cubes surface
│
└── extern/
    ├── glfw/                  ← submodule
    ├── glad/                  ← generated for OpenGL 4.5 core
    └── imgui/                 ← submodule
```

---

## Physics: SPH Overview

SPH approximates fluid properties by summing contributions from neighboring particles
weighted by a smoothing kernel W(r, h):

```
A(x) = Σ_j  m_j * (A_j / ρ_j) * W(|x - x_j|, h)
```

### Simulation Steps (per frame)

1. **Spatial hashing** — assign each particle to a grid cell, sort by cell key (thrust)
2. **Density** — compute ρ_i using Poly6 kernel
3. **Pressure** — equation of state: P = k * (ρ - ρ₀)
4. **Forces** — pressure gradient (Spiky kernel) + viscosity (Laplacian kernel) + gravity
5. **Integration** — symplectic Euler (position + velocity update)
6. **Boundary** — AABB collision response

### Kernels to implement

| Kernel              | Used for           |
| ------------------- | ------------------ |
| Poly6               | Density estimation |
| Spiky gradient      | Pressure force     |
| Viscosity Laplacian | Viscosity force    |

### Key parameters (tweak via ImGui)

- `h` — smoothing radius (neighbor search radius)
- `k` — gas stiffness constant
- `ρ₀` — rest density
- `μ` — dynamic viscosity
- `dt` — timestep (start at ~0.001s)

---

## CUDA Architecture Guidelines

### Memory Layout

Use **Structure of Arrays (SoA)**, not Array of Structures (AoS):

```cpp
// GOOD — coalesced memory access
float* pos_x;  float* pos_y;  float* pos_z;
float* vel_x;  float* vel_y;  float* vel_z;
float* density;
float* pressure;

// BAD — strided access kills throughput
struct Particle { float3 pos, vel; float density, pressure; };
Particle* particles;
```

### Kernel Launch Config

```cpp
// Standard pattern for this project
constexpr int BLOCK_SIZE = 256;
int grid = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
myKernel<<<grid, BLOCK_SIZE>>>(args...);
```

### Neighbor Search

Use uniform grid with spatial hashing. Sort particles by cell index using
`thrust::sort_by_key` so neighbor lookup is a range scan — no dynamic memory
in device kernels.

### CUDA–OpenGL Interop

Register the OpenGL VBO with CUDA to avoid CPU round-trips:

```cpp
cudaGraphicsGLRegisterBuffer(&cudaVBO, glVBO, cudaGraphicsMapFlagsWriteDiscard);
// Each frame:
cudaGraphicsMapResources(1, &cudaVBO);
cudaGraphicsResourceGetMappedPointer(&devPtr, &size, cudaVBO);
// write positions directly from CUDA kernel into devPtr
cudaGraphicsUnmapResources(1, &cudaVBO);
```

---

## Build Instructions

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
./sph-fluid
```

CMake must find CUDA via `enable_language(CUDA)` and link against
`OpenGL`, `glfw`, `glad`. Set `CMAKE_CUDA_ARCHITECTURES` to `89` for RTX 5070 (Ada Lovelace).

---

## Development Milestones

### Phase 1 — Foundation
- [ ] CMake project with CUDA + OpenGL builds cleanly
- [ ] Custom `vec3` / `mat4` header-only library with `__device__` qualifiers
- [ ] GLFW window + basic OpenGL point rendering
- [ ] Particles initialized in a dam-break cube, falling under gravity (CPU first)

### Phase 2 — GPU Simulation
- [ ] Port particle integration to CUDA
- [ ] Implement uniform grid spatial hash (sort + reorder)
- [ ] Density + pressure kernels (Poly6)
- [ ] Force kernels (Spiky + Viscosity Laplacian)
- [ ] CUDA–OpenGL interop (no CPU readback)
- [ ] Stable simulation at 10k particles, 60fps

### Phase 3 — Scale & Polish
- [ ] Scale to 100k+ particles
- [ ] Sphere impostor rendering (depth-correct per-fragment normals)
- [ ] ImGui overlay with live parameter tuning
- [ ] Nsight profile — identify and fix top bottlenecks

### Phase 4 — Stretch Goals
- [ ] Surface reconstruction with Marching Cubes (GPU)
- [ ] Multiple fluid types / colors
- [ ] Interactive obstacle (click to place a sphere collider)
- [ ] Video capture for portfolio demo

---

## Code Style

- C++17, CUDA 12.x
- Headers use `#pragma once`
- Device functions always annotated `__device__ __forceinline__`
- Kernel names: `verb_noun` style (e.g. `computeDensity`, `applyForces`)
- No raw `new`/`delete` on host — use RAII wrappers or `thrust::device_vector`
- Error checking macro required on all CUDA calls:

```cpp
#define CUDA_CHECK(call) \
  do { \
    cudaError_t err = (call); \
    if (err != cudaSuccess) { \
      fprintf(stderr, "CUDA error at %s:%d — %s\n", \
              __FILE__, __LINE__, cudaGetErrorString(err)); \
      exit(EXIT_FAILURE); \
    } \
  } while(0)
```

---

## References

- Müller et al. 2003 — *Particle-Based Fluid Simulation for Interactive Applications* (original SPH paper)
- [learnopengl.com](https://learnopengl.com) — OpenGL rendering reference
- [CUDA C++ Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/)
- [Thrust library docs](https://nvidia.github.io/thrust/) — for GPU sort
- Simon Green (NVIDIA) — *Particle Simulation using CUDA* (whitepaper, great spatial hash reference)
