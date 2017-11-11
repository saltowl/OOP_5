#pragma once
#include <string>
#include <fstream>
#include "Factorization.h"

class FileParser
{
public:
	FileParser(const std::string& in, const std::string& out);
	~FileParser() = default;

private:
	const std::string inFile, outFile;

	void WorkWithFiles();
};