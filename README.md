# ConverterDSLtoWAV

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/build-CMake-brightgreen.svg)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A lightweight music synthesizer and tracker engine written in modern C++. It parses musical composition scores defined in a custom Domain-Specific Language (DSL) and renders them into standard uncompressed 16-bit 44.1 kHz PCM WAV audio files.

---

## Features

- **Custom Music DSL**: Expressive text-based score syntax supporting BPM, multi-track patterns, note durations, and velocity.
- **Synthesizer Engines**:
  - **Oscillators**: Sine wave, Square wave, and Triangle wave synthesis.
  - **Sampler**: Audio sample playback with pitch shifting, sample looping, attack, and release shaping (ADSR).
- **DSP Audio Effects**:
  - **Gain**: Amplitude modulation and volume control.
  - **Echo**: Configurable delay and decay feedback.
  - **Tremolo**: Low-frequency amplitude oscillation.
- **Sequencer & Mixer**: Timeline pattern arrangement, polyphonic playback, and multi-track audio summing.
- **Pure C++ Audio Generation**: Directly constructs standard RIFF WAV headers and PCM audio data without external audio libraries.

---

## Audio Rendering Pipeline

```
Notes Stream ──> Instrument Generator (Oscillator / Sampler) ──> DSP Effects Chain ──> Master Mixer ──> 16-bit 44.1kHz WAV
```

---

## DSL Example

Here is a snippet showing instrument definitions, DSP effects, and pattern sequencing:

```
bpm 120

instrument lead square
    attack=0.01
    release=0.05
    effect echo delay=0.2 decay=0.3
end

instrument bass sampler
    sample=./samples/bass.wav
    root=C3
end

pattern intro resolution 8
    00 lead C4  2 70
    04 lead E4  2 70
    08 lead G4  2 70
    12 lead B4  4 80
end

track
    intro 0
end
```

Ready-to-render score examples (including a rendition of *Megalovania*) are available in the [`examples/`](examples/) directory.

---

## Build Instructions

### Prerequisites
- Modern C++ compiler with C++20 support (`g++ >= 11` or `clang++ >= 13`)
- CMake `>= 3.20`

### Building

```bash
# Clone the repository
git clone https://github.com/NikitaKkv/ConverterDSLtoWAV.git
cd ConverterDSLtoWAV

# Configure and compile
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

The resulting executable `itmoloops` will be in `build/bin/`.

---

## Usage

```bash
./build/bin/itmoloops <input_score.txt> <output_audio.wav>
```

### Example

```bash
./build/bin/itmoloops examples/megalovania.txt output.wav
```

---

## License

This project is licensed under the MIT License.
