#include "Tests.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include "../Intospection.h"

class Person 
{
friend class Tester;

public:
    Person(const std::string& name, int age, const std::string& city) :
        m_name(name), m_age(age), m_city(city) {};

    INTROSPECTION(Person,
        MEMBER(m_name)
        MEMBER(m_age)
        MEMBER(m_city)
    );

private:
    std::string m_name = "Isak";
    int m_age = 24;
    std::string m_city = "Karlskrona";
};

class Tester
{
public:
	static inline bool GetInformation()
	{
		return false;
	}

	static inline bool Serilization()
	{
		Person t("Isak", 24, "Karlskrona");
		std::stringstream ssTest, ssValid;

		for (auto& data : Person::GetData())
			data->Serialize(ssTest, &t);

		ssValid << "m_name:" << t.m_name << "\n"
				<< "m_age:"  << t.m_age  << "\n"
				<< "m_city:" << t.m_city << "\n";

		std::cout << ssTest.str();

		return ssTest.str() == ssValid.str();
	}

};

bool Test::RunTests()
{
    std::string result;

	/*
	result = Tester::GetInformation() ? "Passed" : "Failed";
	std::cout << "Getting information test: " << result << std::endl;
	*/

	result = Tester::Serilization() ? "Passed" : "Failed";
	std::cout << "Serilization test: " << result << std::endl;

    return false;
}
