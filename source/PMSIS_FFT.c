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
#include "math.h"
#include "stdio.h"
#include "utility.h"

//=============================================================================
// PulseRain Microcontroller Software Interface Standard 
//
// Remarks:
//      FFT
//=============================================================================

void _pulserain_FFT_basic_butterfly(q31_t *pBuffer, uint32_t section_size)
{
    uint32_t i, j;
    q31_t sum_real, delta_real, sum_imag, delta_imag;
    
    for (i = 0; i < section_size / 2; ++i) {
        
        sum_real    = (*pBuffer) + (*(pBuffer + section_size));
        delta_real  = (*pBuffer) - (*(pBuffer + section_size));
        
        sum_imag    = (*(pBuffer + 1)) + (*(pBuffer + section_size + 1));
        delta_imag  = (*(pBuffer + 1)) - (*(pBuffer + section_size + 1));
       
        (*pBuffer)                   = sum_real;
        (*(pBuffer + section_size))  = delta_real;
        ++pBuffer;
    
        (*pBuffer)                   = sum_imag;
        (*(pBuffer + section_size))  = delta_imag;
        ++pBuffer;
    }
}


void _pulserain_FFT_BFI_section(q31_t *pBuffer, uint32_t section_size, uint8_t ifft)
{
    uint32_t i, j;
    q31_t sum_real, delta_real, sum_imag, delta_imag;
    
    for (i = 0; i < section_size / 2; ++i) {
        sum_real    = (*pBuffer) + (*(pBuffer + section_size));
        delta_real  = (*pBuffer) - (*(pBuffer + section_size));
        
        sum_imag    = (*(pBuffer + 1)) + (*(pBuffer + section_size + 1));
        delta_imag  = (*(pBuffer + 1)) - (*(pBuffer + section_size + 1));
                   
        if (i < section_size / 4) {
           
           (*pBuffer)                   = sum_real;
           (*(pBuffer + section_size))  = delta_real;
           ++pBuffer;
        
           (*pBuffer)                   = sum_imag;
           (*(pBuffer + section_size))  = delta_imag;
           ++pBuffer;
           
        } else {
            if (ifft) {
                
                (*pBuffer)                      = sum_real;
                (*(pBuffer + section_size))     = -delta_imag;
                ++pBuffer;
        
                (*pBuffer)                      = sum_imag;
                (*(pBuffer + section_size))     = delta_real;
                ++pBuffer;
                
            } else {
       
                (*pBuffer)                      = sum_real;
                (*(pBuffer + section_size))     = delta_imag;
                ++pBuffer;
                
                (*pBuffer)                      = sum_imag;
                (*(pBuffer + section_size))     = -delta_real;
                ++pBuffer;
            }
        }            
    }
}

void _pulserain_FFT_complex_mul (q31_t x1, q31_t y1, q31_t x2, q31_t y2, q31_t *pResultX, q31_t *pResultY, uint32_t scaleShift)
{
    q31_t x, y;
    //int64_t xx, yy;
    int64_t accum_high = 0;
    int64_t accum_low = 0;
    
    _long_accum (1, 0, &accum_high, &accum_low);
    _long_accum (0, (int64_t)x1 * (int64_t)x2, &accum_high, &accum_low);
    x = _long_shift (accum_high, accum_low, scaleShift);
    
    _long_accum (1, 0, &accum_high, &accum_low);
    _long_accum (0, (int64_t)y1 * (int64_t)y2, &accum_high, &accum_low);
    x -= _long_shift (accum_high, accum_low, scaleShift);
    
    
    _long_accum (1, 0, &accum_high, &accum_low);
    _long_accum (0, (int64_t)x1 * (int64_t)y2, &accum_high, &accum_low);
    y = _long_shift (accum_high, accum_low, scaleShift);
    
    _long_accum (1, 0, &accum_high, &accum_low);
    _long_accum (0, (int64_t)x2 * (int64_t)y1, &accum_high, &accum_low);
    y += _long_shift (accum_high, accum_low, scaleShift);
    
    *pResultX = x;
    *pResultY = y;
}

uint32_t pulserain_bit_reverse(uint32_t x, uint32_t log2N)
{
    uint32_t i;
    uint32_t y = 0;

    for (i = 0; i < log2N; ++i) {
        y = y * 2 + (x & 1);
        x >>= 1;
    }
    
    return y;
}

void _pulserain_FFT_BFII_twiddle(q31_t *pBuffer, uint32_t section_size, uint32_t step, uint32_t scaleShift, q31_t *pTwiddleTable)
{
    uint32_t i;
    uint32_t a = section_size / 4;
    
    uint32_t index ;
    
    pBuffer += a * 2;
    
    index = step * 2;
    pBuffer += 2;
    for (i = 1; i < a; ++i) {
        
        _pulserain_FFT_complex_mul ((*pBuffer), *(pBuffer + 1), pTwiddleTable[index * 2], pTwiddleTable[index * 2 + 1], pBuffer, pBuffer + 1, scaleShift);
     
        pBuffer += 2;
        
        index += step * 2;
    }
    
    index = step;
    pBuffer += 2;
    for (i = 1; i < a; ++i) {
       
       _pulserain_FFT_complex_mul ((*pBuffer), *(pBuffer + 1), pTwiddleTable[index * 2], pTwiddleTable[index * 2 + 1], pBuffer, pBuffer + 1, scaleShift);
        pBuffer += 2;
        
        index += step;
    }
    
    index = step * 3;
    pBuffer += 2;
    for (i = 1; i < a; ++i) {
     
        _pulserain_FFT_complex_mul ((*pBuffer), *(pBuffer + 1), pTwiddleTable[index * 2], pTwiddleTable[index * 2 + 1], pBuffer, pBuffer + 1, scaleShift);
        pBuffer += 2;
        
        index += step * 3;
    }
    
}


