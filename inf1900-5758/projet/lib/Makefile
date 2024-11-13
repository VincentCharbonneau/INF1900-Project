########       AVR Project Makefile       ########
#####                                        #####
#####      Makefile produit et ecrit par     #####
#####   Simon Barrette & Jerome Collin pour  #####
#####           INF1900 - 2016               #####
#####                                        #####
#####         Inspire de Pat Deegan -        #####
#####  Psychogenic Inc (www.psychogenic.com) #####
##################################################

#Modifier par Vincent Charbonneau, Camil Bisson, Rebecca Abi Chahine, Charles-Emile Leblond

#Ce Makefile permet de compiler une librairie
#pour le microcontroleurs ATmeaga324pa
#sur Linux ou Unix, en utilisant l'outil AVR-GCC.



MCU = atmega324pa
F_CPU=8000000UL
OPTLEVEL=s
AVRDUDE_PROGRAMMERID=usbasp

#LIGNE MODIFIABLES
PROJECTNAME=lib5758
PRJSRC =LineFollower.cpp LineSensor.cpp Sound.cpp Connection.cpp USART.cpp Led.cpp Can.cpp memoire_24.cpp Timer.cpp Pwm.cpp Button.cpp IRReceiver.cpp IRTransmitter.cpp DistanceSensor.cpp AbstractDrawable.cpp DrawableChar.cpp DrawableLine.cpp DrawableMatrix.cpp DrawableRectangle.cpp DrawableTerminatedLine.cpp MatrixProxy.cpp EEPROMMatrix.cpp Line.cpp

#compilateur utilise
CC=avr-gcc
#pour copier le contenu d'un fichier objet vers un autre
OBJCOPY=avr-objcopy
#pour permettre le transfert vers le microcontroleur
AVRDUDE=avrdude
#pour supprimer les fichiers lorsque l'on appel make clean
REMOVE=rm -f

# Garde simplement les fichier cpp
CPPFILES=$(filter %.cpp, $(PRJSRC))


CFLAGS=-I. -I/usr/include/simavr -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-std=c++14 -fpack-struct -fshort-enums \
	-funsigned-bitfields -funsigned-char \
	-Wall

CXXFLAGS=-fno-exceptions -DF_CPU=$(F_CPU)

OBJDEPS=$(CFILES:.c=.o) \
	$(CPPFILES:.cpp=.o)

TRG = $(PROJECTNAME).a

.PHONY: all clean lib

all: $(TRG) 
	@echo "Building $(TRG) from $(PRJSRC) using $(CC)"
	

$(TRG): $(OBJDEPS)
	ar -rcs $(TRG) $(OBJDEPS)

# Production des fichiers object
# De C a objet
%.o: %.c
	$(CC) $(CFLAGS) -c $<
# De C++ a objet
%.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

clean:
	$(REMOVE) $(TRG) $(OBJDEPS) *.d

lib: $(TRG)
	$(REMOVE) $(OBJDEPS) *.d
	@echo "Keeping only $(TRG)"
