-include Makefile_common.mk
# Inclut le fichier Makefile commun
# Nom de votre projet
# (utilisez un seul mot, exemple: 'monprojet')
PROJECTNAME=librob
# Dit d'aller chercher tout les fichiers .cpp et .c
PRJSRC=$(filter-out interpreteur.cpp, $(wildcard *.cpp))  $(wildcard *.c)
# Format du fichier à produire ( a pour une librairie, out pour un exécutable )
TRGEXT = a
# Inclusions additionnels (ex: -I/path/to/mydir)
INC=

# Libraires a lier (ex: -lmylib)
LIBS=



####### Creation des commandes du Makefile ####### 
# Creation des cibles Phony (Phony Target)
# En plus de la commande make qui permet de compiler
# votre projet, vous pouvez utilisez les commandes
# make all et make clean
.PHONY: all clean debug

# Make all permet simplement de compiler le projet
#
all: $(TRG)

debug: CFLAGS += -DDEBUG
debug: $(TRG)

# Implementation de la cible
$(TRG): $(OBJDEPS)
	$(AR) $(ARFLAGS) $(TRG) $(OBJDEPS)

# Production des fichiers object
# De C a objet
%.o: %.c
	$(CC) $(CFLAGS) -c $<
# De C++ a objet
%.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

# Verification des dependances (header dependencies)
-include *.d

# Pour plus d'information sur cette section, consulter:
# http://bit.ly/2580FU8

# Production des fichiers hex a partir des fichiers elf
%.hex: %.a
	$(OBJCOPY) -j .text -j .data \
		-O $(HEXFORMAT) $< $@

# Make clean permet d'effacer tout les fichiers generes
# lors de la compilation
clean:
	$(REMOVE) $(TRG) $(TRG).map $(OBJDEPS) $(HEXTRG) *.d



# Pour plus d'information sur les phony target, consulter:
# http://bit.ly/1WBQe61

# De plus, pour mieux comprendre les makefiles et 
# leur fonctionnement, consulter la documentation de GNU Make:
# http://bit.ly/23Vpk8s

# Finalement, ce tutoriel en ligne constitut une bonne 
# introduction au Makefile:
# http://bit.ly/1XvxsN3

#####                    EOF                   #####
