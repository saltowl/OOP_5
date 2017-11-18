#include "FileParser.h"

using namespace std;

// TODO: add minQueueSize, maxQueueSize
// TODO: use condition_variable
// TODO: add 2 thread to read and to write
// TODO: use unique_lock
// TODO: add pause and resume

FileParser::FileParser(const string& in, const string& out) : inFile(in), outFile(out) {}

void FileParser::Calculation()
{
	while (!stop)
	{
		lock_guard<mutex> lck(mut);
		if (!tasks.empty())
		{
			tasks.front().Calculation();
			if (!tasks.front().Check())
				throw WrongAnswer("The result of multiplication of prime divisors is not equal to the original value\n");
			results.push(tasks.front());
			tasks.pop();
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
		if (!ifs.good() && tasks.empty() && results.empty())
			stop = 1;	

		if (ifs.good() && tasks.size() != queueSize)
			ReadFile(ifs);			
		
		if (ofs.good() && !results.empty())
			WriteFile(ofs);			
	}

	if (calc.joinable()) calc.join();
	else throw NotJoinable("The calculation thread is not joinable\n");
}

void FileParser::WriteFile(ofstream &ofs)
{
	lock_guard<mutex> lck(mut);

	if (ofs.good())
	{
		ofs.write(results.front().Description().c_str(), results.front().Description().length());
		results.pop();
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
		tasks.push(obj);
	}
	else 
		throw IOException(inFile + " is damaged in the process of reading\n");
}