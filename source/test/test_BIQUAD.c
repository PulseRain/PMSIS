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


q31_t __attribute__((section(".dsp_top")))      __attribute__ ((aligned (128))) biquad_sample_buffer_soft[]=  {0 , 0 , 0 , 0, 0, 0, 1, 2, -3, -4, -5, 6, -2, -4, 0, 0, 88, -99, -3, -6, -9, -12, 17, 18, -19, -20, -21, 22, 23, 24, -25, -26, -27, 28, 29, 30, 31, -32, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
q31_t __attribute__((section(".dsp_top")))      __attribute__ ((aligned (128))) biquad_sample_buffer_hard[128]=  {0};
q31_t __attribute__((section(".dsp_bottom")))   __attribute__ ((aligned (128))) biquad_coef_buffer[]  =  {1, 2, 3, 4, 5, 0, -1, -2, -3, -4, -5, 0, -3, -6, -9, -12, -15, 0, 3, 6, 9, 12, 15, 0};

extern void _init_biquad_state (q31_t *pSampBuf, uint32_t sampBufLenMinusOne, uint32_t numOfBiquad);

void test_BIQUAD()
{
    uint32_t i, before = 0, after = 0, latency_soft, latency_hard, test_len = 16, num_of_biquad = 4, shamt = 1, numOfInputSample = 3;
    bool mismatch = false;

    for (i = 0; i < test_len; ++i) {
       biquad_sample_buffer_hard[i] =  biquad_sample_buffer_soft[i];
    }
    
    _init_biquad_state(&biquad_sample_buffer_soft[num_of_biquad * 2 - 2], test_len - 1, num_of_biquad);
    _init_biquad_state(&biquad_sample_buffer_hard[num_of_biquad * 2 - 2], test_len - 1, num_of_biquad);
    
    disable_dsp_arty();
    before = rdmcycle();
    pulserain_4_biquad_q31(&biquad_sample_buffer_soft[num_of_biquad * 2 - 2], numOfInputSample, test_len, biquad_coef_buffer, shamt);
    after  = rdmcycle();
    
    latency_soft = after - before;
      
    enable_dsp_arty();
    before = rdmcycle();
    pulserain_4_biquad_q31(&biquad_sample_buffer_hard[num_of_biquad * 2 - 2], numOfInputSample, test_len, biquad_coef_buffer, shamt);
    after  = rdmcycle();
    
    latency_hard = after - before;
   
    for (i = 0; i <numOfInputSample + (num_of_biquad * 2 - 2); ++i) {
                
        if (biquad_sample_buffer_soft[i] != biquad_sample_buffer_hard[i]) {
            mismatch = true;
        }
    }
    
    test_assert (!mismatch, "BIQUAD result matches!", "BIQUAD result mismatches!");
    test_improvement (latency_soft, latency_hard); 
    
}
