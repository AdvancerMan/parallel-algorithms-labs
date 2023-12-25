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

### Results after seq optimization

```
Time for par is 39.8879 s
Time for seq is 19.0402 s
Distance is correct: 1 1
Distance to {500, 500, 500} is 1500
Time for par is 39.3967 s
Time for seq is 18.7501 s
Distance is correct: 1 1
Distance to {500, 500, 500} is 1500
Time for par is 39.6999 s
Time for seq is 18.9220 s
Distance is correct: 1 1
Distance to {500, 500, 500} is 1500
Time for par is 39.6378 s
Time for seq is 18.8909 s
Distance is correct: 1 1
Distance to {500, 500, 500} is 1500
Time for par is 39.6352 s
Time for seq is 19.1541 s
Distance is correct: 1 1
Distance to {500, 500, 500} is 1500
Average time for seq is 18.9515 s
Average time for par is 39.6515 s
par is 0.4780 times faster than seq
```

