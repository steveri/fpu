int SHOW_ALL_RESULTS = 0;
int PRINT_FINAL_SUMMARY = 1;

// #define TEST_SUB
//#define TEST_ADD
//#define TEST_MUL

#if defined(test_SUB)
  #define VTEST Vtest_SUB
  #define VTEST_H "Vtest_SUB.h"
  #define WHICH_TEST "SUB"
  #define WHICH_OP "-"


#elif defined(test_ADD)
  #define VTEST Vtest_ADD
  #define VTEST_H "Vtest_ADD.h"
  #define WHICH_TEST "ADD"
  #define WHICH_OP "+"

#endif



#include <cmath>
#include VTEST_H
#include "verilated.h"
#include "verilated_vcd_c.h"

unsigned int myrand32 () {
    unsigned int x = rand() & 0xff;
    x |= (rand() & 0xff) <<  8;
    x |= (rand() & 0xff) << 16;
    x |= (rand() & 0xff) << 24;
    return x;
}

float bits2shortreal ( unsigned int bits ) {
    float f;
    // sscanf(argv[1], "%08X", &bits);
    memcpy(&f, &bits, sizeof bits);
    return f;
    // printf("%f\n", f[0]);
}

unsigned int shortreal2bits ( float f ) {
    unsigned int data;
    //    float f = atof(argv[1]);
    memcpy(&data, &f, sizeof f);
    return data;
}

void process_args(int argc, char **argv) {
    for (int i=0; i<argc; i++) {
        // printf("Found arg %s\n", argv[i]);
        if (!strcmp(argv[i], "--help")) {
            printf("-d0 (default): no debug info, minimal output (failures only)\n");
            printf("-d1 : full output log\n");
            printf("-d9 : full output log + extensive debugging info\n");
            exit(0);
        }
    }
}

