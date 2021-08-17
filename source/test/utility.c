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
//      Utility functions to aid lib building
//=============================================================================

#include "PMSIS.h"
#include "stdio.h"
#include "encoding.h"
#include "utility.h"


volatile uint32_t* const REG_MTIME_LOW  = (uint32_t*) 0x20000000;
volatile uint32_t* const REG_MTIME_HIGH = (uint32_t*) 0x20000004;

volatile uint32_t* const REG_UART_TX    = (uint32_t*) 0x20000010;

volatile uint32_t* const REG_GPIO       = (uint32_t*) 0x20000018;


//=============================================================================
// Override _write for printf
//=============================================================================
int _write(int file, char *data, int len)
{
  
   int i;
   
    for (i = 0; i < len; ++i) {
        while ((*REG_UART_TX) & 0x80000000);
            (*REG_UART_TX) = data[i];
        while ((*REG_UART_TX) & 0x80000000);
    }

   return len; 
}


//=============================================================================
// Delay in millisecond
//=============================================================================

void delay (uint32_t ms)
{
    uint32_t low, high;
    uint64_t finish_time, current_time;
    
    uint32_t delay_interrupt_saved_mstatus    = read_csr (mstatus);
    
    write_csr (mstatus, 0);
        low  = (*REG_MTIME_LOW);
        high = (*REG_MTIME_HIGH);
    write_csr (mstatus, delay_interrupt_saved_mstatus);
    
    finish_time = ((uint64_t)high << 32) + (uint64_t)(low) + (uint64_t)ms*1000;
    
    do {
        
        write_csr (mstatus, 0);
            low  = (*REG_MTIME_LOW);
            high = (*REG_MTIME_HIGH);
        write_csr (mstatus, delay_interrupt_saved_mstatus);
        
        current_time = ((uint64_t)high << 32) + (uint64_t)(low);
    } while (finish_time > current_time);

} // End of delay()

//=============================================================================
// The number of microseconds passed
//=============================================================================

uint32_t micros ()
{
    return (*REG_MTIME_LOW);
} // End of micros()


//=============================================================================
// set GPIO[15] to high, which will disable DSP coprocessor on Arty7 Platform
//=============================================================================
void __attribute__((optimize("O0"))) disable_dsp_arty()
{
    int i;
    
    *REG_GPIO |= 1 << 15;
    
    for (i = 0; i < 2; ++i) {
        asm volatile ("nop");
    }    
}

//=============================================================================
// set GPIO[15] to low, which will enable DSP coprocessor on Arty7 Platform
//=============================================================================
void __attribute__((optimize("O0"))) enable_dsp_arty()
{
    int i;
    
    *REG_GPIO &= 0xFFFF7FFF;
    
    for (i = 0; i < 2; ++i) {
        asm volatile ("nop");
    }
}

void print64(int64_t x)
{
    int32_t low  = x & 0xFFFFFFFF;
    int32_t high = (x >> 32) & 0xFFFFFFFF;
    
    printf ("0x%08x%08x", high, low);
}


void test_assert (bool x, char* msg_pass, char* msg_fail)
{
    if (!x) {
        printf ("!!!!!!!!!!! Assertion Fail, %s\n", msg_fail);
        while(1);
    }
    
    printf ("=============> Test Pass: %s\n", msg_pass);
}

void test_improvement (uint32_t latency_soft, uint32_t latency_hard)
{
    if (latency_hard) {
        printf ("=============> Cycles of soft DSP = %d, Cycles of hard DSP = %d, Improvement %d%% \n", latency_soft, latency_hard, (latency_soft - latency_hard) * 100 / latency_hard);
    } else {
        printf ("impossible zero latency !!!!!!!!!!! \n");
        while(1);
    }
}


//=============================================================================
// Internal Service Functions
//=============================================================================

void _long_accum (int32_t clear, int64_t value, int64_t *pAccumHigh, int64_t *pAccumLow) 
{
    int64_t accum_high = *pAccumHigh;
    int64_t accum_low  = *pAccumLow;
    uint64_t       t; 
  
    if (clear) {
        accum_high = 0;
        accum_low  = 0;
    } else {

        t = (uint64_t)accum_low + (uint64_t)value;
        
        if ((t < (uint64_t)accum_low) || (t < (uint64_t)value)) {
            if (value >= 0) {
                ++accum_high;
            }
        } else if (value < 0) {
            --accum_high;
        }
        
        accum_low = accum_low + value;
    }
    
     
    *pAccumLow  = accum_low;
    *pAccumHigh = accum_high;
}


int32_t _long_shift (int64_t accum_high, int64_t accum_low, uint32_t shamt)
{
    uint64_t mask_low;
    uint64_t mask_high;
    
    
    
    if (shamt == 0) {
        
    } else {
           
        mask_low  = ((uint64_t)1 << (64 - shamt)) - 1;
        mask_high = ((uint64_t)1 << (shamt)) - 1;
        accum_low = ((accum_low >> (shamt - 1)) & mask_low) | (accum_high & mask_high) << (64 - shamt);
        ++accum_low;
        accum_low >>= 1;
              
    }
    
    return (int32_t)(accum_low & 0xFFFFFFFF);
}



void _move_pointer_back (q31_t **ppSampBuf, uint32_t mask)
{
    mask = mask * 4 + 3; 
    uint32_t t = (uint32_t) *ppSampBuf;
    uint32_t high_mask = 0xFFFFFFFF ^ mask;
    uint32_t high_addr = t & high_mask;
    uint32_t low_addr  = (t - 4) & mask;
     
    *ppSampBuf = (q31_t*)(high_addr | low_addr);
    
}

void _move_pointer_forward (q31_t **ppSampBuf, uint32_t mask)
{
    mask = mask * 4 + 3;
    uint32_t t = (uint32_t) *ppSampBuf;
    uint32_t high_mask = 0xFFFFFFFF ^ mask;
    uint32_t high_addr = t & high_mask;
    uint32_t low_addr  = (t + 4) & mask;
    
    *ppSampBuf = (q31_t*)(high_addr | low_addr);
    
}
