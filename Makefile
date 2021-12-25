COMP = clang
FLAG = -Wall -Wfatal-errors -std=c17
#LIB = -lMLV
OUTPUT = main
LEVEL_FILE = ./assets/level1.txt

SRC_DIR = src
.silent: clean
.SILENT: build
.SILENT: run

clean:
	rm $(OUTPUT) || true

# récupérer tous les noms de fichier en .c dans SRC_DIR
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
#SRC = $(addsuffix .c, $(addprefix, $(SRC_DIR)/,$(SRC_FILES)))

build: $(SRC_FILES)
	$(COMP) $(FLAG) $(SRC_FILES) -o $(OUTPUT) -g

run: build
	./$(OUTPUT) $(LEVEL_FILE)

memleak: clean build
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(OUTPUT)

all: run