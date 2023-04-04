 # PCAD - Pthread Matrix
 # Prima consegna del corso di PCAD dell'Universita' degli Studi di Genova
 # Programma per la manipolazione di matrici in parallelo 
 # creata usando la libreria pthread
 #
 # Copyright (C) 2023 Andrea Valenzano 4548315
 # Copyright (C) 2023 Lorenzo Contino 4832500
 # Copyright (C) 2023 Eugenio Pallestrini 4878184
 #
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation; either version 2 of the License, or
 # (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License along
 # with this program; if not, write to the Free Software Foundation, Inc.,
 # 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 


VERSION := 0.1

CC := clang
CFLAGS := -Wall -Wextra -pedantic -Werror
CUDA_FLAGS := $(CFLAGS) -DCUDA
DEBUG_FLAGS := $(CFLAGS) -g3 -ggdb3 -O0 -fsanitize=address -fsanitize=undefined
RELEASE_FLAGS := $(CFLAGS) -O3
DEBUG_CUDA_FLAGS := $(CUDA_FLAGS) -g3 -ggdb3 -O0 -fsanitize=address -fsanitize=undefined
RELEASE_CUDA_FLAGS := $(CUDA_FLAGS) -O3

BIN_DIR := bin/
SRC_DIR := src/

PROGRAM := $(BIN_DIR)matrix
CUDA_PROGRAM := $(PROGRAM)_cuda
SRC := $(SRC_DIR)main.c


all: $(PROGRAM)
debug: $(PROGRAM)_debug
release: $(PROGRAM)-$(VERSION) 

$(PROGRAM): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

$(PROGRAM)_debug: $(SRC)
	$(CC) $(DEBUG_FLAGS) $< -o $@

$(PROGRAM)-$(VERSION): $(SRC)
	$(CC) $(RELEASE_FLAGS) $< -o $@

# CUDA support needed
cuda: $(CUDA_PROGRAM)
debug_cuda: $(CUDA_PROGRAM)_debug
release_cuda: $(CUDA_PROGRAM)-$(VERSION) 

$(CUDA_PROGRAM): $(SRC)
	$(CC) $(CUDA_FLAGS) $< -o $@

$(CUDA_PROGRAM)_debug: $(SRC)
	$(CC) $(DEBUG_CUDA_FLAGS) $< -o $@

$(CUDA_PROGRAM)-$(VERSION): $(SRC)
	$(CC) $(RELEASE_CUDA_FLAGS) $< -o $@


everything: all debug release cuda debug_cuda release_cuda


.PHONY: clean

clean:
	rm -f $(BIN_DIR)* 
