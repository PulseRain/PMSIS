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

q31_t __attribute__ ((aligned (1024))) samp_sra_rnd[] = {
                3, 0, 
                65539, 9, 
                6553999, 3 ,
                -8893 ,3, 
                -99988, 3,
                23, 3, 
                -3, 3, 
                13, 3};

q31_t __attribute__ ((aligned (1024))) result_sra_rnd_soft[128] = {0};
q31_t __attribute__ ((aligned (1024))) result_sra_rnd_hard[128] = {0};

void test_sra_rnd()
{
    uint32_t i, before = 0, after = 0, latency_soft, latency_hard, test_len = 16, shamt = 3;
    bool mismatch = false;

    disable_dsp_arty();
    before = rdmcycle();
    pulserain_sra_rnd_q31(samp_sra_rnd, result_sra_rnd_soft, test_len, shamt);
    after  = rdmcycle();
    
    latency_soft = after - before;
    
    
    enable_dsp_arty();
    before = rdmcycle();
    pulserain_sra_rnd_q31(samp_sra_rnd, result_sra_rnd_hard, test_len, shamt);
    after  = rdmcycle();
    
    latency_hard = after - before;
    
    
    for (i = 0; i < test_len; ++i) {
            
        if (result_sra_rnd_soft[i] != result_sra_rnd_hard[i]) {
            mismatch = true;
        }
    }
    
    test_assert (!mismatch, "SRA_RND result matches!", "SRA_RND result mismatches!");
    test_improvement (latency_soft, latency_hard);
}
