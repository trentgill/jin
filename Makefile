TARGET=main
EXECUTABLE=main.elf

CUBE=submodules/STM32_Cube_F3/Drivers
HALS=STM32F3xx_HAL_Driver/Src
#WRLIB=../../wrLib
# $(PRJ_DIR) = /calidreamin

CC=arm-none-eabi-gcc-4.9.3
LD=arm-none-eabi-gcc-4.9.3
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

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

OPTIMIZE       = -O2

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


OBJDIR = .
OBJS = $(SRC:%.c=$(OBJDIR)/%.o) 
OBJS += Startup.o

all: $(TARGET).hex

$(TARGET).hex: $(EXECUTABLE)
	$(CP) -O ihex $^ $@

$(EXECUTABLE): $(OBJS)
	$(LD) -g $(MCFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	$(OBJDUMP) --disassemble $@ > $@.lst

$(BIN): $(EXECUTABLE)
	$(CP) -O binary $< $@
	$(OBJDUMP) -x --syms $< > $(addsuffix .dmp, $(basename $<))
	ls -l $@ $<

flash: $(BIN)
	st-flash write $(BIN) 0x08000000

debug: $(BIN)
	st-flash write $(BIN) 0x08000000
	stlink-trace -c 8

%.o: %.c
	$(CC) -ggdb -fno-common $(CFLAGS) -c $< -o $@

%.s: %.c
	$(CC) -ggdb $(CFLAGS) -S $< -o $@

out/main.c: main.carp
	carp build.carp --no-core

Startup.o: $(STARTUP)
	$(CC) -ggdb $(CFLAGS) -c $< -o $@

clean:
	rm -f Startup.lst $(TARGET).lst $(OBJS) $(AUTOGEN) \
		$(TARGET).out  $(TARGET).hex  $(TARGET).map \
		$(TARGET).dmp  $(EXECUTABLE) out/main.c \
