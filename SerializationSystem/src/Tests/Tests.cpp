#include "Tests.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include "../Intospection.h"

class Person 
{
friend class Tester;

public:
	Person() = default;
    Person(const std::string& name, int age, const std::string& city) :
        m_name(name), m_age(age), m_city(city) {};

    INTROSPECTION(Person,
        MEMBER(m_name)
        MEMBER(m_age)
        MEMBER(m_city)
    );

private:
    std::string m_name = "";
    int m_age = -1;
    std::string m_city = "";
};
SERIALIZEBLE(Person);

class PersonPair
{
friend class Tester;

public:
	PersonPair(const Person& p1, const Person& p2) :
		m_p1(p1), m_p2(p2) {}

	INTROSPECTION(PersonPair,
		MEMBER(m_p1)
		MEMBER(m_p2)
	);


private:
	Person m_p1, m_p2;
};
SERIALIZEBLE(PersonPair);

Person p1("Isak", 24, "Karlskrona"),
	   p2("Matilda", 23, "Karlskrona");

std::stringstream ssTest1, ssTest2;

class Tester
{
public:
	static inline bool GetInformation()
	{
		return false;
	}

	static inline bool SerilizationDefault()
	{
		std::stringstream ssValid;

		for (auto& data : Person::GetData())
			data->Serialize(ssTest1, &p1);

		ssValid << "m_name:" << p1.m_name << "\n"
				<< "m_age:"  << p1.m_age  << "\n"
				<< "m_city:" << p1.m_city << "\n";

		std::cout << ssTest1.str();

		return ssTest1.str() == ssValid.str();
	}

	static inline bool SerilizationClassMember()
	{
		std::stringstream ssValid;
		PersonPair pp(p1, p2);
		for (auto& data : PersonPair::GetData())
			data->Serialize(ssTest2, &pp);

		ssValid << "m_p1:{\n"
				<< "m_name:" << p1.m_name << "\n"
				<< "m_age:" << p1.m_age << "\n"
				<< "m_city:" << p1.m_city << "\n"
				<< "}\n"
				<< "m_p2:{\n"
				<< "m_name:" << p2.m_name << "\n"
				<< "m_age:" << p2.m_age << "\n"
				<< "m_city:" << p2.m_city << "\n"
				<< "}\n";

		std::cout << ssTest2.str();

		return ssTest2.str() == ssValid.str();
	}

	static inline bool DeserilizationDefault()
	{
		Person p3;
		for (auto& data : Person::GetData())
			data->Deserialize(ssTest1, &p3);

		std::cout << p3 << std::endl;

		bool pass = true;
		p3.m_name != p1.m_name	? pass = false :
		p3.m_age  != p1.m_age	? pass = false :
		p3.m_city != p1.m_city	? pass = false : pass = true;

		return pass;
	}

};

bool Test::RunTests()
{
    std::string result;

	/*
	result = Tester::GetInformation() ? "Passed" : "Failed";
	std::cout << "Getting information test: " << result << std::endl;
	*/

	std::cout << "--------------------------------------------------" << std::endl;
	result = Tester::SerilizationDefault() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << "--------------------------------------------------" << std::endl
			  << "Serilization Default test: (" << result << ")" << std::endl
			  << "--------------------------------------------------" << std::endl;

	std::cout << "--------------------------------------------------" << std::endl;
	result = Tester::SerilizationClassMember() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << "--------------------------------------------------" << std::endl
			  << "Serilization Class Member test: (" << result << ")" << std::endl
			  << "--------------------------------------------------" << std::endl;

	std::cout << "--------------------------------------------------" << std::endl;
	result = Tester::DeserilizationDefault() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << "--------------------------------------------------" << std::endl
			  << "Serilization Class Member test: (" << result << ")" << std::endl
			  << "--------------------------------------------------" << std::endl;

    return false;
}
