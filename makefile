# -------------------------
#      hwrng Make File
# 
#     -by Steven Novak
#
#        01.05.2018
# -------------------------

# ----- Compiler Options
CC 		    = clang
CFLAGS		= -g

# ----- Main Targets
all		    : main.c obj
	-@echo Building All
	$(CC) $(CFLAGS) main.c hwrng.o -o hwrng.exe

obj			: hwrng.h hwrng.c
	-@echo Building HWRNG
	$(CC) $(CFLAGS) -c hwrng.c -o hwrng.o

clean:
	-@del hwrng.pdb
	-@del hwrng.exe
	-@del hwrng.ilk
	-@del hwrng.o