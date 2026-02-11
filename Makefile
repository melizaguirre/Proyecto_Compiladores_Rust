CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
FLEX = flex
BISON = bison

BUILD = build
TARGET = rust_parser

all: $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/parser.tab.c $(BUILD)/parser.tab.h: src/parser.y | $(BUILD)
	$(BISON) -d -o $(BUILD)/parser.tab.c src/parser.y

$(BUILD)/lex.yy.c: src/lexer.l $(BUILD)/parser.tab.h | $(BUILD)
	$(FLEX) -o $(BUILD)/lex.yy.c src/lexer.l

$(TARGET): $(BUILD)/parser.tab.c $(BUILD)/lex.yy.c src/main.cpp
	$(CXX) $(CXXFLAGS) -I$(BUILD) $^ -o $@

clean:
	rm -rf $(BUILD) $(TARGET)

.PHONY: all clean

