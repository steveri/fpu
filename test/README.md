# OVERVIEW

In the absence of better criteria, I decided that the adder is "good enough" if it gets within 1 ppm of what happens if you add two single-precision floats on the local machine. This happens most of the time. In cases where the adder fails this criteria, it mostly seems to involve *very small* numbers in the denorm / NaN range, e.g.
```
 3.301e-35 +  1.016e-38 =  3.303e-35 =?  3.302e-35 :: err= 23 ppm FAIL
  062f9041 +   006eaef0 =   062f9f2b =?   062f9e17 :: FALSE

 8.813e-39 +  1.233e-36 =  1.243e-36 =?  1.241e-36 :: err= 1184 ppm FAIL
  005ff77e +   03d1d060 =   03d3904e =?   03d3503e :: FALSE

-9.282e-39 + -2.200e-38 = -3.252e-38 =? -3.129e-38 :: err= 39506 ppm FAIL
  80651409 +   80efa56a =   813117b7 =?   812a5cba :: FALSE
```

A typical ("good enough") mid-range result looks more like this, where the numbers are exact to within six significant (decimal) digits, differing only the least-significant bit of the result, for an error of less than 0.07 parts per million:
```
 4.096661e+09 -  5.564915e+04 =  4.096605e+09 =?  4.096606e+09 :: err= 0.062491 ppm OKAY
 4f742e17 -  47596127 =  4f742d3d =?  4f742d3e :: FALSE
```

Why is there any difference at all? I assume that the local machine's FPU has internal precision greater than 32 bits, and in fact probably uses the double precision adder for the operation, rounding the final result to 32 bits at the end. Meanwhile, my adder uses internal precision matching that of the operands.

A better FPU would have parameterizable internal precision etc.


# HOW TO TEST THE ADDER
```
% make clean
% make build TEST=ADD
% make run TEST=ADD
```

where
```
# "make clean" does this:
/bin/rm -rf counter.vcd obj_dir

# "make build" does this:
verilator -Wno-fatal -Wall --cc test_ADD.v --exe test_ADD.cpp -y .. --trace
make -j -C obj_dir/ -f Vtest_ADD.mk Vtest_ADD

# "make run" does this:
obj_dir/Vtest_ADD
```

# HOW TO TEST SUBTRACTION and MULTIPLICATION

Same as above except replace `ADD` with `SUB` and `MUL` respectively.

# SAMPLE OUTPUT

The example test below ran 2000 vectors, resulting in 1797 exact matches, i.e. all 32 bits of the result were correct. 188 operations were within acceptable "good enough" error range of 1 ppm, as described above. 15 tests involved NaN or denormalized numbers, so did not count as pass or fail. Three were 0 outright failures, i.e. all tests fell within one of the preceding categories.

The test reports worst-case error found (0.2 ppm in this example) along with the vectors that produced the error.

```
ADD TEST
------------------------------------------------------------------------
 6.055668e-39 +  3.904965e-37 =  3.994018e-37 =?  3.965522e-37 :: err= 7185.915527 ppm DENORM FAIL
 0041f0bd +  0304e10c =  0307e8ce =?  0306f092 :: FALSE

Number of tests:    2000
Exact matches:      1797
Acceptable error:    188
NaN:                  14
Denorm:                1
Outright fails:        0

Max error found:  0.200323 ppm
--------------------------------------------------------------
 f418576a + e8f1154b = f418576a SHOULD BE f418576c
-4.827894e+31 + -9.107865e+24 = -4.827894e+31 =? -4.827895e+31 :: err=0.200323 ppm
--------------------------------------------------------------
```

Also see example results in `examples/` subdirectory.