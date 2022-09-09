OUTPUT=main.exe

CC=mingw32-g++.exe

# -msse3 -ffast-math -march=native
C_FLAGS=-O3

#-Wl,-subsystem,windows
L_FLAGS=-flto 

LIBS=-lwinmm -lpthread

SOURCE_FILES=AudioBuffer.cpp AudioSource.cpp AudioMixer.cpp main.cpp
			
OBJECT_FILES = $(SOURCE_FILES:.cpp=.o)

build: $(OUTPUT)

clean:
	rm -rf $(OBJECT_FILES) $(OUTPUT)

rebuild: clean build

%.o: %.cpp
	$(CC) $(C_FLAGS) -c $< -o $@

$(CORE_SOURCE)/%.o: $(CORE_SOURCE)/%.cpp
	$(CC) $(C_FLAGS) -c $< -o $@

$(OUTPUT): $(OBJECT_FILES)
	$(CC) $(L_FLAGS) $(OBJECT_FILES) $(LIBS) -o $(OUTPUT)
	chmod +xr $(OUTPUT)