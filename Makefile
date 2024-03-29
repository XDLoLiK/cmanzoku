APPLICATION = $(notdir $(CURDIR))

BUILD = debug

SRC_DIR = Source
BIN_DIR = Binary
INC_DIR = Include

CXX = g++
CXXFLAGS = -Wall -Wextra -no-pie -msse4.2 -mavx2 -I $(INC_DIR)

CC = gcc
СCFLAGS = -Wall -Wextra -no-pie -msse4.2 -mavx2 -I $(INC_DIR)

AS = nasm
ASFLAGS = -felf64

ifeq ($(BUILD), debug)
	BUILD_DIR = Build
	ASFLAGS += -g
	СCFLAGS += -O0 -g -fdiagnostics-color=always
	CXXFLAGS += -O0 -g -fdiagnostics-color=always
else
	BUILD_DIR = /usr/bin
	ASFLAGS += -s
	СCFLAGS += -s -O2
	CXXFLAGS += -s -O2
endif

SRC =  $(shell find $(SRC_DIR) -name "*.c")
ASM =  $(shell find $(SRC_DIR) -name "*.asm")

CC_OBJ = $(addprefix $(BIN_DIR)/, $(SRC:$(SRC_DIR)/%=%.o))
AS_OBJ = $(addprefix $(BIN_DIR)/, $(ASM:$(SRC_DIR)/%=%.o))

all: $(BUILD_DIR)/$(APPLICATION)

$(BUILD_DIR)/$(APPLICATION): $(CC_OBJ) $(AS_OBJ)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(СCFLAGS)

-include $(addprefix $(BIN_DIR)/, $(SRC:.c=.d))
-include $(addprefix $(BIN_DIR)/, $(ASM:.asm=.d))

$(CC_OBJ) : $(BIN_DIR)/%.c.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MD -c -o $@ $< $(СCFLAGS)

$(AS_OBJ) : $(BIN_DIR)/%.asm.o : $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) -MD -o $@ $< $(ASFLAGS)

.PHONY: info clean

info:
	@echo [*] CC:       $(CC)
	@echo [*] CCFLAGS:  $(СCFLAGS)
	@echo [*] CXX:      $(CXX)
	@echo [*] CXXFLAGS: $(CXXFLAGS)
	@echo [*] ASM:      $(AS)
	@echo [*] ASMFLAGS: $(ASFLAGS)
	@echo [*] Sources:  ${SRC} ${ASM}
	@echo [*] Objects:  ${CC_OBJ} $(AS_OBJ)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(BUILD_DIR)
