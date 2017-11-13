#include "FileParser.h"

using namespace std;

FileParser::FileParser(const string& in, const string& out) : inFile(in), outFile(out)
{
	WorkWithFiles();
}

void FileParser::Calculation()
{
	while (stop == 0)
	{
		lock_guard<mutex> lck(this->m);
		if (!task.empty())
		{
			cout << "Calc\n";
			task.front().Calculation();
			if (!task.front().Check())
				throw WrongAnswer("The result of multiplication of prime divisors is not equal to the original value\n");
			res.push(task.front());
			task.pop();
		}
	}
}

void FileParser::WorkWithFiles()
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

	while (true)
	{
		cout << stop << '\n';

		if (!ifs.good() && task.empty() && res.empty())
		{
			stop = 1;
			break;
		}	

		if (ifs.good() && task.size() != queueSize)
			ReadFile(ifs);
		
		if (ofs.good() && !res.empty())
			WriteFile(ofs);
	}
	if (calc.joinable()) calc.join();
	else cout << "Error\n";
	ifs.close();
	ofs.close();
}

void FileParser::WriteFile(ofstream &ofs)
{
	lock_guard<mutex> lck(this->m);
	cout << "Write\n";
	ofs.write(res.front().Description().c_str(), res.front().Description().length());
	res.pop();
}

void FileParser::ReadFile(ifstream &ifs)
{
	lock_guard<mutex> lck(this->m);
	cout << "Read\n";
	uint64_t obj;
	ifs >> obj;
	task.push(obj);
}