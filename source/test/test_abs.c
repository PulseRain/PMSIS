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

q31_t __attribute__ ((aligned (1024))) samp_abs[] = {
                3, 0, 
                -65539, 9, 
                6553999, 3 ,
                3 ,3, 
                -3, 13,
                -93, 3333, 
                3, -999993, 
                -6666663, 3};

q31_t __attribute__ ((aligned (1024))) result_abs_soft[128] = {0};
q31_t __attribute__ ((aligned (1024))) result_abs_hard[128] = {0};

void test_abs()
{
    uint32_t i, before = 0, after = 0, latency_soft, latency_hard, test_len = 16;
    bool mismatch = false;
    
    disable_dsp_arty();
    before = rdmcycle();
    pulserain_abs_q31(samp_abs, result_abs_soft, test_len);
    after  = rdmcycle();
    
    latency_soft = after - before;
        
    enable_dsp_arty();
    before = rdmcycle();
    pulserain_abs_q31(samp_abs, result_abs_hard, test_len);
    after  = rdmcycle();
    
    latency_hard = after - before;
    
    
    for (i = 0; i < test_len; ++i) {
        
        if (result_abs_soft[i] != result_abs_hard[i]) {
            mismatch = true;
        }
    }
    
    test_assert (!mismatch, "Abs result matches!", "Abs result mismatches!");
    test_improvement (latency_soft, latency_hard);
}



