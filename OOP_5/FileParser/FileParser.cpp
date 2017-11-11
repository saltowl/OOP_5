#include "FileParser.h"
#include <iostream>
#include "Exception.h"

using namespace std;

FileParser::FileParser(const string& in, const string& out) : inFile(in), outFile(out)
{
	WorkWithFiles();
}

void FileParser::WorkWithFiles()
{
	ifstream ifs(inFile);
	ofstream ofs(outFile);
	while (ifs.good())
	{
		if (ifs.good())
		{
			uint64_t objBeg, objEnd;
			ifs >> objBeg;
			objEnd = objBeg;
			Factorization f(objEnd);

			if (f.Check() != objBeg)
				throw WrongAnswer("The result of multiplication of prime divisors is not equal to the original value\n");

			if (ofs.good())
			{
				string output = to_string(objBeg) + " : " + f.Description();
				ofs.write(output.c_str(), output.length());
			}
			else
			{
				throw IOException("IO error in " + outFile + '\n');
			}
		}
		else
		{
			throw IOException("IO error in " + inFile + '\n');
		}
	}
	ifs.close();
	ofs.close();
}