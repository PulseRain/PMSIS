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
#include "stdio.h"
#include "utility.h"

//=============================================================================
// PulseRain Microcontroller Software Interface Standard 
//
// Remarks:
//      FIR filter
//=============================================================================

void __attribute__((optimize("O2"))) _pulserain_fir_q31(q31_t *pX, uint32_t yLenMinusOne, uint32_t xLenMinusOne, q31_t *pY,  uint32_t shamt)
{
    uint32_t i, j;
    int64_t accum_high = 0;
    int64_t accum_low = 0;
    int64_t t;
    q31_t *pZ = pX;
        
    for (i = 0; i <= xLenMinusOne; ++i) {
        
        _long_accum (1, 0, &accum_high, &accum_low);
        
        for (j = 0; j <= yLenMinusOne; ++j) {
            
            
            t = pX[(i + j)] * pY[j];
            
            _long_accum (0, t, &accum_high, &accum_low);
        }
        
        *pZ++ = _long_shift (accum_high, accum_low, shamt);
    }
    
}

void pulserain_fir_q31(q31_t *pSampBuf, uint32_t sampBufLength, q31_t *pCoef, uint32_t numOfCoef, uint32_t shamt)
{
    _pulserain_fir_q31 (pSampBuf, numOfCoef - 1, sampBufLength - 1, pCoef, shamt);DSP_HINT_FIR_32BIT;
}
