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
    std::string m_name = "Isak";
    int m_age = 24;
    std::string m_city = "Karlskrona";
};
SERIALIZEBLE(Person)

class PersonPair
{
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


class Tester
{
public:
	static inline bool GetInformation()
	{
		return false;
	}

	static inline bool SerilizationDefault()
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

	static inline bool SerilizationClassMember()
	{
		PersonPair pp({ "Isak", 24, "Karlskrona" }, { "Matilda", 23, "Karlskrona" });
		for (auto& data : PersonPair::GetData())
			data->Serialize(std::cout, &pp);

		return false;
	}

};

bool Test::RunTests()
{
    std::string result;

	/*
	result = Tester::GetInformation() ? "Passed" : "Failed";
	std::cout << "Getting information test: " << result << std::endl;
	*/

	result = Tester::SerilizationDefault() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << "--------------------------------------------------" << std::endl
			  << "Serilization Default test: (" << result << ")" << std::endl
			  << "--------------------------------------------------" << std::endl;

	result = Tester::SerilizationClassMember() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << "--------------------------------------------------" << std::endl
			  << "Serilization Class Member test: (" << result << ")" << std::endl
			  << "--------------------------------------------------" << std::endl;

    return false;
}
