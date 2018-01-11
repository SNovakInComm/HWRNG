# -------------------------
#      hwrng Make File
# 
#     -by Steven Novak
#
#        01.05.2018
# -------------------------

# ----- Folder Options
BIN			= bin
TEMP		= tmp

# ----- Compiler Options
CC 		    = clang
CFLAGS		= -g

# ----- Main Targets
all		    : main.c obj dirs
	-@echo "Building All"
	$(CC) $(CFLAGS) main.c hwrng.o -o $(BIN)/hwrng.exe

dirs		: 
	-@mkdir $(BIN)
	-@mkdir $(TEMP)

obj			: hwrng.h hwrng.c
	-@echo "Building HWRNG"
	$(CC) $(CFLAGS) hwrng.c -o $(TEMP)/hwrng.o

clean:
	-@del $(BIN)/*.*
	-@del $(TEMP)/*.*