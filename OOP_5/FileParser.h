#pragma once
#include <fstream>
#include <queue>
#include "Factorization.h"
#include "Exception.h"
#include <iostream>
#include <mutex>
#include <thread>

class FileParser
{
public:
	FileParser(const std::string& in, const std::string& out);
	~FileParser() = default;

	void Work();

private:
	const uint16_t queueSize = 5;
	const std::string inFile, outFile;

	bool stop = 0;
	std::mutex m, m1;
	std::queue<Factorization> task, res;

	void Calculation();
	void WriteFile(std::ofstream &ofs);
	void ReadFile(std::ifstream &ifs);
};