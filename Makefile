default:
	clang src/*.c -o main -lallegro -lallegro_image -lm -g
	./main.exe