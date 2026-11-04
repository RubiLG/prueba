# Prueba VST

**Prueba** is a small open-source audio effect plugin built with C++ and JUCE.

It is an initial learning project for experimenting with audio plugin development.

## Features

- Gain control (-24 dB to +12 dB)
- Bass control (-12 dB to +12 dB)
- Treble control (-12 dB to +12 dB)
- Mono and stereo processing
- Parameter state is saved by the host
- VST3 and standalone build targets
- Simple custom interface

## Build requirements

- CMake 3.22 or newer
- A C++17 compiler
- Git
- JUCE 8.0.6 (downloaded automatically by CMake)

## Build

From the project folder:

```bash
cmake -B build -S .
cmake --build build --config Release
```

The VST3 plugin is created in the build output directory under `PruebaVST_artefacts`.

## Install

Copy the generated `Prueba.vst3` bundle to your DAW's VST3 plugin folder, then rescan plugins in your DAW.

Common locations:

- Windows: `C:\\Program Files\\Common Files\\VST3`
- macOS: `/Library/Audio/Plug-Ins/VST3`
- Linux: `~/.vst3`

## Notes

This is an early prototype. The bass and treble controls use simple first-order tone shaping and are intended for experimentation, not mastering-critical use.

## License

MIT. See [LICENSE](LICENSE).
