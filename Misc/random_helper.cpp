
#include "stdafx.h"
#include "random_helper.h"

std::vector<int> getRandoms(int min, int max, int count)
{
	const size_t N = std::mt19937::state_size;
	int seed_data[N];
	std::random_device r;
	std::generate_n(seed_data, N, std::ref(r));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 gen(seq);
 
	std::uniform_int_distribution<int> dis(min, max);
	std::vector<int> ret;
	ret.resize(count);
	std::transform(ret.begin(), ret.end(), ret.begin(), 
		[dis, &gen] (int val) -> int { return dis(gen); }
	);

	return ret;
}
