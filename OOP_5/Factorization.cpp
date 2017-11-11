#include "Factorization.h"
#include <iostream>

using namespace std;

Factorization::Factorization(uint64_t& number) : source(number) {}

void Factorization::Calculation()
{
	uint64_t number = source;
	for (auto i = 2; i <= sqrt(number); i++)
	{
		while (number % i == 0)
		{
			factors[i]++;
			number /= i;
		}
	}

	if (number != 1)
		factors[number]++;
}

bool Factorization::Check() const
{
	uint64_t result = 1;
	for (auto& obj : this->factors)
	{
		for (auto i = 0; i < obj.second; i++)
			result *= obj.first;
	}
	return result == source;
}

string Factorization::Description() const
{
	string toReturn = to_string(source) + ' ';
	for (auto& obj : this->factors)
	{
		for (auto i = 0; i < obj.second; i++)
			toReturn += (to_string(obj.first) + ' ');
	}
	return toReturn + '\n';
}