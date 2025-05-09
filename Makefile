CC = g++
FLAGS = -std=c++17 -Wall -O3
TARGET = HuffmanTool

SRCS = \
	src/tree.cpp \
	src/file.cpp \
	src/table.cpp \
	src/utils.cpp \
	src/config.cpp \
	src/main.cpp
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))

all: bin/$(TARGET) output

# 編譯 .cpp 檔案為 .o 檔案的通用規則 (Pattern Rule)
# 所有 obj/%.o 檔案都依賴於對應的 src/%.cpp 檔案，並且依賴於 obj 資料夾 (order-only prerequisite)
bin/$(TARGET): $(OBJS) | bin
	$(CC) $(FLAGS) -o $@ $(OBJS)

# 編譯 .cpp 檔案為 .o 檔案的通用規則 (Pattern Rule)
# 所有 obj/%.o 檔案都依賴於對應的 src/%.cpp 檔案，並且依賴於 obj 資料夾 (order-only prerequisite)
obj/%.o: src/%.cpp | obj
	$(CC) $(FLAGS) -c $< -o $@

bin:
	mkdir -p bin
obj:
	mkdir -p obj
output:
	mkdir -p output output/tmp

clean:
	rm -rf output obj bin

.PHONY: all clean bin obj output
