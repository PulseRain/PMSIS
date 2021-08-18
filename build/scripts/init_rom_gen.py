#! python3
###############################################################################
# Copyright (c) 2021, PulseRain Technology LLC 
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License (LGPL) as 
# published by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
# or FITNESS FOR A PARTICULAR PURPOSE.  
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################


import os, sys, getopt
import math, time

from time import sleep
from CRC16_CCITT import CRC16_CCITT
from PRBS import PRBS

from ROM_Hex_Format import *

import serial

from pathlib import Path

import ctypes

import subprocess
import re

            
    
#==============================================================================
# main            
#==============================================================================

if __name__ == "__main__":

    hex_file = sys.argv[1]
    
    intel_hex_file =  Intel_Hex(hex_file)
        
    data_list_to_write = []
    addr = 0
        
    count = 0
    for record in intel_hex_file.data_record_list:
        #print ("=================================== ", [hex(k) for k in  record.data_list])
        
        if (len(data_list_to_write) == 0):
            data_list_to_write = record.data_list
            addr = record.address
        elif (((addr + len(data_list_to_write)) == record.address) and (len(data_list_to_write) < 8192)):
            data_list_to_write = data_list_to_write + record.data_list
            
            count = count + 1
            
            
        else:
            if (len(data_list_to_write) % 4):
                data_list_to_write = data_list_to_write + [0] * (len(data_list_to_write) % 4)
            
            #print ("+++++++++++++++++++ ", [hex(k) for k in  data_list_to_write])
            data_list_to_write_reorder = [0] * len(data_list_to_write)
            for i in range (len(data_list_to_write) // 4):
                value = data_list_to_write [i * 4 + 0] + (data_list_to_write [i * 4 + 1] << 8) + (data_list_to_write [i * 4 + 2] << 16) + (data_list_to_write [i * 4 + 3] << 24)
                
                print ("rom_mem[%d] <= 32\'h%08x;" % ((addr + (i * 4) - 0x80000000)/4, value))
                                
            data_list_to_write = record.data_list
            addr = record.address
    