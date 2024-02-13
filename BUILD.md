# Requirements

- C/C++ compiler.
- vcpkg.
- CMake 27 and above.

## Install

IMPORTANT: vcpkg and cmake should be installed on system drive (C:/ for example)

1. Install [cmake](https://cmake.org).
2. [Install vcpkg globally](<(https://learn.microsoft.com/en-us/vcpkg/)>).
3. Add `VCPKG_ROOT` env var pointing to vcpkg dir.
4. Add vcpkg dir to `PATH`.
5. Install dependencies:

In project dir

```sh
vcpkg install
git submodule update --init --recursive
```
