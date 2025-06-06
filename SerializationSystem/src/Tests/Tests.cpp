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

class PersonCatalouge
{
friend class Tester;

public:
	PersonCatalouge() = default;
	~PersonCatalouge() = default;

	inline void AddPerson(const Person& p, unsigned int index) { if (index < CAPACITY) m_catalouge[index] = p; }

	inline bool operator==(const PersonCatalouge& other) const
	{
		for (int i = 0; i < CAPACITY; i++)
			if (m_catalouge[i] != other.m_catalouge[i])
				return false;
		return true;
	}

	INTROSPECTION(PersonCatalouge,
		MEMBER(m_catalouge)
	);

private:
	static const unsigned int CAPACITY = 2;
	Person m_catalouge[CAPACITY];
};
SERIALIZEBLE(PersonCatalouge);

class PersonPtr
{
friend class Tester;

public:
	PersonPtr() = default;
	PersonPtr(Person p) : m_ptr(new Person(p)) {};

	inline bool operator==(const PersonPtr& other) const { return *m_ptr == *other.m_ptr; }

	INTROSPECTION(PersonPtr,
		MEMBER(m_ptr)
	);

private:
	Person* m_ptr;

};
SERIALIZEBLE(PersonPtr);

Person p1("Temp", 9, "Karlskrona"),
	   p2("Temp2", 34, "Karlskrona");

PersonCatalouge pc;

std::stringstream ssTest1, ssTest2, ssTest3;

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

	static inline bool SerilizationArray()
	{
		std::stringstream ssValid;
		pc.AddPerson(p1, 0);
		pc.AddPerson(p2, 1);

		for (auto& data : PersonCatalouge::GetData())
			data->Serialize(ssTest3, &pc);

		ssValid << "m_catalouge:[\n"
				<< "{\n"
				<< "m_name:" << p1.m_name << "\n"
				<< "m_age:"	 << p1.m_age  << "\n"
				<< "m_city:" << p1.m_city << "\n"
				<< "}\n"
				<< "{\n"
				<< "m_name:" << p2.m_name << "\n"
				<< "m_age:"	 << p2.m_age  << "\n"
				<< "m_city:" << p2.m_city << "\n"
				<< "}\n"
				<< "]\n";

		std::cout << ssTest3.str();

		return ssTest3.str() == ssValid.str();
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
		PersonPair pp;
		for (auto& data : PersonPair::GetData())
			data->Deserialize(ssTest2, &pp);

		std::cout << pp << std::endl;

		return pp == PersonPair(p1, p2);
	}

	static inline bool DeserilizationArray()
	{
		PersonCatalouge pcTest;
		for (auto& data : PersonCatalouge::GetData())
			data->Deserialize(ssTest3, &pcTest);

		std::cout << pc << std::endl;
		return pcTest == pc;
	}

};

bool Test::RunTests()
{
    std::string result;
	unsigned int counter = 0,
				 counterPass = 0;

	{
		counter++;
		std::cout << "--------------------------------------------------"	<< std::endl
				  << "Serilization Default test:"							<< std::endl
				  << "--------------------------------------------------"	<< std::endl;
		result = Tester::SerilizationDefault() ? "Passed" : "Failed";
		if (result == "Passed") counterPass++;
		std::cout << std::endl 
				  << " ** Result: (" << result << ") **" << std::endl
				  << "--------------------------------------------------" << std::endl
				  << std::endl;
	}

	{
		counter++;
		std::cout << "--------------------------------------------------"	<< std::endl
				  << "Serilization Class Member test:"						<< std::endl
				  << "--------------------------------------------------"	<< std::endl;
		result = Tester::SerilizationClassMember() ? "Passed" : "Failed";
		if (result == "Passed") counterPass++;
		std::cout << std::endl 
				  << " ** Result: (" << result << ") **" << std::endl
				  << "--------------------------------------------------" << std::endl
				  << std::endl;
	}

	{
		counter++;
		std::cout << "--------------------------------------------------"	<< std::endl
				  << "Serilization Array test:"								<< std::endl
				  << "--------------------------------------------------"	<< std::endl;
		result = Tester::SerilizationArray() ? "Passed" : "Failed";
		if (result == "Passed") counterPass++;
		std::cout << std::endl 
				  << " ** Result: (" << result << ") **" << std::endl
				  << "--------------------------------------------------" << std::endl
				  << std::endl;
	}

	{
		counter++;
		std::cout << "--------------------------------------------------"	<< std::endl
				  << "Deserilization Default test:"							<< std::endl
				  << "--------------------------------------------------"	<< std::endl;
		result = Tester::DeserilizationDefault() ? "Passed" : "Failed";
		if (result == "Passed") counterPass++;
		std::cout << std::endl 
				  << " ** Result: (" << result << ") **" << std::endl
				  << "--------------------------------------------------" << std::endl
				  << std::endl;
	}

	{
		counter++;
		std::cout << "--------------------------------------------------"	<< std::endl
				  << "Deserilization Class Member test:"					<< std::endl
				  << "--------------------------------------------------"	<< std::endl;
		result = Tester::DeserilizationClassMember() ? "Passed" : "Failed";
		if (result == "Passed") counterPass++;
		std::cout << std::endl 
				  << " ** Result: (" << result << ") **" << std::endl
				  << "--------------------------------------------------" << std::endl
				  << std::endl;
	}

	{
		counter++;
		std::cout << "--------------------------------------------------"	<< std::endl
				  << "Deserilization Array test:"							<< std::endl
				  << "--------------------------------------------------"	<< std::endl;
		result = Tester::DeserilizationArray() ? "Passed" : "Failed";
		if (result == "Passed") counterPass++;
		std::cout << std::endl 
				  << " ** Result: (" << result << ") **" << std::endl
				  << "--------------------------------------------------" << std::endl
				  << std::endl;
	}



	std::cout << "--------------------------------------------------" << std::endl
			  << "**************************************************" << std::endl
			  << "Passed: " << counterPass << "/" << counter		  << std::endl
			  << "**************************************************" << std::endl
			  << "--------------------------------------------------" << std::endl;
			

    return false;
}
