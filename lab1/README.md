### Install parlaylib

```
git clone https://github.com/cmuparlay/parlaylib.git
cd parlaylib
mkdir build
cd build
cmake ..
cmake --build . --target install
```

### Build and run quick sort benchmark

```
git clone https://github.com/AdvancerMan/parallel-algorithms-labs.git
cd parallel-algorithms-labs/lab1
mkdir build
cd build
cmake ..
make
PARLAY_NUM_THREADS=4 ./quick_sort_benchmark 100000000 5
```

### My results

```
Time for par is 2.5512 s
Time for seq is 8.6459 s
Sorting is correct: 1 1
Time for par is 2.6345 s
Time for seq is 9.0469 s
Sorting is correct: 1 1
Time for par is 2.6232 s
Time for seq is 9.0809 s
Sorting is correct: 1 1
Time for par is 2.5712 s
Time for seq is 9.0776 s
Sorting is correct: 1 1
Time for par is 2.5815 s
Time for seq is 9.0165 s
Sorting is correct: 1 1
Average time for seq is 8.9735 s
Average time for par is 2.5923 s
par is 3.4616 times faster than seq
O(log n) span algorithm is slow :(
Time for par is 2.6827 s
Time for par_theory_optimal is 13.1021 s
```

