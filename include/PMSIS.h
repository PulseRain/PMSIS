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


//=============================================================================
// PulseRain Microcontroller Software Interface Standard 
// (PMSIS for PulseRain GRV3000D)
//
// Remarks:
//      Head File for PMSIS
//=============================================================================

#ifndef PMSIS_H
#define PMSIS_H

#include "stdint.h"

//=============================================================================
// DEBUG
//=============================================================================

#ifndef C_ASSERT
#define C_ASSERT(cond) \
    extern char compile_time_assertion[(cond) ? 1 : -1]
#endif


#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field) \
        ((long)(long*)&(((type *)0)->field)) 
#endif

#ifndef TYPE_ALIGNMENT
#define TYPE_ALIGNMENT( t ) \
        ((long)(sizeof(struct { char x; t test; }) - sizeof(t)))
#endif


//=============================================================================
// Datatype
//=============================================================================


C_ASSERT(sizeof(uint8_t) == 1);
C_ASSERT(sizeof(uint16_t) == 2);
C_ASSERT(sizeof(uint32_t) == 4);
C_ASSERT(sizeof(uint64_t) == 8);

C_ASSERT(sizeof(int8_t) == 1);
C_ASSERT(sizeof(int16_t) == 2);
C_ASSERT(sizeof(int32_t) == 4);
C_ASSERT(sizeof(int64_t) == 8);

typedef int32_t q31_t;
C_ASSERT(sizeof(q31_t) == 4);

//=============================================================================
// Variable Number of Arguments
//=============================================================================

#define PP_NARG(...) \
    PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
    PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,\
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30,\
    _31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
    _41,_42,_43,_44,_45,_46,_47,_48,_49,_50,\
    _51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
    _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
    63,62,61,60,                   \
    59,58,57,56,55,54,53,52,51,50,\
    49,48,47,46,45,44,43,42,41,40,\
    39,38,37,36,35,34,33,32,31,30,\
    29,28,27,26,25,24,23,22,21,20,\
    19,18,17,16,15,14,13,12,11,10,\
    9,8,7,6,5,4,3,2,1,0


#define PP_CONCAT(a,b) PP_CONCAT_(a,b)
#define PP_CONCAT_(a,b) a ## b


//=============================================================================
// DSP HINT
//=============================================================================
#define DSP_HINT_32BIT 0
#define DSP_HINT_8BIT  1
#define DSP_HINT_16BIT 2
#define DSP_HINT_64BIT 3


#define DSP_HINT_SINGLE_SRC(...) PP_CONCAT(DSP_HINT_SINGLE_SRC_, PP_NARG(__VA_ARGS__))(__VA_ARGS__)

#define DSP_HINT_SINGLE_SRC_1(cmd) DSP_HINT_SINGLE_SRC_2(cmd, DSP_HINT_32BIT)
#define DSP_HINT_SINGLE_SRC_2(cmd, word_size) DSP_HINT_SINGLE_SRC_3(cmd, word_size, 0)
#define DSP_HINT_SINGLE_SRC_3(cmd, word_size, param1) DSP_HINT_SINGLE_SRC_4(cmd, word_size, param1, 0)
#define DSP_HINT_SINGLE_SRC_4(cmd, word_size, param1, param2) ({asm volatile ("sltiu x0, a0, %0\n\t" \
                                                                              "sltiu x0, a1, %1\n\t" \
                                                                              "sltiu x0, a4, %2" : : "n" (cmd + 1024*word_size), "n" (param1*32 + 12), "n" (param2*32 + 13) );})



#define DSP_HINT_DUAL_SRC(...) PP_CONCAT(DSP_HINT_DUAL_SRC_, PP_NARG(__VA_ARGS__))(__VA_ARGS__)

#define DSP_HINT_DUAL_SRC_1(cmd) DSP_HINT_DUAL_SRC_2(cmd, DSP_HINT_32BIT)
#define DSP_HINT_DUAL_SRC_2(cmd, word_size) DSP_HINT_DUAL_SRC_3(cmd, word_size, 0)
#define DSP_HINT_DUAL_SRC_3(cmd, word_size, param1) DSP_HINT_DUAL_SRC_4(cmd, word_size, param1, 0)
#define DSP_HINT_DUAL_SRC_4(cmd, word_size, param1, param2) ({asm volatile ("sltiu x0, a0, %0\n\t" \
                                                                            "sltiu x0, a1, %1\n\t" \
                                                                            "sltiu x0, a3, %2" : : "n" (cmd + 1024*word_size), "n" (param1*32 + 12), "n" (param2*32 + 14) );})

