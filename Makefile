COMP = clang
FLAG = -Wall -Wfatal-errors
#LIB = -lMLV
OUTPUT = main
LEVEL_FILE = ./assets/level1.txt

SRC_DIR = src
# récupérer tous les noms de fichier en .c dans SRC_DIR
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
#SRC = $(addsuffix .c, $(addprefix, $(SRC_DIR)/,$(SRC_FILES)))

.SILENT: build
build: $(SRC_FILES)
	$(COMP) $(FLAG) $(SRC_FILES) -o bin/$(OUTPUT) -g

.SILENT: run
run: build
	bin/./$(OUTPUT) $(LEVEL_FILE)

all: run