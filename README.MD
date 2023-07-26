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

```bash
❯ ./order_statistic
❯ k 4 k 2 k 1 k 5 k 3 m 3 m 4 n 5 n 2 n 1
```

```bash
❯ ./range_find
❯ 5 10 20 30 40 50 3 29 35 34 37 29 41
```


#### Running benchmark

```bash
❯ ./benchmarks/avl_vs_splay {N}
```