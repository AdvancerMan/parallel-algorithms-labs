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
cd parallel-algorithms-labs/lab2
mkdir build
cd build
cmake ..
make
PARLAY_NUM_THREADS=4 ./bfs_benchmark 500 5
```

### My results

```
Time for par is 37.6121 s
Time for seq is 123.2693 s
Distance is correct: 1 1
Time for par is 36.6050 s
Time for seq is 122.5627 s
Distance is correct: 1 1
Time for par is 36.6469 s
Time for seq is 126.5624 s
Distance is correct: 1 1
Time for par is 36.6113 s
Time for seq is 122.5305 s
Distance is correct: 1 1
Time for par is 36.7418 s
Time for seq is 126.8914 s
Distance is correct: 1 1
Average time for seq is 124.3633 s
Average time for par is 36.8434 s
par is 3.3755 times faster than seq
```

