#pragma once
#include <fstream>
#include <queue>
#include "Factorization.h"
#include "Exception.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

class FileParser
{
public:
	FileParser(const std::string& in, const std::string& out);
	~FileParser() = default;

	void Start();

private:
	const uint16_t minQueueSize = 2;
	const uint16_t maxQueueSize = 5;
	const std::string inFile, outFile;

	std::ifstream ifs;
	std::ofstream ofs;
	bool stop = 0, pause = 0, resume = 0;
	bool notified;
	std::condition_variable check;
	std::mutex mut;
	std::queue<Factorization> tasks, results;

	void Calculation();
	void WriteFile();
	void ReadFile();
	void ReadConsole();
	void Wait();
};