void pulserain_twiddle_gen (q31_t *pTwiddleTable, uint32_t log2N, q31_t scaleFactor, uint8_t ifft)
{
    int32_t i;
    q31_t x, y;
    
    int32_t N = 1 << log2N;
        
    for (i = 0; i < 3 * (N - 3 * N / 4); ++i) {
        
        x = round(cos(2 * 3.1415926 * i / N) * scaleFactor);
        y = round(sin(2 * 3.1415926 * i / N) * scaleFactor);
        
        if (x >= scaleFactor) {
            x = scaleFactor - 1;
        } else if (x <= -scaleFactor) {
            x = -(scaleFactor - 1);
        }
        
        if (y >= scaleFactor) {
            y = scaleFactor - 1;
        } else if (y <= -scaleFactor) {
            y = -(scaleFactor - 1);
        }
                
        pTwiddleTable[i * 2] = x;
        
        if (ifft) {
            pTwiddleTable[i * 2 + 1] = y;
        } else {
            pTwiddleTable[i * 2 + 1] = -y;
        }
      
    }
}


void __attribute__((optimize("O2"))) _pulserain_FFT_BF_I (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size, uint32_t ifft)
{
    uint32_t j;
    q31_t *p;
    
    p = pBuffer;
    for (j = 0; j < num_of_sections; ++j) {
        _pulserain_FFT_BFI_section (p, section_size, ifft); 
        p += section_size * 2;
    }
}

void __attribute__((optimize("O2"))) _pulserain_FFT_BF_Basic (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size)
{
    uint32_t j;
    q31_t *p;
    
    p = pBuffer;
    for (j = 0; j < num_of_sections; ++j) {
        _pulserain_FFT_basic_butterfly (p, section_size); 
        p += section_size * 2;
    }
}

void pulserain_FFT_BF_I (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size, uint32_t ifft)
{
    _pulserain_FFT_BF_I (pBuffer, num_of_sections, section_size, ifft); DSP_HINT_FFT_BF_32BIT(1);
}

void pulserain_FFT_BF_Basic (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size)
{
    _pulserain_FFT_BF_Basic (pBuffer, num_of_sections, section_size); DSP_HINT_FFT_BF_32BIT(2);
}

void __attribute__((optimize("O2"))) _pulserain_BFII_twiddle (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size, q31_t* pTwiddleTable, uint32_t scaleShift)
{
    uint32_t j;
    q31_t *p = pBuffer;
    
    for (j = 0; j < num_of_sections; ++j) {
        _pulserain_FFT_BFII_twiddle (p, section_size, num_of_sections, scaleShift, pTwiddleTable);
        p += section_size * 2;
    }
}

void pulserain_BFII_twiddle (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size, uint32_t scaleShift, q31_t* pTwiddleTable)
{
    _pulserain_BFII_twiddle (pBuffer, num_of_sections, section_size, pTwiddleTable, scaleShift);DSP_HINT_FFT_BF_32BIT(3);
}

void __attribute__((optimize("O2"))) _pulserain_FFT(q31_t *pBuffer, uint32_t log2N,  uint32_t N, q31_t *pTwiddleTable, uint32_t scaleShift, uint8_t ifft)
{
    uint32_t numOfStages = (log2N - (log2N % 2)) / 2;
    
    uint32_t i, j;
    uint32_t section_size;
    q31_t *p;
    
    section_size = N * 2;
        
    for (i = 0; i < numOfStages; ++i) {
        
        
        section_size >>= 1;
        pulserain_FFT_BF_I (pBuffer, (uint32_t)(1 << (2 * i)), section_size, ifft);
                  
        p = pBuffer;
        section_size >>= 1;
        pulserain_FFT_BF_Basic(pBuffer, (uint32_t)(1 << (2 * i + 1)), section_size);
        
        
        pulserain_BFII_twiddle(pBuffer, (uint32_t)(1 << (2 * i)), section_size * 2, scaleShift, pTwiddleTable);
                 
    }
}

void pulserain_FFT(q31_t *pBuffer, uint32_t log2N,  q31_t *pTwiddleTable, uint32_t scaleShift, uint8_t ifft)
{
    uint32_t N = 1 << log2N;
    
    if (ifft) {
        _pulserain_FFT(pBuffer, log2N, N, pTwiddleTable, scaleShift, 1); 
    } else {
        _pulserain_FFT(pBuffer, log2N, N, pTwiddleTable, scaleShift, 0); 
    }
}
