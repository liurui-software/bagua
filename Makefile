# 编译器与标志
CC = gcc
CFLAGS = -Wall -Wextra -O2

# 根据操作系统设置可执行文件名
ifeq ($(OS),Windows_NT)
    TARGET = bagua.exe
else
    TARGET = bagua
endif

# 默认目标
all: $(TARGET)

# 链接规则（直接编译源文件）
$(TARGET): bagua.c
	$(CC) $(CFLAGS) -o $@ $<

# 清理编译产物
clean:
	rm -f $(TARGET)

# 声明伪目标
.PHONY: all clean
