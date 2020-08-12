# dependencies

Pipenv

CMake

xtensa-esp32-elf-gcc

# build

```bash
pipenv sync
cmake -DVENDOR=espressif -DBOARD=esp32_wrover_kit -DCOMPILER=xtensa-esp32 -GNinja -S . -B build
cmake --build build
```

# Flash

`cmake --build build --target flash`

