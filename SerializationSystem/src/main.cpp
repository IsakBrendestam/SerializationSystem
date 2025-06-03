
#include <iostream>

#include "Tests/Tests.h"

int main()
{
	std::string result = Test::GetInformation() ? "Passed" : "Failed";
	std::cout << "Getting information test: " << result << std::endl;

	return 0;
}