COMP = clang
FLAG = -Wall -Wfatal-errors -std=c17
LIB = -lMLV
OUTPUT = main
LEVEL_FILE = ./data/levels/level5

SRC_DIR = src
.SILENT: clean
.SILENT: build
.SILENT: run

#clean:
#	rm $(OUTPUT) || true

# récupérer tous les noms de fichier en .c dans SRC_DIR
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
#SRC = $(addsuffix .c, $(addprefix, $(SRC_DIR)/,$(SRC_FILES)))

build: $(SRC_FILES)
	$(COMP) $(FLAG) $(SRC_FILES) -o $(OUTPUT) $(LIB)

# compilation pour le debug
build-debug: $(SRC_FILES)
		$(COMP) $(FLAG) $(SRC_FILES) -o $(OUTPUT) -g $(LIB)

# lancer le jeu dans le terminal
run-cli: build
	./$(OUTPUT) $(LEVEL_FILE) -a

# lancer le jeu dans une fenêtre
run-gui: build
	./$(OUTPUT) $(LEVEL_FILE) -g -x=1920 -y=1080

# Fuites mémoire  (Memcheck)
# --track-origins=yes --show-leak-kinds=all ////--gen-suppressions=yes
memcheck-cli: clean build-debug
	valgrind --leak-check=full  ./$(OUTPUT) $(LEVEL_FILE) -a

memcheck-gui: clean build-debug
	valgrind --leak-check=full ./$(OUTPUT) $(LEVEL_FILE) -g -x=1920 -y=1080

all: run-cli