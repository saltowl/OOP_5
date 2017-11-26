#include "FileParser.h"

using namespace std;

// TODO: add condition_variable to WriteFile()

FileParser::FileParser(const string& in, const string& out) : inFile(in), outFile(out) {}

void FileParser::Calculation()
{
	while (!stop)
	{
		unique_lock<mutex> lck(mut);

		while (!notified) // loop to avoid spurious wakeups
			check.wait(lck);

		while (!tasks.empty() && !stop && !pause)
		{
			Factorization &curFact = tasks.front();
			lck.unlock();

			//logFile << "Calculation\n";
			curFact.Calculation();
			if (!curFact.IsRight())
				throw WrongAnswer("The result of multiplication of prime divisors is not equal to the original value\n");

			lck.lock();
			results.push(curFact);
			tasks.pop();
		}

		if (pause) Wait();

		notified = 0;
		check.notify_one();
	}
}

void FileParser::Start()
{
	ifs.open(inFile);
	if (!ifs.good())
	{
		stop = 1;
		throw IOException("IO error in " + inFile + '\n');
	}

	ofs.open(outFile);
	if (!ofs.good())
	{
		stop = 1;
		throw IOException("IO error in " + outFile + '\n');
	}

	thread readFile(&FileParser::ReadFile, this);
	thread calc(&FileParser::Calculation, this);
	thread writeFile(&FileParser::WriteFile, this);

	thread readConsole(&FileParser::ReadConsole, this);
	readConsole.detach();

	if (calc.joinable()) calc.join();
	else throw NotJoinable("The calculation thread is not joinable\n");

	if (readFile.joinable()) readFile.join();
	else throw NotJoinable("The ReadFile thread is not joinable\n");

	if (writeFile.joinable()) writeFile.join();
	else throw NotJoinable("The WriteFile thread is not joinable\n");
}

void FileParser::ReadConsole()
{
	while (!stop)
	{
		string line;
		cin >> line;
		switch (line[0])
		{
		case 'e':
			stop = 1;
			break;
		case 'p':
			pause = 1;
			break;
		case 'r':
			resume = 1;
			pause = 0;
			break;
		default:
			break;
		}
	}
}

void FileParser::Wait()
{
	if (pause) ofs.close();

	while (pause)
	{
		if (stop) break;
	}

	if (!pause && resume)
	{
		ofs.open(outFile, std::ofstream::app);
		resume = 0;
	}
}

void FileParser::WriteFile()
{
	while (!stop)
	{
		if (!ofs.good() && !pause)
		{
			stop = 1;
			throw IOException(outFile + " is damaged in the process of writing\n");
		}

		unique_lock<mutex> lck(mut);

		while (!results.empty() && !pause)
		{
			//logFile << "Write\n";
			if (ofs.good())
			{
				ofs.write(results.front().Description().c_str(), results.front().Description().length());
				results.pop();
			}
			else throw IOException(outFile + " is damaged in the process of writing\n");
		}

		if (pause) Wait();
	}
}

void FileParser::ReadFile()
{
	while (!stop)
	{
		if (!ifs.good() && !ifs.eof() && !pause)
		{
			stop = 1;
			throw IOException(inFile + " is damaged in the process of reading\n");
		}

		unique_lock<mutex> lck(mut);

		//logFile << "Read\n";
		if (tasks.size() < minQueueSize && !pause)
		{
			if (ifs.eof() && tasks.empty() && results.empty())
				stop = 1;
			else while (ifs.good() && tasks.size() < maxQueueSize && !pause)
			{
				uint64_t obj;
				ifs >> obj;
				tasks.push(obj);
			}

			notified = 1;
			check.notify_one();
		}

		if (pause) Wait();
	}
}