#define DSP_HINT_EXP_32BIT      DSP_HINT_SINGLE_SRC(0)
#define DSP_HINT_ABS_32BIT      DSP_HINT_SINGLE_SRC(1)
#define DSP_HINT_NEG_32BIT      DSP_HINT_SINGLE_SRC(2)
#define DSP_HINT_CLIP_32BIT     DSP_HINT_SINGLE_SRC(3)
#define DSP_HINT_SRA_32BIT      DSP_HINT_SINGLE_SRC(4)
#define DSP_HINT_SRL_32BIT      DSP_HINT_SINGLE_SRC(5)
#define DSP_HINT_SLL_32BIT      DSP_HINT_SINGLE_SRC(6)
#define DSP_HINT_SRA_RND_32BIT  DSP_HINT_SINGLE_SRC(7)

#define DSP_HINT_FIR_32BIT                      DSP_HINT_DUAL_SRC(16)
#define DSP_HINT_BIQUAD_32BIT(num_of_biquad)    DSP_HINT_DUAL_SRC(17, DSP_HINT_32BIT, 0, num_of_biquad)
#define DSP_HINT_FFT_32BIT                      DSP_HINT_DUAL_SRC(18)
#define DSP_HINT_IFFT_32BIT                     DSP_HINT_DUAL_SRC(19)

#define DSP_HINT_DOT_PRODUCT_REAL_32BIT         DSP_HINT_DUAL_SRC(20)
#define DSP_HINT_DOT_PRODUCT_COMPLEX_32BIT      DSP_HINT_DUAL_SRC(20, DSP_HINT_32BIT, 0, 1)

#define DSP_HINT_FFT_BF_32BIT(bf_type)          DSP_HINT_DUAL_SRC(21, DSP_HINT_32BIT, 0, bf_type)



//=============================================================================
// API prototype
//=============================================================================

extern void pulserain_exp_q31 (q31_t *pSrc, q31_t *pExp, uint32_t blockSize);
extern void pulserain_abs_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize);
extern void pulserain_negate_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize);
extern void pulserain_clip_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize, q31_t limit);
extern void pulserain_sra_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize, uint32_t shamt);
extern void pulserain_srl_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize, uint32_t shamt);
extern void pulserain_sll_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize, uint32_t shamt);
extern void pulserain_sra_rnd_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize, uint32_t shamt);
extern void pulserain_fir_q31(q31_t *pSampBuf, uint32_t sampBufLength, q31_t *pCoef, uint32_t numOfCoef, uint32_t shamt);
extern void pulserain_dot_product_q31_real(q31_t *pX, q31_t *pY, uint32_t bufLength, uint32_t shamt);
extern void pulserain_dot_product_q31_complex(q31_t *pX, q31_t *pY, uint32_t bufLength, uint32_t shamt);

extern void pulserain_1_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_2_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_3_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_4_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_5_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_6_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_7_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_8_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_9_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_10_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_11_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_12_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_13_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_14_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_15_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_16_biquad_q31(q31_t *pSampBuf, uint32_t numOfInputSample, uint32_t sampBufLen, q31_t *pCoef,  uint32_t shamt);
extern void pulserain_twiddle_gen (q31_t *pTwiddleTable, uint32_t log2N, q31_t scaleFactor, uint8_t ifft);
extern void pulserain_FFT(q31_t *pBuffer, uint32_t log2N,  q31_t *pTwiddleTable, uint32_t scaleShift, uint8_t ifft);
extern uint32_t pulserain_bit_reverse(uint32_t x, uint32_t log2N);

extern void pulserain_FFT_BF_I (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size, uint32_t ifft);
extern void pulserain_FFT_BF_II (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size);
extern void pulserain_BFII_twiddle (q31_t *pBuffer, uint32_t num_of_sections, uint32_t section_size, uint32_t scaleShift, q31_t* pTwiddleTable);

#endif


