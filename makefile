NAME        := fat32-util
INCS        := include/

BUILD_DIR   := .build
LOCAL_SRCS        := $(wildcard *.c)
LOCAL_OBJS  := $(LOCAL_SRCS:%.c=$(BUILD_DIR)/%.o)

EXTERNAL_SRC_DIR := src
EXTERNAL_SRCS := $(wildcard $(EXTERNAL_SRC_DIR)/*.c)
EXTERNAL_OBJS := $(EXTERNAL_SRCS:$(EXTERNAL_SRC_DIR)/%.c=$(EXTERNAL_SRC_DIR)/.build/%.o)

OBJS        := $(LOCAL_OBJS) $(EXTERNAL_OBJS)
DEPS        := $(OBJS:.o=.d) $(EXTERNAL_OBJS:.o=.d)

CC          := gcc
CFLAGS      := -Wall -Wextra -std=gnu89 -g -DDEBUG -MMD -MP $(addprefix -I,$(INCS))

RM          := rm -f
DIR_DUP     = mkdir -p $(@D)

all: $(NAME)

$(NAME): $(OBJS)
	$(info Building the final target : $@)
	$(DIR_DUP)
	$(CC) $(OBJS) $(CFLAGS) -o $@

$(EXTERNAL_SRC_DIR)/$(BUILD_DIR)/%.o: $(EXTERNAL_SRC_DIR)/%.c
	-$(MAKE) -C $(EXTERNAL_SRC_DIR)

$(BUILD_DIR)/%.o: %.c
	$(info Compiling object file : $@)
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c -o $@ $<

-include $(DEPS)

clean:
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(NAME)

.PHONY: clean