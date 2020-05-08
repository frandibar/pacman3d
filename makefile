# Makefile de ejemplo para programa cliente-servidor en C/C++
# 
# Creado: vie abr 27 00:37:46 ART 2007
#
# Copyleft 2007 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]
#

# CONFIGURACION
################

# Extensi�n de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C.
CFLAGS =-I"./" -Wall -ansi

# Para que explote lo mas posible
#CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
#CFLAGS += -DDEBUG -ggdb

# Opciones para el compilador C++.
CXXFLAGS = $(CFLAGS) -fno-inline

# Opciones del enlazador.
LDFLAGS= -lm -lpthread -lSDL -lSDL_mixer -lGL -lGLU

# Bibliotecas estaticas a agregar
LDADD=/usr/lib/libm.a /usr/lib/libpthread.a /usr/lib/libpthread_nonshared.a

# Usamos ccache para acelerar la cosa...
#CC=ccache gcc
#CXX=ccache g++


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrego flags y libs de GTK+ de ser necesario.
#ifdef gtk
CFLAGS += $(shell pkg-config --cflags libglademm-2.4) 
CXXFLAGS += $(shell pkg-config --cflags libglademm-2.4)
LDFLAGS += $(shell pkg-config --libs libglademm-2.4 ) 
#endif

# Uso enlazador de c++ si es c�digo no C.
ifeq ($(extension), c)
enlazador = $(CC)
else
enlazador = $(CXX)
endif

#Archivos del clean
directorios = ./client/*.o ./common/*.o ./editor/*.o ./server/*.o ./common/tinyxml/*.o

# Fuentes del cliente
fuentes_client = $(wildcard ./client/*.$(extension))
# Fuentes del servidor
fuentes_server = $(wildcard ./server/*.$(extension))
#Fuentes common
fuentes_common = $(wildcard ./common/*.$(extension)) $(wildcard ./common/tinyxml/*.$(extension))
#Fuentes del editor
fuentes_editor = $(wildcard ./editor/*.$(extension)) 

main_server = $(wildcard ./server_main.$(extension))
main_editor = $(wildcard ./editor_main.$(extension))
main_client = $(wildcard ./client_main.$(extension))



# REGLAS
#########

.PHONY: all clean

all: editor server client

server_main = $(patsubst %.$(extension),%.o,$(main_server))
client_main = $(patsubst %.$(extension),%.o,$(main_client))
editor_main = $(patsubst %.$(extension),%.o,$(main_editor))

o_common_files = $(patsubst %.$(extension),%.o,$(fuentes_common))
o_server_files = $(patsubst %.$(extension),%.o,$(fuentes_server)) $(o_common_files) 
o_client_files = $(patsubst %.$(extension),%.o,$(fuentes_client)) $(o_server_files) 
o_editor_files = $(patsubst %.$(extension),%.o,$(fuentes_editor)) $(o_server_files) 

fo_server_files = $(o_server_files) $(server_main)
fo_client_files = $(o_client_files) $(client_main)
fo_editor_files = $(o_editor_files) $(editor_main)

client: $(fo_client_files)
	@if [ -z "$(fo_client_files)" ]; \
	then \
		echo "No hay archivos de entrada para el cliente ."; \
		false; \
	fi >&2
	$(enlazador) $(LDFLAGS) $(fo_client_files) $(LOADLIBES) $(LDLIBS) -o Pacman3D_client $(LDADD)

server: $(fo_server_files)
	@if [ -z "$(fo_server_files)" ]; \
	then \
		echo "No hay archivos de entrada para el servidor ."; \
		false; \
	fi >&2
	$(enlazador) $(LDFLAGS) $(fo_server_files) $(LOADLIBES) $(LDLIBS) -o Pacman3D_server $(LDADD)

editor: $(fo_editor_files)
	@if [ -z "$(fo_editor_files)" ]; \
	then \
		echo "No hay archivos de entrada para el editor ."; \
		false; \
	fi >&2
	$(enlazador) $(LDFLAGS) $(fo_editor_files) $(LOADLIBES) $(LDLIBS) -o Pacman3D_editor $(LDADD)

clean:
	@$(RM) -fv $(directorios) *.o Pacman3D_client Pacman3D_server Pacman3D_editor


