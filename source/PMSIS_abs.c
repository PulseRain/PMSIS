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


#include "PMSIS.h"

//=============================================================================
// PulseRain Microcontroller Software Interface Standard 
//
// Remarks:
//      Get absolute value
//=============================================================================


void __attribute__((optimize("O2"))) _pulserain_abs_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSizeMinusOne)
{
    uint32_t i;
    
    for (i = 0; i <= blockSizeMinusOne; ++i) {
        if ((*pSrc) < 0) {
            *pDst++ = -(*pSrc++); 
        } else {
            *pDst++ = *pSrc++; 
        }
    } // End of for loop
}


void pulserain_abs_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize)
{
    _pulserain_abs_q31(pSrc, pDst, blockSize - 1);DSP_HINT_ABS_32BIT;
}
