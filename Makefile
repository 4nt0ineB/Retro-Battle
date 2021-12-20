COMP = clang
FLAG = -Wall -Wfatal-errors
#LIB = -lMLV
OUTPUT = main

SRC_DIR = src
# récupérer tous les noms de fichier en .c dans SRC_DIR
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
#SRC = $(addsuffix .c, $(addprefix, $(SRC_DIR)/,$(SRC_FILES)))

.SILENT: build
build: $(SRC_FILES)
	$(COMP) $(FLAG) $(SRC_FILES) -o bin/$(OUTPUT)

.SILENT: run
run: build
	bin/./$(OUTPUT)

all: run