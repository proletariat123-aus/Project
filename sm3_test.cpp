#include "sm3.h"
#include <stdio.h>
#include <stdlib.h>
#define SIZE 4096
int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		FILE* fp = fopen(argv[i], "rb");
		if (fp == nullptr) {
			printf("Cannot open %s!", argv[i]);
		}
		SM3 sm3;
		uint8_t readbuf[SIZE], hashbuf[32];
		size_t len = fread(readbuf, 1, SIZE, fp);
		while (len == SIZE) {
			for (size_t j = 0; j < SIZE; j += 64) {
				sm3.join(readbuf + j);
			}
			len = fread(readbuf, 1, SIZE, fp);
		}
		size_t n = len - len % 64;
		for (size_t j = 0; j < n; j += 64) {
			sm3.join(readbuf + j);
		}
		sm3.join_last(readbuf + n, len % 64, hashbuf);
		printf("%s: ", argv[i]);
		print_digest(hashbuf, 32);
	}
}