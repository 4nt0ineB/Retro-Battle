COMP = clang
FLAG = -Wall -Wfatal-errors -std=c17
LIB = -lMLV
OUTPUT = main
LEVEL_FILE = ./data/levels/level4

SRC_DIR = src
.SILENT: clean
.SILENT: build
.SILENT: run

clean:
	rm $(OUTPUT) || true

# récupérer tous les noms de fichier en .c dans SRC_DIR
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
#SRC = $(addsuffix .c, $(addprefix, $(SRC_DIR)/,$(SRC_FILES)))

build: $(SRC_FILES)
	$(COMP) $(FLAG) $(SRC_FILES) -o $(OUTPUT) -g $(LIB)

run-cli: build
	./$(OUTPUT) $(LEVEL_FILE) -a

run-gui: build
	./$(OUTPUT) $(LEVEL_FILE) -g

# Memcheck
# --track-origins=yes --show-leak-kinds=all ////--gen-suppressions=yes
memcheck-cli: clean build
	valgrind --leak-check=full  ./$(OUTPUT) $(LEVEL_FILE) -a

memcheck-gui: clean build
	valgrind --leak-check=full ./$(OUTPUT) $(LEVEL_FILE) -g

all: run-cli