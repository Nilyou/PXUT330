
# Optimization level
OPTIMIZATION = -O2

#-------------------------------------------------------------------------------
#		Tools
#-------------------------------------------------------------------------------

# Tool suffix when cross-compiling
CROSS_COMPILE = arm-none-eabi-

# Compilation tools
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
SIZE = $(CROSS_COMPILE)size
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
NM = $(CROSS_COMPILE)nm

# Flags
CFLAGS += $(TARGET_OPTS)
CFLAGS += -Wall -mlong-calls -ffunction-sections
CFLAGS += -g $(OPTIMIZATION)
ASFLAGS = $(TARGET_OPTS) -Wall -g $(OPTIMIZATION)
LDFLAGS = -g $(OPTIMIZATION) -nostartfiles $(TARGET_OPTS)

#-------------------------------------------------------------------------------
#		Files
#-------------------------------------------------------------------------------

# Objects built from C source files
C_OBJECTS += main.o tools.o common.o display.o sysdef.o gain.o sdcard.o armtest.o ini.o

# Objects built from Assembly source files
ASM_OBJECTS += startup.o

# All c source files
C_FILES = main.c tools.c common.c display.c sysdef.c gain.c sdcard.c armtest.c ini.c

# All header files
H_FILES = M10Era.h tools.h char.h globe.h common.h display.h sysdef.h Media.h ff.h


#Library Path
LIB_PATH_1 = "C:\Program Files\CodeSourcery\Sourcery G++ Lite\arm-none-eabi\lib"
LIB_PATH_2 = "C:\Program Files\CodeSourcery\Sourcery G++ Lite\lib\gcc\arm-none-eabi\4.4.1"

#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------
# $(LD) $(LDFLAGS)  -T ldscript.lds -o UserApp.elf $^ -l :libm10era.a -l :libc.a -l :libm.a -l :libgcc.a  -l :fsfat.a
# $(LD) $(LDFLAGS)  -T ldscript.lds -o UserApp.elf $^ -l :libm10era.a -l :libc.a -l :libm.a -l :libgcc.a -l :fsfat.a -L "C:\Program Files\CodeSourcery\Sourcery G++ Lite\arm-none-eabi\lib" -L "C:\Program Files\CodeSourcery\Sourcery G++ Lite\lib\gcc\arm-none-eabi\4.4.1"
#all: $(C_OBJECTS) $(ASM_OBJECTS) UserApp.elf

UserApp.elf: $(ASM_OBJECTS) $(C_OBJECTS)
	$(LD) $(LDFLAGS)  -T ldscript.lds -o UserApp.elf $^ -l :libm10era.a -l :libc.a -l :libm.a -l :libgcc.a -l :libfatfs.a -L LIB_PATH_1 -L LIB_PATH_2
	$(OBJCOPY) -O binary UserApp.elf UserApp.bin

#$(C_OBJECTS): %.o: %.c Makefile $(C_FILES) $(H_FILES)
#	$(CC) $(CFLAGS) -c -o $@ $<

#$(ASM_OBJECTS): %.o: %.S Makefile
#	$(CC) $(ASFLAGS) -c -o $@ $<

main.o: main.h main.c menu.c test.c data.c char.h globe.h common.h display.h sysdef.h Media.h ff.h
	$(CC) $(CFLAGS) -c main.c

common.o : common.h common.c function.c keyio.c para.c print.c user.c input.c sysdef.h globe.h hzk.h
	$(CC) $(CFLAGS) -c common.c

display.o : display.c display.h common.h globe.h sysdef.h
	$(CC) $(CFLAGS) -c display.c

sysdef.o : sysdef.c sysdef.h globe.h
	$(CC) $(CFLAGS) -c sysdef.c

ini.o: ini.c
	$(CC) $(CFLAGS) -c ini.c

gain.o:	gain.c sysdef.h
	$(CC) $(CFLAGS) -c gain.c

sdcard.o:sdcard.c sdcard.h
	$(CC) $(CFLAGS) -c sdcard.c

armtest.o:	armtest.c armtest.h
	$(CC) $(CFLAGS) -c armtest.c


all:
	$(CC) $(ASFLAGS) -c startup.s
	$(CC) $(CFLAGS) -c $(C_FILES)
	$(LD) $(LDFLAGS)  -T ldscript.lds -o UserApp.elf ${ASM_OBJECTS} $(C_OBJECTS) -l :libm10era.a -l :libc.a -l :libm.a -l :libgcc.a -l :libfatfs.a -L LIB_PATH_1 -L LIB_PATH_2
	$(OBJCOPY) -O binary UserApp.elf UserApp.bin
clean:
	rm $(C_OBJECTS) startup.o UserApp.bin UserApp.elf

