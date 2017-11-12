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

private:
	const uint16_t queueSize = 10;
	const std::string inFile, outFile;

	bool stop = 0;
	std::recursive_mutex m;
	std::queue<Factorization> objs;

	void WorkWithFiles();
	void Calculation();
	void WriteFile(std::ofstream &ofs);
	void ReadFile(std::ifstream &ifs);
};