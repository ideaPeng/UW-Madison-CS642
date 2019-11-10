
This homework assignment needs you to understand vulnerabilities in five target programs. The first four are required; if you do the fifth you can earn extra credit. You **may pair up for this home work** (but it's not a requirement). No extra credit (beyond Professor's commendment) for doing it alone.

## The Environment

You will test your exploit programs within a virtual machine (VM) we provide, which is configured with Debian stable ("Lenny"), with ASLR (address space layout randomization) turned off. Check this out for [VMs and setup](https://canvas.wisc.edu/courses/168496/pages/buffoverflowdemo-details?module_item_id=2178343).

## The Targets

Download [hw4.zip](https://canvas.wisc.edu/courses/168496/files/10062367/download?wrap=1) to get the files for the homework. The `targets` directory contains the source code for the targets, along with a Makefile for building them.

Your exploits should assume that the compiled target programs are installed setuid-root in `/tmp` -->  `/tmp/target1`, `/tmp/target2`, etc.

To build the targets, change to the `targets/` directory and type `make` on the command line; the Makefile will take care of building the targets.

To install the target binaries in `/tmp`, run:

```shell
make install
```

To make the target binaries setuid-root, run:

```shell
su
make setuid
exit  # to get out of the root shell
```

Once you've run `make setuid` use `exit` to return to your user shell. 

Keep in mind that it'll be easier to debug the exploits if the targets aren't setuid. (See below for more on debugging.) If an exploit succeeds in getting a user shell on a non-setuid target in `/tmp`, it should succeed in getting a root shell on that target when it is setuid. (But be sure to test that way, too, before submitting your solutions!)

## The Exploits

The `sploits/`contains skeleton source for the exploits which you are to write, along with a Makefile for building them. Also included is `shellcode.h`, which gives Aleph One's shellcode.

## The Assignment

You are to write exploits, one per target.

1.  The goal of sploit0 is different from the rest of the exploits. Take a look at target0.c, the output is "Grade = F" for any string (<20 bytes) you pass. Your goal as an attacker is to hijack the control flow of target0 to print "Grade = A". Use sploit0.c to generate and pass a "string" that is going to aid you in obtaining the desired grade, which is A obviously :P. 
2.  The rest of the exploits (sploit1 through sploit5), when run in the virtual machine with its target installed setuid-root in `/tmp`, should yield a root shell (`/bin/sh`).
3.  For full credit you **must provide** an explanation in the **sploit[0-5].txt** files.

Your task is to attack these targets. Targets 0-4 are required; target5 is extra credit.

## Grading

- Target0: 10 pts
- Target1: 10 pts
- Target2: 20 pts
- Target3; 30 pts
- Target4: 30 pts
- Target5: 30 pts (extra credit)

## Hints

- Read the Phrack articles suggested below. Read Aleph One's paper carefully, in particular.
- To understand what's going on, it is helpful to run code through gdb. See the GDB tips section below.
- Make sure that your exploits work within the provided virtual machine.
- Start early! Theoretical knowledge of exploits does not readily translate into the ability to write working exploits. Target1 is relatively simple and the other problems are quite challenging.

## GDB tips

- Notice the `disassemble` and `stepi` commands.
- You may find the `x` command useful to examine memory (and the different ways you can print the contents such as `/a` `/i` after `x`). The `info register` command is helpful in printing out the contents of registers such as `ebp` and `esp`.
- A useful way to run gdb is to use the `-e` and `-s` command line flags; for example, the command `gdb -e sploit3 -s /tmp/target3` in the VM tells gdb to execute `sploit3` and use the symbol file in `target3`. These flags let you trace the execution of the `target3` after the sploit's memory image has been replaced with the target's through the execve system call.
- When running gdb using these command line flags, you should follow the following procedure for setting breakpoints and debugging memory:
  1.  Tell gdb to notify you on exec(), by issuing the command `catch exec`
  2.  Run the program. gdb will execute the sploit until the execve syscall, then return control to you
  3.  Set any breakpoints you want in the target
  4.  Resume execution by telling gdb `continue` (or just `c`).
- If you try to set breakpoints before the exec boundary, you will get a segfault.
- If you wish, you can instrument the target code with arbitrary assembly using the `__asm__()` pseudofunction, to help with debugging. **Be sure**, however, that your final exploits work against the unmodified targets, since these we will use these in grading.

## Warnings

Aleph One gives code that calculates addresses on the target's stack based on addresses on the exploit's stack. Addresses on the exploit's stack can change based on how the exploit is executed (working directory, arguments, environment, etc.); during grading, we do not guarantee to execute your exploits exactly the same way bash does.  You must therefore **hard-code target stack locations** in your exploits. You should *not* use a function such as get_sp() in the exploits you hand in.

(In other words, during grading the exploits may be run with a different environment and different working directory than one would get by logging in as user, changing directory to `~/hw4/sploits`, and running `./sploit1`, etc.; your exploits must work even so.)

Your exploit programs should not take any command-line arguments.

## Submission:

You need to submit:

1.  The source code for your exploits (sploit0 through sploit4 or sploit5), along with any files (Makefile, shellcode.h) necessary for building them.
2.  ID: this include both you and your partner's netIDs and names.
3.  sploit0.txt, ... , splotit5.txt are six files explaining how your exploit works: what is the bug in the corresponding target, how you exploit it, and where the various constants in your exploit come from. If you **don't explain, you won't get full credits** even if your code works.

You should compress all the files into one zip file.

As a team, only one person should submit the homework.

## Suggested reading

- Ryan (Michael) Schmidt, [RMS's gdb Debugger Tutorial](http://www.unknownroad.com/rtfm/gdbtut/gdbtoc.html)
-   Aleph One, [Smashing the stack for fun and profit (Links to an external site.)](https://insecure.org/stf/smashstack.html)
-   blexim, [Basic Integer Overflows (Links to an external site.)](http://phrack.org/issues/60/10.html), Phrack 60 #0x10.
-   Gera and Riq, [Advances in Format String Exploiting (Links to an external site.)](http://phrack.org/issues/59/7.html), Phrack 59 #0x04.
-   klog, [The Frame Pointer Overwrite (Links to an external site.)](http://phrack.org/issues/55/8.html), Phrack 55 #08.
-   Anonymous, [Once Upon a free()... (Links to an external site.)](http://phrack.org/issues/57/9.html), Phrack 57 #0x09.

Advanced reading:

-   Bulba and Kil3r, [Bypassing StackGuard and StackShield (Links to an external site.)](http://phrack.org/issues/56/5.html), Phrack 56 #0x05.
-   Silvio Cesare, [Shared Library Call Redirection via ELF PLT Infection (Links to an external site.)](http://www.phrack.org/archives/issues/56/7.txt), Phrack 56 #0x07.
-   Michel Kaempf, [vudo - An Object Superstitiously Believed to Embody Magical Powers (Links to an external site.)](http://phrack.org/issues/57/8.html), Phrack 57 #0x08.

## Acknowledgements

This assignment is based in part on materials from Prof. Hovav Shacham at UC San Diego as well as Prof. Dan Boneh at Stanford. Thanks for their hard work.
