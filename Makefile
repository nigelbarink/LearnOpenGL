CC = g++
CFLAGS = -std=c++17 -O2 -g
LDFLAGS = -lglfw3 -lGL -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lz -lassimp
OUT_NAME = LearnOpenGL

build: src/*.cpp  src/*.h src/**/*.cpp src/**/*.h
	mkdir -p build/
	$(CC) $(CFLAGS) -Ilib/GLAD/include -Ilib/glfw/include -Ilib/stb -o build/$(OUT_NAME) lib/GLAD/src/glad.c lib/stb/stb_image.cpp src/**.cpp  src/**/*.cpp $(LDFLAGS)


run: build build/$(OUT_NAME)
	./build/$(OUT_NAME)


debug: build build/$(OUT_NAME)
	gdb build/$(OUT_NAME)