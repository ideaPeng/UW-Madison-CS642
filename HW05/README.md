# Flush and Reload Attack on Shared File Memory Access

## How it Work

In calibration program, we want to know the average of hit and miss cycles for the
current CPU so that we can decide a proper threshold to distingish between hits
and misses during attack. For carlibration, we just use a 4KB array and access it
with and without cache flushing before fetching to get the average of CPU cycles
of cache hits and cache misses. For the attack, we randomly generate a raw index
for the CSV file and for each iteration, we measure the memory access time for 
that row `TEST_TIME` times. We also set the CPU cycle threshold of hit and miss 
to a number `hit_cycle_threshold`. For each test, if the access time is smaller
than `hit_cycle_threshold`, we increase the `hit_count`. If the `hit_count`
exceeds the `HIT_THRESHOLD`, we print the number of that row.

After some experiemtn, we set the above parameters as the following numbers:
```
TEST_TIME = 15
hit_cycle_threshold = hit_time + (miss_time - hit_time) / 3
HIT_THRESHOLD = 30
```

## How to Run/Test
```shell
make
./attack $(./carlibration)
./victim <number-of-rows-to-access> [...a lit of row number [1 to 84]]
```
Use this file to explain how the calibration and the attack works.

## Part C Questions

### Prime+Probe

1. If you were to carry out an attack similar to the one described in Part B but using Prime+Probe technique, would it be easier or harder than the Flush+Reload method? Why?
   If would be harder. Because 1) We need to fill all the caches that in the CPU during Prime step. 2) It is hard to know which cache line corresponds to which row for the CSV files. 3) Some other program may also displace the cache, introducing more noise.

2. How does the cache organisation (number of levels, cache size) affect Prime+Probe attack? Is that any different from Flush+Reload?
   If there are multiple cache levels and the victim and the attacker's programs are running on the different cores, the victim program may only displace the cache at the shared level, which make the Prime+Probe difficult. Also, if the cache size is large, P&P method will requires to fill more cache and monitor those cache more frequently to reduce the noise. However, Flush+Reload will not affect by those kind of problems and it will work as long as the attacker program and the victim program shared some level of cache.

### Spectre and Meltdown

1. Between Spectre and Meltdown attacks, which one is closer to be a covert channel, and why?
   
2. Which of the two attacks do you think is stronger? Why?
3. Linux released a patch with a feature called KPTI (Kernel Page Table Isolation) to defend against one of these attacks. Which one?


## Reference Paper

Yarom, Yuval, and Katrina Falkner. ["FLUSH+ RELOAD: a high resolution, low noise, L3 cache side-channel attack."](https://www.usenix.org/conference/usenixsecurity14/technical-sessions/presentation/yarom) 23rd {USENIX} Security Symposium ({USENIX} Security 14). 2014.