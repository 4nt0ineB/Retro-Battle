COMP = clang
FLAG = -Wall -Wfatal-errors -std=c17
LIB = -lMLV
OUTPUT = main
LEVEL_FILE = ./data/levels/level

SRC_DIR = src

# ne pas afficher les commandes
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
ifdef l
	./$(OUTPUT) $(LEVEL_FILE)$(l) -a
else
	@echo level undefined
endif

# lancer le jeu dans une fenêtre
run-gui: build
ifdef l
	./$(OUTPUT) $(LEVEL_FILE)$(l) -g -x=1920 -y=1080
else
	@echo level undefined
endif

# Fuites mémoire  (Memcheck)
# --track-origins=yes --show-leak-kinds=all ////--gen-suppressions=yes
memcheck-cli: build-debug
	valgrind --leak-check=full  ./$(OUTPUT) $(LEVEL_FILE)5 -a

memcheck-gui: build-debug
	valgrind --leak-check=full ./$(OUTPUT) $(LEVEL_FILE)5 -g -x=1920 -y=1080

all: run-cli
