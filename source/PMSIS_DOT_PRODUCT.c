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
//      Dot Product
//=============================================================================

void __attribute__((optimize("O2"))) _pulserain_dot_product_q31(q31_t *pX, uint32_t lenMinusOne, uint32_t is_complex, q31_t *pY,  uint32_t shamt)
{
    uint32_t i;
    int64_t accum_high = 0;
    int64_t accum_low = 0;
    //int64_t real_part, imag_part;
    int64_t t;
    q31_t t_real, t_imag;
    q31_t *pZ = pX;
    
    if (!is_complex) {
    
        for (i = 0; i <= lenMinusOne; ++i) {

            _long_accum (1, 0, &accum_high, &accum_low);
            
            t = pX[i] * pY[i];
            
            _long_accum (0, t, &accum_high, &accum_low);
            
            *pZ++ = _long_shift (accum_high, accum_low, shamt);
        }
    } else {
        for (i = 0; i <= (lenMinusOne + 1)/2; ++i) {
            
            _long_accum (1, 0, &accum_high, &accum_low);
            _long_accum (0, pX[i*2] * pY[i*2], &accum_high, &accum_low);
            t_real = _long_shift (accum_high, accum_low, shamt);
            
            _long_accum (1, 0, &accum_high, &accum_low);
            _long_accum (0, pX[i*2 + 1] * pY[i*2 + 1], &accum_high, &accum_low);
            t_real -= _long_shift (accum_high, accum_low, shamt);
                       
            
            _long_accum (1, 0, &accum_high, &accum_low);
            _long_accum (0, pX[i*2] * pY[i*2 + 1], &accum_high, &accum_low);
            t_imag = _long_shift (accum_high, accum_low, shamt);
            
            _long_accum (1, 0, &accum_high, &accum_low);
            _long_accum (0, pX[i*2 + 1] * pY[i*2], &accum_high, &accum_low);
            t_imag += _long_shift (accum_high, accum_low, shamt);
            
            *pZ++ = t_real;
            *pZ++ = t_imag;
        }
    }
}

void pulserain_dot_product_q31_real(q31_t *pX, q31_t *pY, uint32_t bufLength, uint32_t shamt)
{
    _pulserain_dot_product_q31 (pX, bufLength - 1, 0, pY, shamt);DSP_HINT_DOT_PRODUCT_REAL_32BIT;
}

void pulserain_dot_product_q31_complex(q31_t *pX, q31_t *pY, uint32_t bufLength, uint32_t shamt)
{
    _pulserain_dot_product_q31 (pX, bufLength - 1, 1, pY, shamt);DSP_HINT_DOT_PRODUCT_COMPLEX_32BIT;
}
