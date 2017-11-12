#include "FileParser.h"

using namespace std;

FileParser::FileParser(const string& in, const string& out) : inFile(in), outFile(out)
{
	WorkWithFiles();
}

void FileParser::Calculation()
{
	while (!stop)
	{
		lock_guard<recursive_mutex> lck(this->m);
		if (!objs.empty())
		{
			objs.front().Calculation();
			if (!objs.front().Check())
				throw WrongAnswer("The result of multiplication of prime divisors is not equal to the original value\n");
		}
	}
}

void FileParser::WorkWithFiles()
{
	ifstream ifs(inFile);
	ofstream ofs(outFile);

	if (!ifs.good())
		throw IOException("IO error in " + inFile + '\n');

	if (!ofs.good())
		throw IOException("IO error in " + outFile + '\n');

	thread calc(&FileParser::Calculation, this);

	while (ifs.good() || ofs.good())
	{
		if (!ifs.good())
			stop = 1;		

		if (ifs.good() && objs.size() != queueSize)
		{
			ReadFile(ifs);
		}
		
		if (ofs.good() && !objs.empty())
		{
			WriteFile(ofs);
		}
	}
	if (calc.joinable()) calc.join();
	ifs.close();
	ofs.close();
}

void FileParser::WriteFile(ofstream &ofs)
{
	lock_guard<recursive_mutex> lck(this->m);
	ofs.write(objs.front().Description().c_str(), objs.front().Description().length());
	objs.pop();
}

void FileParser::ReadFile(ifstream &ifs)
{
	lock_guard<recursive_mutex> lck(this->m);
	uint64_t obj;
	ifs >> obj;
	objs.push(obj);
}