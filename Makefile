APPLICATION = $(notdir $(CURDIR)).out

BUILD = debug

CXX = g++
CXXFLAGS = -Wall -Wextra -no-pie -msse4.2 -mavx2 -I $(INC_DIR)

CC = gcc
CFLAGS = -Wall -Wextra -no-pie -msse4.2 -mavx2 -I $(INC_DIR)

AS = nasm
ASFLAGS = -felf64

ifeq ($(BUILD), debug)
	CFLAGS += -O0 -g -fdiagnostics-color=always
	CXXFLAGS += -O0 -g -fdiagnostics-color=always
	BUILD_DIR = build-debug
else
	ASFLAGS += -s
	CFLAGS += -s -O2
	CXXFLAGS += -s -O2
	BUILD_DIR = build-release
endif

SRC_DIR  = src
BIN_DIR  = bin
INC_DIR  = inc

SRC =  $(shell find $(SRC_DIR) -name "*.c")
ASM =  $(shell find $(SRC_DIR) -name "*.asm")

CC_OBJ = $(addprefix $(BIN_DIR)/, $(SRC:$(SRC_DIR)/%=%.o))
AS_OBJ = $(addprefix $(BIN_DIR)/, $(ASM:$(SRC_DIR)/%=%.o))

$(BUILD_DIR)/$(APPLICATION): $(CC_OBJ) $(AS_OBJ)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(CFLAGS)

-include $(addprefix $(BIN_DIR)/, $(SRC:.c=.d))
-include $(addprefix $(BIN_DIR)/, $(ASM:.asm=.d))

$(CC_OBJ) : $(BIN_DIR)/%.c.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MD -c -o $@ $< $(CFLAGS)

$(AS_OBJ) : $(BIN_DIR)/%.asm.o : $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) -MD -o $@ $< $(ASFLAGS)

.PHONY: info clean

info:
	@echo "[*] Sources: ${SRC}             "
	@echo "[*] Objects: ${CC_OBJ} $(AS_OBJ)"

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(BUILD_DIR)
