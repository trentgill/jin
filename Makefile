TARGET=main
EXECUTABLE=main.elf

CUBE=submodules/STM32_Cube_F3/Drivers
HALS=STM32F3xx_HAL_Driver/Src
#WRLIB=../../wrLib
# $(PRJ_DIR) = /calidreamin

COMPILER=arm-none-eabi
#COMPILER=~/dev/arm-gcc-8/bin/arm-none-eabi

CC=$(COMPILER)-gcc-4.9.3
#CC=$(COMPILER)-gcc
LD=$(COMPILER)-gcc-4.9.3
#LD=$(COMPILER)-gcc
AR=$(COMPILER)-ar
AS=$(COMPILER)-as
CP=$(COMPILER)-objcopy
OBJDUMP=$(COMPILER)-objdump

BIN = $(TARGET).bin

DEFS = -DSTM32F3XX -DARM_MATH_CM4 -DHSE_VALUE=8000000
STARTUP = $(CUBE)/CMSIS/Device/ST/STM32F3xx/Source/Templates/gcc/startup_stm32f301x8.s

MCFLAGS = -march=armv7e-m -mthumb

STM32_INCLUDES = \
	-I$(CUBE)/CMSIS/Device/ST/STM32F3xx/Include/ \
	-I$(CUBE)/CMSIS/Include/ \
	-I$(CUBE)/STM32F3xx_HAL_Driver/Inc/ \
	-I$(CARP_DIR)/core/ \
#    -I$(WRLIB)/

OPTIMIZE       = -Os

CFLAGS += -std=c99
CFLAGS += $(MCFLAGS)
CFLAGS += $(OPTIMIZE)
CFLAGS += $(DEFS) -I. -I./ $(STM32_INCLUDES)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
AFLAGS	= $(MCFLAGS) 

LDFLAGS = -Wl,-T,STM32F301_flash.ld
LIBS    = -lm -lc -lnosys

SRC = out/main.c \
	system_stm32f3xx.c \
	$(CUBE)/$(HALS)/stm32f3xx_hal.c \
	$(CUBE)/$(HALS)/stm32f3xx_hal_cortex.c \
	$(CUBE)/$(HALS)/stm32f3xx_hal_flash.c \
	$(CUBE)/$(HALS)/stm32f3xx_hal_flash_ex.c \
	$(CUBE)/$(HALS)/stm32f3xx_hal_gpio.c \
	$(CUBE)/$(HALS)/stm32f3xx_hal_rcc.c \
	$(CUBE)/$(HALS)/stm32f3xx_hal_rcc_ex.c \
	lib/din.c \
	lib/dout.c \
	lib/gpio.c \
	lib/stm.c \


OBJDIR = .
OBJS = $(SRC:%.c=$(OBJDIR)/%.o) 
OBJS += Startup.o


# OS dependent size printing
UNAME := $(shell uname)

GETSIZE = stat

ifeq ($(UNAME), Darwin)
	GETSIZE = stat -x
endif


all: $(TARGET).hex $(BIN)

$(TARGET).hex: $(EXECUTABLE)
	@$(CP) -O ihex $^ $@

$(EXECUTABLE): $(OBJS)
	@$(LD) -g $(MCFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	@echo "linked:       $@"
	@$(OBJDUMP) --disassemble $@ > $@.lst
	@echo "disassembly:  $@.lst"

$(BIN): $(EXECUTABLE)
	@$(CP) -O binary $< $@
	@echo "binary:       $@"
	@$(OBJDUMP) -x --syms $< > $(addsuffix .dmp, $(basename $<))
	@echo "symbol table: $@.dmp"
	@$(GETSIZE) main.bin | grep 'Size'
	@echo "        ^ must be less than 32kB (32,000)"

flash: $(BIN)
	st-flash write $(BIN) 0x08000000

debug: $(BIN)
	st-flash write $(BIN) 0x08000000
	stlink-trace -c 8

%.o: %.c
	@$(CC) -ggdb -fno-common $(CFLAGS) -c $< -o $@
	@echo $@
	@$(GETSIZE) $@ | grep 'Size'

%.s: %.c
	@$(CC) -ggdb $(CFLAGS) -S $< -o $@

out/main.c: main.carp
	carp build.carp --no-core

Startup.o: $(STARTUP)
	@$(CC) -ggdb $(CFLAGS) -c $< -o $@
	@echo $@

clean:
	@rm -f Startup.lst $(TARGET).lst $(OBJS) $(AUTOGEN) \
		$(TARGET).out  $(TARGET).hex  $(TARGET).map \
		$(TARGET).dmp  $(EXECUTABLE) out/main.c \
