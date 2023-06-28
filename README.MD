# HWT homework project. CPP-Graduate cource.

#### Building Manually

```bash
❯ rm -rf build && mkdir build
❯ cd build
❯ cmake ..
❯ cmake --build .
```

#### Running the tests

```bash
❯ ctest
```

#### Running the CLI Executable

Without arguments, it prints out its usage:

```bash
❯ ./hwt k 4 k 2 k 1 k 5 k 3 m 3 m 4 n 5 n 2 n 1
```