#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unordered_map>
#include "sm3.h"
#define SIZE 8	
auto hash(uint64_t data) {
	uint64_t buf[4];
	SM3().join_last((uint8_t*)&data, SIZE, (uint8_t*)buf);
	return buf[0] & (1ull << 8 * SIZE) - 1;
}
auto birthday_attack(uint64_t seed) {
	std::unordered_map<uint64_t, uint64_t> map;
	for (uint64_t i = seed;; i++) {
		auto n = hash(i);
		if (map.count(n)) {
			return std::pair<uint64_t, uint64_t>(map[n], i);
		}
		map[n] = i;
	}
}
int main() {
	uint64_t buf[4];
	uint64_t seed = 0;
	printf("seed = ");
	print_digest((uint8_t*)&seed, SIZE);
	auto start = clock();
	auto res = birthday_attack(seed);
	auto end = clock();
	SM3().join_last((uint8_t*)&res.first, SIZE, (uint8_t*)buf);
	printf("Ma   = ");
	print_digest((uint8_t*)&res.first, SIZE);
	printf("Ha   = ");
	print_digest((uint8_t*)buf, SIZE);
	SM3().join_last((uint8_t*)&res.second, SIZE, (uint8_t*)buf);
	printf("Mb   = ");
	print_digest((uint8_t*)&res.second, SIZE);
	printf("Hb   = ");
	print_digest((uint8_t*)buf, SIZE);
	printf("time = %ld ms\n", end - start);
}