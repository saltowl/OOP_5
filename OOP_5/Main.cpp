#include "FileParser.h"
#include <iostream>
#include "Exception.h"

using namespace std;

int main(int argc, char* argv[])
{
	const string input = argv[1];
	const string output = argv[2];

	try
	{
		FileParser f(input, output);
	}
	catch (ProgramException& e)
	{
		cerr << e.what();
	}
	return 0;
}