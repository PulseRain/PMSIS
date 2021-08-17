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

#include "test.h"
#include "stdio.h"


q31_t __attribute__((section(".dsp_top")))      __attribute__ ((aligned (128))) x_buffer_soft_real[]=  {1, 2, -3, -4, -5, 0, -2, -4, -6, -8, -10, 0, -3, -6, -9, -12, 17, 18, -19, -20, -21, 22, 23, 24, -25, -26, -27, 28, 29, 30, 31, -32, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
q31_t __attribute__((section(".dsp_top")))      __attribute__ ((aligned (128))) x_buffer_hard_real[128]=  {0};
q31_t __attribute__((section(".dsp_bottom")))   __attribute__ ((aligned (128))) y_buffer[]  =  {1, 2, 3, 4, 5, 0, -1, -2, -3, -4, -5, 0, -3, -6, -9, -12, -15, 0, 3, 6, 9, 12, 15, 0, 99, 100, 111, 122, 12, 34, 22, 77};

q31_t __attribute__((section(".dsp_top")))      __attribute__ ((aligned (128))) x_buffer_soft_complex[]=  {1, 2, -3, -4, -5, 0, -2, -4, -6, -8, -10, 0, -3, -6, -9, -12, 17, 18, -19, -20, -21, 22, 23, 24, -25, -26, -27, 28, 29, 30, 31, -32, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
q31_t __attribute__((section(".dsp_top")))      __attribute__ ((aligned (128))) x_buffer_hard_complex[128]=  {0};

void test_dot_product_real()
{
    uint32_t i, before = 0, after = 0, latency_soft, latency_hard, test_len = 32, shamt = 1;
    bool mismatch = false;
    
    for (i = 0; i < test_len; ++i) {
       x_buffer_hard_real[i] =  x_buffer_soft_real[i];
    }
    
    disable_dsp_arty();
    before = rdmcycle();
    pulserain_dot_product_q31_real(x_buffer_soft_real, y_buffer, test_len, shamt);
    after  = rdmcycle();
    
    latency_soft = after - before;
       
    enable_dsp_arty();
    before = rdmcycle();
    pulserain_dot_product_q31_real(x_buffer_hard_real, y_buffer, test_len, shamt);
    after  = rdmcycle();
    
    latency_hard = after - before;
      
    for (i = 0; i < test_len; ++i) {
                
        if (x_buffer_soft_real[i] != x_buffer_hard_real[i]) {
            mismatch = true;
        }
    }
    
    test_assert (!mismatch, "Dot Product Real result matches!", "Dot Product Real result mismatches!");
    test_improvement (latency_soft, latency_hard); 
    
}

void test_dot_product_complex()
{
    uint32_t i, before = 0, after = 0, latency_soft, latency_hard, test_len = 32, shamt = 1;
    bool mismatch = false;
    
    for (i = 0; i < test_len; ++i) {
       x_buffer_hard_complex[i] =  x_buffer_soft_complex[i];
    }
    
    disable_dsp_arty();
    before = rdmcycle();
    pulserain_dot_product_q31_complex(x_buffer_soft_complex, y_buffer, test_len, shamt);
    after  = rdmcycle();
    
    latency_soft = after - before;
  
    enable_dsp_arty();
    before = rdmcycle();
    pulserain_dot_product_q31_complex(x_buffer_hard_complex, y_buffer, test_len, shamt);
    after  = rdmcycle();
    
    latency_hard = after - before;
     
    for (i = 0; i < test_len/2; ++i) {
                
        if (x_buffer_soft_complex[i*2] != x_buffer_hard_complex[i*2]) {
            printf ("mismatch %d, %d vs %d\n", i * 2, x_buffer_soft_complex[i*2], x_buffer_hard_complex[i*2]);
            mismatch = true;
        }
        
        if (x_buffer_soft_complex[i*2+1] != x_buffer_hard_complex[i*2+1]) {
            printf ("mismatch %d, %d vs %d\n", i * 2 + 1, x_buffer_soft_complex[i*2 + 1], x_buffer_hard_complex[i*2 + 1]);
            mismatch = true;
        }
    }
    
    test_assert (!mismatch, "Dot Product Complex result matches!", "Dot Product Complex result mismatches!");
    test_improvement (latency_soft, latency_hard); 
    
}
