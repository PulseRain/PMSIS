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


#ifndef UTILITY_H
#define UTILITY_H

#include "stdio.h"
#include "stdbool.h"

extern volatile uint32_t* const REG_MTIME_LOW;
extern volatile uint32_t* const REG_MTIME_HIGH;
extern volatile uint32_t* const REG_UART_TX;

extern void delay (uint32_t ms);
extern uint32_t micros ();

extern void disable_dsp_arty();
extern void enable_dsp_arty();
extern void test_assert (bool x, char* msg_fail, char* msg_pass);
extern void test_improvement (uint32_t latency_soft, uint32_t latency_hard);
extern void print64(int64_t x);

extern void _long_accum (int32_t clear, int64_t value, int64_t *pAccumHigh, int64_t *pAccumLow);
extern int32_t _long_shift (int64_t accum_high, int64_t accum_low, uint32_t shamt);
extern void _move_pointer_back (q31_t **ppSampBuf, uint32_t mask);
extern void _move_pointer_forward (q31_t **ppSampBuf, uint32_t mask);

#endif
