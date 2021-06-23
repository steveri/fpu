# FPU - 32-bit single-precision floating point unit

A simple freeware 32-bit FPU. So far it's just add, subtract, and multiply. It's not even close to being IEEE compliant, but I think it can add 2 and 2 okay. It's designed to be synthesizable but I've never done that (yet). For now it's just RTL that runs correctly on a simulator.

Files include
* addsub.v - add/sub driver
* sradd.v - core adder, needs driver to work correctly
* srsub.v - core subtractor, needs driver to work correctly
* srmul.v - multiplier, works find standalone

See test dir `test/` for examples of how to use the FPU.

For now, see `0notes-fpu.txt` for all the gory technicals.


# BACKGROUND

What got me here so far:
* needed a simple 32-bit add/sub/mul unit for my freeware FFT
* initially I used designware, but that's not free, is it
* looked for freeware. looked here, looked there. freeware sucked
* so I wrote my own. it sucks too, but it's mine haha



# TODO

See test/TODO.txt
