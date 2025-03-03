SRC_DIR = src
BUILD_DIR = build

# Name of final executable
override OUTPUT = trash

# C compiler flags
override CFLAGS = \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -O3

# C preprocessor flags
override CPPFLAGS = \
    -I $(SRC_DIR) \
    -I $(SRC_DIR)/include \

override SRCFILES := $(shell cd $(SRC_DIR) && find -L * -type f | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override OBJ := $(addprefix $(BUILD_DIR)/obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))

.PHONY: all
all: $(BUILD_DIR)/bin/$(OUTPUT)
	
$(BUILD_DIR)/bin/$(OUTPUT): $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

$(BUILD_DIR)/obj/%.c.o: src/%.c 
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	
# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

