default:
	clang src/*.c -o main -lallegro -lallegro_image -lallegro_audio -lallegro_acodec -lm -g
	./main.exe