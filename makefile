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
all		    : main.c
	-@echo "Building All"
	$(CC) $(CFLAGS) main.c -o hwrng.exe
