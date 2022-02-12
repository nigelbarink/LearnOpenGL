CC = g++
CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw3 -lGL -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
OUT_NAME = LearnOpenGL

build: src/*.cpp  src/*.h
	mkdir -p build/
	$(CC) $(CFLAGS) -Ilib/GLAD/include -Ilib/glfw/include -o build/$(OUT_NAME) lib/GLAD/src/glad.c src/*.cpp $(LDFLAGS)


run: build build/$(OUT_NAME)
	./build/$(OUT_NAME)