#include "FileParser.h"

using namespace std;

FileParser::FileParser(const string& in, const string& out) : inFile(in), outFile(out) {}

void FileParser::Calculation()
{
	while (!stop)
	{
		lock_guard<mutex> lck(mut);
		if (!task.empty())
		{
			task.front().Calculation();
			if (!task.front().Check())
				throw WrongAnswer("The result of multiplication of prime divisors is not equal to the original value\n");
			res.push(task.front());
			task.pop();
		}
	}
}

void FileParser::Work()
{
	ifstream ifs(inFile);
	ofstream ofs(outFile);

	if (!ifs.good())
	{
		stop = 1;
		throw IOException("IO error in " + inFile + '\n');
	}

	if (!ofs.good())
	{
		stop = 1;
		throw IOException("IO error in " + outFile + '\n');
	}

	thread calc(&FileParser::Calculation, this);

	while (!stop)
	{
		if (!ifs.good() && task.empty() && res.empty())
			stop = 1;	

		if (ifs.good() && task.size() != queueSize)
			ReadFile(ifs);			
		
		if (ofs.good() && !res.empty())
			WriteFile(ofs);			
	}

	if (calc.joinable()) calc.join();
	else throw NotJoinable("The calculation thread is not joinable\n");

	ifs.close();
	ofs.close();
}

void FileParser::WriteFile(ofstream &ofs)
{
	lock_guard<mutex> lck(mut);

	if (ofs.good())
	{
		ofs.write(res.front().Description().c_str(), res.front().Description().length());
		res.pop();
	}
	else 
		throw IOException(outFile + " is damaged in the process of writing\n");
}

void FileParser::ReadFile(ifstream &ifs)
{
	lock_guard<mutex> lck(mut);

	if (ifs.good())
	{
		uint64_t obj;
		ifs >> obj;
		task.push(obj);
	}
	else 
		throw IOException(inFile + " is damaged in the process of reading\n");
}