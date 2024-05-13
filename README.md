Client for [ledy-micro](https://github.com/oklookat/ledy-micro).

## Build

# Requirements

- Windows 10 or above.
- C/C++ compiler.
- vcpkg.
- CMake 27 and above.

## Install

IMPORTANT: vcpkg and cmake should be installed on system drive (C:/ for example)

1. Install [cmake](https://cmake.org).
2. [Install vcpkg globally](<(https://learn.microsoft.com/en-us/vcpkg/)>).
3. Add `VCPKG_ROOT` env var pointing to vcpkg dir.
4. Install dependencies:

In project dir

```sh
vcpkg install
```
