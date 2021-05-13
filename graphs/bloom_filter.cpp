/*
 * With a 4.3MB Bloom filter and 13 hash functions, inserting 1.8M items
 * took roughly 189 nanoseconds per element on my laptop.
 * The false positive rate also matched the theoretical value.
 */
#include "MurmurHash3.h"
/*
 * Function to calculate the 128 bit hash of a given item. We’ll be using MurmurHash3,
 * a 128 bit hash function which has good tradeoffs between performance, distribution, avalanche behavior,
 * and collision resistance. As this function generates a 128 bit hash, and we need 2 64 bit hashes, we can
 * split the returned hash in half to get hasha(x) and hashb(x)!
 */
std::array<uint64_t, 2> hash(const uint8_t *data,
                             std::size_t len) {
  std::array<uint64_t, 2> hashValue;
  MurmurHash3_x64_128(data, len, 0, hashValue.data());

  return hashValue;
}
/*
 * function to return the output of the nth hash function.
 */
inline uint64_t nthHash(uint8_t n,
                        uint64_t hashA,
                        uint64_t hashB,
                        uint64_t filterSize) {
    return (hashA + n * hashB) % filterSize;
}

struct BloomFilter {
	BloomFilter(uint64_t size, uint8_t numHashes)
		: m_bits(size), m_numHashes(numHashes) {}

	void add(const uint8_t *data, std::size_t len);//add
	bool possiblyContains(const uint8_t *data, std::size_t len) const;//query

	private:
	uint8_t m_numHashes;//number of hash functions to use
	std::vector<bool> m_bits;//filter size in bits
	//the range of the number returned by the hash function should be at least as large as the filter size
};

void BloomFilter::add(const uint8_t *data, std::size_t len) {
	auto hashValues = hash(data, len);

	for (int n = 0; n < m_numHashes; n++) {
		m_bits[nthHash(n, hashValues[0], hashValues[1], m_bits.size())] = true;
	}
}

bool BloomFilter::possiblyContains(const uint8_t *data, std::size_t len) const {
	auto hashValues = hash(data, len);

	for (int n = 0; n < m_numHashes; n++) {
		if (!m_bits[nthHash(n, hashValues[0], hashValues[1], m_bits.size())]) {
			return false;
		}
	}

	return true;
}
