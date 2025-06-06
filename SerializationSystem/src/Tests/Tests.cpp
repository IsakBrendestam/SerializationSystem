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

	inline bool operator==(const Person& other) const 
	{
		return m_name == other.m_name &&
			   m_age == other.m_age &&
			   m_city == other.m_city;
	}

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
	PersonPair() = default;
	PersonPair(const Person& p1, const Person& p2) :
		m_p1(p1), m_p2(p2) {}

	inline bool operator==(const PersonPair& other) const
	{
		return m_p1 == other.m_p1 &&
			   m_p2 == other.m_p2;
	}

	INTROSPECTION(PersonPair,
		MEMBER(m_p1)
		MEMBER(m_p2)
	);

private:
	Person m_p1, m_p2;
};
SERIALIZEBLE(PersonPair);

Person p1("Temp", 9, "Karlskrona"),
	   p2("Temp2", 34, "Karlskrona");

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
				<< "m_age:"  << p1.m_age  << "\n"
				<< "m_city:" << p1.m_city << "\n"
				<< "}\n"
				<< "m_p2:{\n"
				<< "m_name:" << p2.m_name << "\n"
				<< "m_age:"  << p2.m_age  << "\n"
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

		return p3 == p1;
	}

	static inline bool DeserilizationClassMember()
	{
		std::stringstream ssValid;
		PersonPair pp;
		for (auto& data : PersonPair::GetData())
			data->Deserialize(ssTest2, &pp);

		std::cout << pp << std::endl;

		return pp == PersonPair(p1, p2);
	}

};

bool Test::RunTests()
{
    std::string result;

	/*
	result = Tester::GetInformation() ? "Passed" : "Failed";
	std::cout << "Getting information test: " << result << std::endl;
	*/

	std::cout << "--------------------------------------------------"	<< std::endl
			  << "Serilization Default test:"							<< std::endl
			  << "--------------------------------------------------"	<< std::endl;
	result = Tester::SerilizationDefault() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << " ** Result: (" << result << ") **" << std::endl
			  << "--------------------------------------------------" << std::endl
			  << std::endl;


	std::cout << "--------------------------------------------------"	<< std::endl
			  << "Serilization Class Member test:"						<< std::endl
			  << "--------------------------------------------------"	<< std::endl;
	result = Tester::SerilizationClassMember() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << " ** Result: (" << result << ") **" << std::endl
			  << "--------------------------------------------------" << std::endl
			  << std::endl;


	std::cout << "--------------------------------------------------"	<< std::endl
			  << "Deserilization Default test:"							<< std::endl
			  << "--------------------------------------------------"	<< std::endl;
	result = Tester::DeserilizationDefault() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << " ** Result: (" << result << ") **" << std::endl
			  << "--------------------------------------------------" << std::endl
			  << std::endl;


	std::cout << "--------------------------------------------------"	<< std::endl
			  << "Deserilization Class Member test:"					<< std::endl
			  << "--------------------------------------------------"	<< std::endl;
	result = Tester::DeserilizationClassMember() ? "Passed" : "Failed";
	std::cout << std::endl 
			  << " ** Result: (" << result << ") **" << std::endl
			  << "--------------------------------------------------" << std::endl
			  << std::endl;

    return false;
}
