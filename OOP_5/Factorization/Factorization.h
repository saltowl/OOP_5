#pragma once
#include <map>
#include <cstdint>
#include <string>

class Factorization
{
public:
	Factorization(uint64_t& number);
	~Factorization() = default;

	uint64_t Check() const;
	std::string Description() const;

private:
	std::map <uint64_t, uint8_t> factors;
};