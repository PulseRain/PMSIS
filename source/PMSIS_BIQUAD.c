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
//      BIQUAD filter
//=============================================================================

void _init_biquad_state (q31_t *pSampBuf, uint32_t sampBufLenMinusOne, uint32_t numOfBiquad)
{
    uint32_t i;
    q31_t *pState = pSampBuf;
    
    for (i = 0; i < numOfBiquad; ++i) {
        _move_pointer_back(&pState, sampBufLenMinusOne);
        *pState = 0;
        _move_pointer_back(&pState, sampBufLenMinusOne);
        *pState = 0;
    }
}


void __attribute__((optimize("O2"))) _pulserain_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLenMinusOne, q31_t *pCoef,  uint32_t shamt, uint32_t numOfBiquad)
{
    uint32_t i, j;
    int64_t accum_low = 0;
    int64_t accum_high = 0;
    
    q31_t *pState, *pSampIndex, *pCoefIndex;
    
    
    pSampIndex = pSampBuf;
    
    for (i = 0; i < numOfInputSample; ++i) {
        
        pCoefIndex = pCoef;
        pSampIndex = pSampBuf;
        pState     = pSampIndex;
        _move_pointer_back (&pState, sampBufLenMinusOne);
        _move_pointer_back (&pState, sampBufLenMinusOne);
        
        for (j = 0; j < numOfBiquad; ++j) {
            
            accum_high = 0;
            accum_low  = 0;
            
            _long_accum (0, pCoefIndex[2] * (*pSampIndex), &accum_high, &accum_low);
            _move_pointer_forward (&pSampIndex, sampBufLenMinusOne);
            
            _long_accum (0, pCoefIndex[3] * (*pSampIndex), &accum_high, &accum_low);
            _move_pointer_forward (&pSampIndex, sampBufLenMinusOne);
            
            _long_accum (0, pCoefIndex[4] * (*pSampIndex), &accum_high, &accum_low);
            
            _move_pointer_forward (&pSampIndex, sampBufLenMinusOne);
                        // coef is supposed to be zero
            _long_accum (0, pCoefIndex[5] * (*pSampIndex), &accum_high, &accum_low);
            _move_pointer_back (&pSampIndex, sampBufLenMinusOne);
            _move_pointer_back (&pSampIndex, sampBufLenMinusOne);
            _move_pointer_back (&pSampIndex, sampBufLenMinusOne);
            _move_pointer_back (&pSampIndex, sampBufLenMinusOne);
            _move_pointer_back (&pSampIndex, sampBufLenMinusOne);
                           
            _long_accum (0, pCoefIndex[0] * (*pState), &accum_high, &accum_low);
            _move_pointer_forward (&pState, sampBufLenMinusOne);
            
            _long_accum (0, pCoefIndex[1] * (*pState), &accum_high, &accum_low);
            _move_pointer_forward (&pState, sampBufLenMinusOne);
            
            *pState = _long_shift (accum_high, accum_low, shamt);
            _move_pointer_back(&pState, sampBufLenMinusOne);
             
            _move_pointer_back (&pState, sampBufLenMinusOne);
            _move_pointer_back (&pState, sampBufLenMinusOne);
            _move_pointer_back (&pState, sampBufLenMinusOne);
            
             pCoefIndex = pCoefIndex + 6;
        }
                
        _move_pointer_forward (&pSampBuf, sampBufLenMinusOne);
    }
}


void pulserain_1_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 1);DSP_HINT_BIQUAD_32BIT(1);
}


void pulserain_2_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 2);DSP_HINT_BIQUAD_32BIT(2);
}


void pulserain_3_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 3);DSP_HINT_BIQUAD_32BIT(3);
}

void pulserain_4_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(4);
}

void pulserain_5_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(5);
}

void pulserain_6_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(6);
}

void pulserain_7_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(7);
}

void pulserain_8_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(8);
}

void pulserain_9_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(9);
}

void pulserain_10_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(10);
}

void pulserain_11_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(11);
}

void pulserain_12_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(12);
}

void pulserain_13_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(13);
}

void pulserain_14_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(14);
}

void pulserain_15_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(15);
}

void pulserain_16_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt)
{
    _pulserain_biquad_q31(pSampBuf, numOfInputSample, sampBufLen - 1, pCoef,  shamt, 4);DSP_HINT_BIQUAD_32BIT(16);
}