int main(int argc, char **argv, char **env) {
  int i;
  int clk;
  int ncy;
  Verilated::commandArgs(argc, argv);

  printf("\n");
  printf("%s TEST\n", WHICH_TEST);

  process_args(argc, argv);

  // init top verilog instance
  // VTEST_SUB* top = new Vtest_SUB;
  VTEST* top = new VTEST;

  // init trace dump
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  top->trace (tfp, 99);
  tfp->open ("counter.vcd");

  //Uncomment to debug "didn't converge" error
  // (see verilator manual i.e. "verilator --help")
  //Verilated::debug(1);

  top->eval(); // establish initial values?
  ncy=0;       // Timestamps for waveforms (trace).

  srand(time(0)); // Standard practice?

  // Stats
  int ntests=0;
  int n_exact=0;
  int n_okay=0;
  int n_fail=0;
  int n_nan=0;

  // Error tracking
  double error;
  double max_error_found=0.0;
  double max_error_allowed=1.0; // ppm

  float max_afloat, max_bfloat, max_zfloat, max_abfloat;
  unsigned int max_abits, max_bbits, max_zbits, max_abbits;

  printf("------------------------------------------------------------------------\n");
  int MAX_I = 2000;
  for (i=0; i<MAX_I; i++) {

      top->a = myrand32();
      top->b = myrand32();

      // Four minus two
      // top->a = 0x40000000; top->b = 0x3f800000;

      float afloat = bits2shortreal(top->a);
      float bfloat = bits2shortreal(top->b);

      // "Call" the simulator
      top->eval();

      // Process the results
      float zfloat = bits2shortreal(top->z);
      float abfloat;
      if (!strcmp(WHICH_TEST, "ADD")) {
          abfloat = (float) ( (float)afloat + (float)bfloat );
      } else if (!strcmp(WHICH_TEST, "SUB")) {
          abfloat = (float) ( (float)afloat - (float)bfloat );
      }
      unsigned int abbits = shortreal2bits(abfloat);

      // Calculate ab=solute error as parts per million (ppm)

      error = fabs(1000000*(zfloat-abfloat)/abfloat);
      /*
      error = 1000000*(zfloat-abfloat)/abfloat;

      printf("error1 = %f\n", error);
      error = (error < 0.0) ? -error : error;
      printf("error2 = %f\n", error);
      error=fabs(error);
      printf("error3 = %f\n", error);
      */

      // Keep track of max_error_found
      if (error > max_error_found) {
          max_error_found = error;

          max_afloat = afloat; max_bfloat = bfloat; max_zfloat = zfloat;
          max_abits  = top->a; max_bbits  = top->b; max_zbits  = top->z;

          max_abfloat = abfloat; max_abbits = abbits;
      }

#define EXACT (abbits == top->z)
#define OKAY  (error <= max_error_allowed)
#define FAIL  (error >  max_error_allowed)
#define MYNAN ((error != error) || (zfloat != zfloat))

      //      int DBG=0;
      //      printf("FOO %d || (%f > %f) =? %d\n", DBG, error, max_error_found, (DBG || (error > max_error)));

      if EXACT { error = 0.0; }

      const char *RESULT = EXACT ? "EXACT" : (OKAY  ? "OKAY" : "FAIL");
          
      // Don't care about nan for now; gots bigger fish to fry.
      // if (SHOW_ALL_RESULTS || FAIL || MYNAN) {
      if (SHOW_ALL_RESULTS || FAIL) {

          printf("%13.6e %s %13.6e = %13.6e =? %13.6e :: err= %lf ppm %s%s\n", 
                 afloat, WHICH_OP, bfloat, zfloat, abfloat,
                 error,
                 MYNAN ? "NAN " : "",
                 RESULT
                 );

          printf(" %08x %s  %08x =  %08x =?  %08x :: %s\n",
                 top->a, WHICH_OP, top->b, top->z, abbits,
                 (abbits == top->z) ? "TRUE" : "FALSE");
          /*
          if      EXACT { printf("result EXACT\n"); }
          else if OKAY  { printf("result OKAY\n");  }
          else          { printf("result FAIL\n");  }

          if MYNAN { printf("WHUPPS nan \n"); }
          */
          printf("\n");
      }
      ntests++;

      // Can only be one of: exact, okay, or fail
      if MYNAN     { n_nan++; }
      else if EXACT{ n_exact++; }
      else if OKAY { n_okay++; }
      else         { n_fail++; }

      // Note some nan's are okay, others are exact etc.

      tfp->dump(ncy+=5);
    }

//----------------------------------------------------------------
  if (PRINT_FINAL_SUMMARY) {
      printf("Number of tests:  %6d\n", i);
      printf("Exact matches:    %6d\n", n_exact);
      printf("Acceptable error: %6d\n", n_okay);
      printf("NaN:              %6d\n", n_nan);
      printf("Outright fails:   %6d\n", n_fail);
      printf("\n");

      printf("Max error found:  %f ppm\n", max_error_found);
      if (max_error_found > 0.0) {
          printf("--------------------------------------------------------------\n");
          printf(" %08x + %08x = %08x SHOULD BE %08x\n",
                 max_abits, max_bbits, max_zbits, max_abbits);
      
          // printf("%f + %f = %f =? %f :: %s err=%lf ppm\n", 
          printf("%13.6e + %13.6e = %13.6e =? %13.6e :: err=%lf ppm\n", 
                 max_afloat, max_bfloat, max_zfloat, max_abfloat, max_error_found);
          /*
            0.297941 + 92.117119 = 92.415054 =? 92.415062
            3e988bc2 +  42b83bf7 =  42b8d482 =?  42b8d483 :: FALSE
          */
          printf("--------------------------------------------------------------\n");
          printf("\n");
      }
  }
//----------------------------------------------------------------

  tfp->close();
  exit(0);
}

/*
Sample output:
------------------------------------------------------------------------
 3.301982e-35 +  1.016466e-38 =  3.303078e-35 =?  3.302999e-35 :: err= 23.980667 ppm FAIL
 062f9041 +  006eaef0 =  062f9f2b =?  062f9e17 :: FALSE

 8.813156e-39 +  1.233176e-36 =  1.243460e-36 =?  1.241989e-36 :: err= 1184.231934 ppm FAIL
 005ff77e +  03d1d060 =  03d3904e =?  03d3503e :: FALSE

-9.282572e-39 + -2.200802e-38 = -3.252678e-38 =? -3.129060e-38 :: err= 39506.546875 ppm FAIL
 80651409 +  80efa56a =  813117b7 =?  812a5cba :: FALSE

Number of tests:    2000
Exact matches:      1799
Acceptable error:    191
NaN:                   7
Outright fails:        3

Max error found:  39506.546875 ppm
--------------------------------------------------------------
 80651409 + 80efa56a = 813117b7 SHOULD BE 812a5cba
-9.282572e-39 + -2.200802e-38 = -3.252678e-38 =? -3.129060e-38 :: err=39506.546875 ppm
--------------------------------------------------------------
*/

/*
  JUNK
  top->a = shortreal2bits( -1.0);
  top->b = shortreal2bits(  2.0);

  top->a = shortreal2bits(  1.0);
  top->b = shortreal2bits(100.0);

  // printf("%02d: ok looks like %x + %x = %x\n", i, top->a, top->b, top->z);
*/

