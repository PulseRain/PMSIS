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

q31_t __attribute__ ((aligned (1024))) samp_exp[] = {
                3, 0, 
                65539, 9, 
                6553999, 3 ,
                -8893 ,3, 
                88, 3,
                23, 3, 
                -3, 3, 
                13, 3};


void test_exp()
{
    uint32_t i, before = 0, after = 0, latency_soft, latency_hard;
    
    q31_t exp_dst_soft = 0, exp_dst_hard = 0;
    
    disable_dsp_arty();
    before = rdmcycle();
    pulserain_exp_q31(samp_exp, &exp_dst_soft, 16);
    after  = rdmcycle();
    
    latency_soft = after - before;
        
    enable_dsp_arty();
    before = rdmcycle();
    pulserain_exp_q31(samp_exp, &exp_dst_hard, 16);
    after  = rdmcycle();
    
    latency_hard = after - before;
        
    test_assert (exp_dst_soft == exp_dst_hard, "Exponent matches!", "Exponent mismatches!");
    test_improvement (latency_soft, latency_hard);
}



