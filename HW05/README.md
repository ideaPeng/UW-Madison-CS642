This homework assignment needs you to understand caches structures and their behavior in modern processors to create a cache side-channel attack. This is a group assignment with a max group size of 2. Remember to write down the group members' names in the ID file, and only one group member should make a submission in Canvas.

### Environment

It is highly recommended that you perform this assignment on any system with an Intel processor. You can use the CSL instructional machines to carry out this assignment.

You can use `lscpu` to figure out details about the processor and the cache. For example,

```
[chatterjee@royal-12: h/chatterjee]$ lscpu\
Architecture:        x86_64\
CPU op-mode(s):      32-bit, 64-bit\
Byte Order:          Little Endian\
. . .\
L1d cache:           32K # 64 sets, 8 ways **,** 64 byte block\
L1i cache:           32K\
L2 cache:            256K\
L3 cache:            6144K\
. . .
```

### Part A: Calibration (20 pts)

Before you begin writing a cache side-channel attack, you need to figure out some details about the caches in the system you are running the attack on. Specifically, you need to figure out the cache hit and miss times (cycles). 

1.  Use the `calibration.c` to write your code that prints out the cache hit and miss latencies. It is important that the output is in the exact format hit_time,miss_time. For example, if hit latency is 100 cycles and miss latency is 500 cycles, then the output of your program should be 100,500.
2.  To calculate the average hit latency, access a cache block so that it is brought into the cache. Now measure the time to re-access that block. Repeat this experiment for all the sets in the cache and calculate the average.
3.  To calculate the average miss latency, flush a cache block so that it is removed completely from all the caches. Measure the time to access this block. Repeat the experiment for different sets in the cache and calculate the average.

### Part B: Flush Reload attack (50 pts)

In this part of the assignment, you will write a side-channel attack code that uses the Flush+Reload technique to learn about some secrets being accessed by a toy victim process.

You will use the hit and miss latencies that were determined using the calibration program in Part A.

**Victim's Description:**

The victim program (`victim.c`) reads a `Records.csv` file containing information about some employees. The `victim.c` takes a list of indices as inputs. These indices correspond to employees who are receiving a promotion. The victim accesses rows of the `Records.csv` file corresponding to the input indices to obtain details of the employees.

**Attack Description:**

As an attacker, your goal is to use figure out the employees who are receiving a promotion. In other words, you should determine the indices the victim is trying to access. You have access to the `Records.csv` file which means that this file is shared between the victim and you. Use the Flush+Reload technique to monitor the lines accessed by the victim. You should use the hit and miss times (should be passed as a comma-separated argument to `attack.c`) calculated in part A to assist you in the attack.

Note:

1.  We recommend that you do not edit the `victim.c` because we would be testing your solutions on the unmodified version of `victim.c`. If it helps, you can edit while you are building your solution but make sure that your final solution would work on an unmodified `victim.c`
2.  Do not hard code the values of hit and miss latencies in your attack. These values can vary across machines.

**Running the attack**:

1\. Open a terminal and run the attack: ./attack $(./calibration)

Open another terminal and run the victim: ./victim 3 2 4 5  (Or something similar)

OR

2\. Run the attack and victim together:

./victim N indices... & ./attack $(./calibration)

3\. The attack needs to run forever. You could use a Keyboard Interrupt (Ctrl+C) to terminate the attack program after the victim has finished execution.

**Output Format:**

1\. The output of `calibration.c` should be as follows: hit_time,miss_time

```
Example: 80,400
```

2\. The output of the `attack.c` should be the list of indices with each index printed on a new line. Here's an example:

```
./victim 2 12 43 & ./attack $(./calibration)

Expected output:

12\
43
```

**Pseudocode:**

To help you out here's pseudocode for the attack:

```
while(1):
 For each set in cache_sets:
 Do this multiple times:
   Monitor the access time of the block
   If access time is less than cache hit threshold:
     Take note
 Determine the set with highest cache hits
 Print the index corresponding to the determined block
```

### Part C: Questions (30 pts)

**Prime+Probe:**

1.  If you were to carry out an attack similar to the one described in Part B but using Prime+Probe technique, would it be easier or harder than the Flush+Reload method? Why?
2.  How does the cache organisation (number of levels, cache size) affect Prime+Probe attack? Is that any different from Flush+Reload?

**Spectre and Meltdown:**

1.  Between Spectre and Meltdown attacks, which one is closer to be a covert channel, and why?
2.  Which of the two attacks do you think is stronger? Why?
3.  Linux released a patch with a feature called KPTI (Kernel Page Table Isolation) to defend against one of these attacks. Which one?

### Submission:

You need to submit:

1.  The source code for `calibration.c` and `attack.c`
2.  ID: this includes your partner's and your netIDs and names.
3.  solutions.txt: This file should explain the calibration procedure and the attack. Also briefly talk about your implementation of the Flush+Reload attack. If you **don't explain, you won't get full credits** even if your code works. This file should also include the answers to questions in Part C.

You should compress all the files into one zip file.  As a team, only one person should submit the homework.
