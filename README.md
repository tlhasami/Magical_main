# MAGICAL PET KINGDOM 🐾

**A small 2D game written in C++ using SFML**

**Author:** Talha Sami

---

## Overview ✨

Magical Pet Kingdom is a simple, self-contained 2D game implemented in C++ that uses the SFML library for graphics, windowing, and audio. The game includes player data, guilds, a menu system, inventory, and other gameplay features organized across headers and asset folders.

---

## Project structure 🔧

- `B_24i3118_project/` - Main project source
  - `B_24i3118_project.cpp` - Entry point
  - `users.txt`, `guilds.txt` - Simple text DB for users and guilds
- `header/` - Game headers (e.g., `game.h`, `player.h`, `guild.h`, ...)
- `images/` - Sprite and UI images
- `sounds/` - Background music and sound effects (e.g., `bg1.ogg`)
- `fonts/` - Fonts used by the game (e.g., `font.ttf`)
- `DataBase/` - Example user files

---

## Features ✅

- Game loop with scenes (menu, gameplay, inventory, etc.)
- Music and sound effects
- Simple text-based user and guild storage
- Organized headers for modular features (battle, shop, pets, etc.)

---

## Prerequisites ⚠️

- A modern C++ compiler (g++ / clang++) with C++17 support
- SFML (Graphics, Window, System, Audio)
  - On Ubuntu: `sudo apt update && sudo apt install libsfml-dev`

---

## Build (Quick start) 🏗️

From the repository root, a minimal single-file build (works if code is contained in headers and `B_24i3118_project.cpp`) is:

```bash
g++ -std=c++17 -Iheader B_24i3118_project/B_24i3118_project.cpp -o magical \
  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

Notes:
- If the project grows, switch to a proper build system (CMake).
- Make sure you run the binary from the repository root (so `fonts/` and `sounds/` are found):

```bash
./magical
```

---

## CMake example (recommended) 🧰

Create a `CMakeLists.txt` with the following minimal content:

```cmake
cmake_minimum_required(VERSION 3.16)
project(MagicalPetKingdom)
set(CMAKE_CXX_STANDARD 17)
add_executable(magical B_24i3118_project/B_24i3118_project.cpp)
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
target_link_libraries(magical PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
include_directories(${CMAKE_SOURCE_DIR}/header)
```

Build with:

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

---

## Running & Assets 📂

- Ensure the `fonts/font.ttf` and `sounds/bg1.ogg` files exist and are accessible relative to the working directory.
- If you see errors like "Failed to load font!" or audio load errors, check file paths and working directory.

---

## Troubleshooting 💡

- Linker errors (undefined references to sf::): ensure you linked SFML libraries and installed `libsfml-dev`.
- Runtime asset errors: start the binary from the repository root so asset relative paths resolve correctly.
- If you use a package manager or non-system SFML, update `LD_LIBRARY_PATH` or use static linking accordingly.

---

## Contributing 🤝

- Bug reports and pull requests are welcome.
- For larger changes, please open an issue first to discuss the design.

---

## License 📝

This repository currently does not include a license file. Add an open-source license (e.g., MIT) if you want others to reuse your code.

---

If you'd like, I can also add a `CMakeLists.txt`, a `.gitignore`, or a short CONTRIBUTING guide. 💬