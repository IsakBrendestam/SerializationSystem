#include "Tests.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include "../Intospection.h"

class TestClass 
{
public:
    TestClass() = default;

    std::string name = "Isak";
    int age = 24;
    std::string city = "Karlskrona";

    INTROSPECTION(TestClass,
        MEMBER(name)
        MEMBER(age)
        MEMBER(city)
    );
};

bool GetInformation()
{
    return false;
}

bool Serilization()
{
    TestClass t;
    std::stringstream ssTest, ssValid;

    for (auto& data : TestClass::GetData())
        data->Serialize(ssTest, &t);

    ssValid << "name:" << t.name << "\n"
            << "age:" << t.age << "\n"
            << "city:" << t.city << "\n";

    std::cout << ssTest.str();

    return ssTest.str() == ssValid.str();
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
