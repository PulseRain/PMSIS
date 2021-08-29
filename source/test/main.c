/*
//=============================================================================
// Copyright (c) : Pulserain Technology, LLC. 2021 
//=============================================================================
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.
*/


//=============================================================================
// PulseRain Microcontroller Software Interface Standard 
// (PMSIS for PulseRain GRV3000D)
//
// Remarks:
//      Main program for lib test
//=============================================================================

#include "PMSIS.h"
#include "stdio.h"
#include "encoding.h"
#include "utility.h"
#include "test.h"


q31_t __attribute__ ((aligned (1024))) samp[] = {3, 0, 
                65539, 9, 
                6553999, 3 ,
                3 ,3, 
                3, 3,
                3, 3, 
                3, 3, 
                3, 3};

q31_t dst;


void main()
{
    uint32_t i, before = 0, after = 0;
    
    
    delay(2000);
    printf ("==================> DSP HINT TEST\n\n");
  
    test_exp(); printf ("\n");
    test_abs(); printf ("\n");
    test_negate(); printf ("\n");
    test_clip(); printf ("\n");
    test_sra(); printf ("\n");
    test_srl(); printf ("\n");
    test_sll(); printf ("\n");
    test_sra_rnd(); printf ("\n");
    test_FIR(); printf ("\n");
    test_BIQUAD();printf ("\n");
    test_dot_product_real();printf ("\n");
    test_dot_product_complex();printf ("\n");
    
    
    test_FFT_BF_I(1,  4096, 1); printf ("\n");
    test_FFT_BF_I(4,  1024, 1); printf ("\n");
    test_FFT_BF_I(16,  256, 1); printf ("\n");
    test_FFT_BF_I(64,  64, 1);  printf ("\n");
    test_FFT_BF_I(256, 16, 1);  printf ("\n");
    test_FFT_BF_I(1024, 4, 1);  printf ("\n");
    
    test_FFT_BF_I(1,  4096, 0); printf ("\n");
    test_FFT_BF_I(4,  1024, 0); printf ("\n");
    test_FFT_BF_I(16,  256, 0); printf ("\n");
    test_FFT_BF_I(64,  64, 0);  printf ("\n");
    test_FFT_BF_I(256, 16, 0);  printf ("\n");
    test_FFT_BF_I(1024, 4, 0);  printf ("\n");
    
    
    test_FFT_BF_Basic(2,  2048);  printf ("\n");
    test_FFT_BF_Basic(8,  512);   printf ("\n");
    test_FFT_BF_Basic(32, 128);   printf ("\n");
    test_FFT_BF_Basic(128,  32);  printf ("\n");
    test_FFT_BF_Basic(512,  8);   printf ("\n");
    test_FFT_BF_Basic(2048,  2);  printf ("\n");
 
    test_FFT_TWIDDLE(1, 4096);
    test_FFT_TWIDDLE(4, 1024);
    test_FFT_TWIDDLE(16, 256);
    test_FFT_TWIDDLE(64, 64);
    test_FFT_TWIDDLE(256, 16);
    test_FFT_TWIDDLE(1024, 4);
   
   
    test_FFT(); printf ("\n");
   
    while(1) {
        delay(1000);
    }
}
