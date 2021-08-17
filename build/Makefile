RISCV_PREFIX = riscv-none-embed-

CC  = $(RISCV_PREFIX)gcc
CXX = $(RISCV_PREFIX)g++
AS  = $(RISCV_PREFIX)as
LD  = $(RISCV_PREFIX)gcc
OBJDUMP = $(RISCV_PREFIX)objdump

CFLAGS = -I. -I../include
CFLAGS += --specs=nosys.specs -march=rv32i -mabi=ilp32 -O3  -fdata-sections -ffunction-sections -fno-exceptions -fno-unwind-tables
CFLAGS += -W -DVMAJOR=1 -DVMINOR=1 -D__GXX_EXPERIMENTAL_CXX0X__

LDFLAGS = -T ./GRV3000D.ld --specs=nosys.specs -march=rv32i -mabi=ilp32 -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -static -fno-exceptions -fno-unwind-tables -lm

#pattern rule


#== Put all the object files here
obj = ../source/test/main.o ../source/test/utility.o ../source/PMSIS_exp.o ../source/PMSIS_abs.o ../source/PMSIS_negate.o ../source/PMSIS_clip.o 
obj += ../source/PMSIS_sra.o ../source/PMSIS_srl.o ../source/PMSIS_sll.o ../source/PMSIS_sra_rnd.o ../source/PMSIS_FIR.o ../source/PMSIS_BIQUAD.o ../source/PMSIS_FFT.o 
obj += ../source/PMSIS_DOT_PRODUCT.o
      
obj += ../source/test/test_exp.o ../source/test/test_abs.o ../source/test/test_negate.o ../source/test/test_clip.o ../source/test/test_sra.o 
obj += ../source/test/test_srl.o ../source/test/test_sll.o ../source/test/test_sra_rnd.o ../source/test/test_FIR.o ../source/test/test_BIQUAD.o ../source/test/test_FFT.o
obj += ../source/test/test_dot_product.o

      
target = GRV3000D.elf
dump = $(patsubst %.elf,%.dump,$(target)) 


all: $(obj)
	@echo "====> Linking $(target)"
	@$(LD) $(obj) -o $(target) $(LDFLAGS)
	@chmod 755 $(target)
	@echo "===> Dumping sections for $@"
	@$(OBJDUMP) --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data --section=.rodata --section=.sdata --section=.sdata2 --section=.init_array --section=.fini_array --section=.dsp_top --section=.dsp_bottom $(target) > $(dump)

	
	
	
%.o : %.c 
	@echo "===> Building $@"
	@echo "============> Building Dependency"
	@$(CC) $(CFLAGS) -M $< | sed "s,$(@F)\s*:,$@ :," > $*.d
	@echo "============> Generating OBJ"
	@$(CC) $(CFLAGS) -c -o $@ $<; \
	if [ $$? -ge 1 ] ; then \
		exit 1; \
	fi
	@echo "----------------------------------------------------------------------------"

%.o : %.cpp 
	@echo "===> Building $@"
	@echo "============> Building Dependency"
	@$(CXX) $(CFLAGS) -M $< | sed "s,$(@F)\s*:,$@ :," > $*.d
	@echo "============> Generating OBJ"
	@$(CXX) $(CFLAGS) -c -o $@ $<; \
	if [ $$? -ge 1 ] ; then \
		exit 1; \
	fi
	@echo "----------------------------------------------------------------------------"

    
    
dependency = $(patsubst %.o,%.d,$(obj)) 

ifneq "$(MAKECMDGOALS)" "clean"
	-include $(dependency)
endif

	
clean :
	-@rm -vf $(target)
	-@find ../ -type f \( -name "*.riscv" -o -name "*.d" -o -name "*.o" -o -name "*.lst" -o -name "*.dump" \
	                    -o -name "*.bin" -o -name "*.out" -o -name "*.elf" \) -exec rm -vf {} \;


.PHONY: clean all
