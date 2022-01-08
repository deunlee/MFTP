CC = gcc
CFLAGS = -Wall -Wno-parentheses -O2
LDFLAGS = -pthread
SRC_DIR = ./src
OBJ_DIR = ./obj
TARGET = mftp

SRCS = $(notdir $(wildcard $(SRC_DIR)/*.c))
OBJS = $(SRCS:.c=.o)
OBJS_PATH = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

all: $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $< -MD

$(TARGET) : $(OBJS_PATH)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS_PATH)

.PHONY: clean all
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

-include $(DEPS)
