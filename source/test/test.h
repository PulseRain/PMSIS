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

#ifndef TEST_H
#define TEST_H

#include "PMSIS.h"
#include "encoding.h"
#include "utility.h"

extern void test_exp();
extern void test_abs();
extern void test_negate();
extern void test_clip();
extern void test_sra();
extern void test_srl();
extern void test_sll();
extern void test_sra_rnd();
extern void test_FIR();
extern void test_BIQUAD();
extern void test_FFT();
extern void test_dot_product_real();
extern void test_dot_product_complex();

extern void test_FFT_BF_I(uint32_t num_of_sections, uint32_t section_size, uint32_t ifft);
extern void test_FFT_BF_II(uint32_t num_of_sections, uint32_t section_size);
extern void test_FFT_TWIDDLE(uint32_t num_of_sections, uint32_t section_size);

#endif
