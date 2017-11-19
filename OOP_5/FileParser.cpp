#include "FileParser.h"

using namespace std;

// TODO: use condition_variable
// TODO: add pause and resume

FileParser::FileParser(const string& in, const string& out) : inFile(in), outFile(out) {}

void FileParser::Calculation()
{
	while (!stop)
	{
		unique_lock<mutex> lck(mut);

		while (!notified) // loop to avoid spurious wakeups
			check.wait(lck);

		if (!tasks.empty())
		{
			while (!tasks.empty())
			{
				Factorization &curFact = tasks.front();
				lck.unlock();

				//clog << "Calculation\n";
				curFact.Calculation();
				if (!curFact.IsRight())
					throw WrongAnswer("The result of multiplication of prime divisors is not equal to the original value\n");

				lck.lock();
				results.push(curFact);
				tasks.pop();
			}
		}
		notified = 0;
	}
}

void FileParser::Work()
{
	thread readFile(&FileParser::ReadFile, this);
	thread calc(&FileParser::Calculation, this);
	thread writeFile(&FileParser::WriteFile, this);

	if (calc.joinable()) calc.join();
	else throw NotJoinable("The calculation thread is not joinable\n");

	if (readFile.joinable()) readFile.join();
	else throw NotJoinable("The ReadFile thread is not joinable\n");

	if (writeFile.joinable()) writeFile.join();
	else throw NotJoinable("The WriteFile thread is not joinable\n");
}

void FileParser::WriteFile()
{
	ofstream ofs(outFile);

	if (!ofs.good())
	{
		stop = 1;
		throw IOException("IO error in " + outFile + '\n');
	}

	while (!stop)
	{
		if (!ofs.good())
		{
			stop = 1;
			throw IOException(outFile + " is damaged in the process of writing\n");
		}

		while (!results.empty())
		{
			unique_lock<mutex> lck(mut);

			//clog << "Write\n";
			if (ofs.good())
			{
				ofs.write(results.front().Description().c_str(), results.front().Description().length());
				results.pop();
			}
			else throw IOException(outFile + " is damaged in the process of writing\n");
		}
	}
}

void FileParser::ReadFile()
{
	ifstream ifs(inFile);

	if (!ifs.good())
	{
		stop = 1;
		throw IOException("IO error in " + inFile + '\n');
	}
	
	while (!stop)
	{
		if (!ifs.good() && !ifs.eof())
		{
			stop = 1;
			throw IOException(inFile + " is damaged in the process of reading\n");
		}

		unique_lock<mutex> lck(mut);

		//clog << "Read\n";
		if (tasks.size() < minQueueSize)
		{
			if (!ifs.good() && tasks.empty() && results.empty())
				stop = 1;

			while (ifs.good() && tasks.size() < maxQueueSize)
			{
				uint64_t obj;
				ifs >> obj;
				tasks.push(obj);
			}

			notified = 1;
			check.notify_one();
		}
	}
}