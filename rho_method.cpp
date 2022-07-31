#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sm3.h"
#define SIZE 4
struct hash {
	uint8_t data[SIZE];
	hash next() const {
		uint8_t buf[32];
		SM3().join_last(data, SIZE, buf);
		hash result;
		memcpy(result.data, buf, SIZE);
		return result;
	}
	bool operator==(hash const& rval) const {
		return !memcmp(data, rval.data, SIZE);
	}
};

auto get_rho(hash const& seed) {
	hash x = seed;
	for (uint64_t n = 1;; n <<= 1) {
		hash y = x;
		for (uint64_t rho = 0; rho < n;) {
			x = x.next();
			if (rho++, x == y) {
				return rho;
			}
		}
	}
}
auto rho_method(hash const& seed) {
	uint64_t rho = get_rho(seed);
	hash x = seed, y = seed;
	for (uint64_t i = 0; i < rho; i++) {
		x = x.next();
	}
	for (;;) {
		auto x_tmp = x.next();
		auto y_tmp = y.next();
		if (x_tmp == y_tmp) {
			return std::pair<hash, hash>(x, y);
		}
		x = x_tmp;
		y = y_tmp;
	}
}

int main() {
	hash seed = {};
	printf("seed = ");
	print_digest(seed.data, SIZE);
	auto start = clock();
	auto res = rho_method(seed);
	auto end = clock();
	printf("Ma   = ");
	print_digest(res.first.data, SIZE);
	printf("Ha   = ");
	print_digest(res.first.next().data, SIZE);
	printf("Mb   = ");
	print_digest(res.second.data, SIZE);
	printf("Hb   = ");
	print_digest(res.second.next().data, SIZE);
	printf("time = %ld ms\n", end - start);
}