INCS := ../include

SRCS        := $(wildcard *.c)
BUILD_DIR   := .build
OBJS        := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

CC=gcc
CFLAGS= -Wall -Wextra -std=gnu89 -g -MMD -MP $(addprefix -I,$(INCS))

RM          := rm -f
DIR_DUP     = mkdir -p $(@D)

all: $(OBJS)

$(BUILD_DIR)/%.o: %.c
	$(info Compiling object file : $@)
	$(DIR_DUP)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPS)
	
clean:
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(NAME)

.PHONY: clean $(LIBS_TARGET)