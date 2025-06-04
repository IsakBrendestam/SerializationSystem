#include "Tests.h"

#include <iostream>
#include "../Intospection.h"

class TestClass 
{
public:
    TestClass() = default;

    std::string name = "Isak";
    int age = 24;

    INTROSPECTION(TestClass,
        MEMBER(name)
        MEMBER(age)
    );
};

bool GetInformation()
{
    return false;
}

bool Serilization()
{
    TestClass t;
    for (int i = 0; i < 2; i++)
        TestClass::data[i]->Serialize(std::cout, &t);

    return false;
}

bool Test::RunTests()
{
    std::string result;

	result = GetInformation() ? "Passed" : "Failed";
	std::cout << "Getting information test: " << result << std::endl;

	result = Serilization() ? "Passed" : "Failed";
	std::cout << "Serilization test: " << result << std::endl;

    return false;
}
