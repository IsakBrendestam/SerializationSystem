#pragma once

#include <iterator>
#include <cstddef>
#include <any>
#include <memory>
#include <limits>

struct member_interface
{
	virtual ~member_interface() = default;

	virtual char const* Name() const = 0;
	virtual void Serialize(std::ostream& out, const void* instance) const = 0;
	virtual void Deserialize(std::istream& in, void* instance) const = 0;
};

/*
*		Default Member
*/
template<typename TClass, typename TMember>
struct member_t : member_interface
{
	member_t(char const* name, TMember TClass::*ptr) :
		m_name(name), m_ptr(ptr){}

	inline char const* Name() const override { return m_name; }

	inline void Serialize(std::ostream& out, const void* instance) const override
	{
		const TClass* obj = static_cast<const TClass*>(instance);
		const TMember& value = obj->*m_ptr;
		out << m_name << ":" << value << "\n"; // NOTE: This format can be changed
	}

	inline void Deserialize(std::istream& in, void* instance) const override
	{
		TClass* obj = static_cast<TClass*>(instance);
		std::string label;
		if (!std::getline(in, label, ':'))
			return;

		// Optional: trim whitespace or validate `label == m_name`

		TMember value;
		in >> value;
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip to next line

		obj->*m_ptr = value;
	}

private:
	TMember TClass::* m_ptr;
	const char const* m_name;
};

/*
*		Array Member
*/
// TODO: Thorough inspection of this class is needed
template<typename TClass, typename TMember, size_t N>
struct member_t<TClass, TMember[N]> : member_interface
{
	using array_type = TMember[N];

	member_t(char const* name, TMember (TClass::*ptr)[N])
		: m_name(name), m_ptr(ptr) {}

	char const* Name() const override { return m_name; }

	void Serialize(std::ostream& out, const void* instance) const override
	{
		const TClass* obj = static_cast<const TClass*>(instance);
		out << m_name << ":[\n";
		for (size_t i = 0; i < N; ++i)
		{
			out << (*obj.*m_ptr)[i] << "\n";
		}
		out << "]\n";
	}

	void Deserialize(std::istream& in, void* instance) const override
	{
		TClass* obj = static_cast<TClass*>(instance);
		std::string label;
		if (!std::getline(in, label, ':') || label != m_name)
			return;

		std::string openBracket;
		std::getline(in, openBracket);
		if (openBracket != "[")
			return;

		for (size_t i = 0; i < N; ++i)
		{
			in >> (*obj.*m_ptr)[i];
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		std::string closeBracket;
		std::getline(in, closeBracket); // Expect "]"
	}

private:
	TMember (TClass::* m_ptr)[N];
	const char* m_name;
};


/*
*		Pointer Member
*/
template<typename TClass, typename TMember>
struct member_t<TClass, TMember*> : member_interface
{
	member_t(char const* name, TMember* TClass::*ptr) :
		m_name(name), m_ptr(ptr){}

	inline char const* Name() const override { return m_name; }

	inline void Serialize(std::ostream& out, const void* instance) const override
	{
		const TClass* obj = static_cast<const TClass*>(instance);
		const TMember* ptr = obj->*m_ptr;
		out << *ptr << "\n";
	}

	inline void Deserialize(std::istream& in, void* instance) const override
	{
		TClass* obj = static_cast<TClass*>(instance);
		std::string label;
		if (!std::getline(in, label, ':') || label != m_name)
			return;

		// Free old memory if needed
		delete (obj->*m_ptr);

		// Allocate and read
		TMember* buffer = new TMember();
		in >> *buffer;
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		obj->*m_ptr = buffer;
	}

private:
	TMember* TClass::* m_ptr;
	const char const* m_name;
};


template<typename TClass, typename TMember>
std::unique_ptr<member_interface> member_instance(char const* name, TMember TClass::* member)
{
	return std::make_unique<member_t<TClass, TMember>>(name, member);
}

class member_holder
{
public:
	member_holder(std::unique_ptr<member_interface>* data, size_t size):
		m_data(data), m_size(size) { }

	struct Iterator 
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type	= std::ptrdiff_t;
		using value_type		= std::unique_ptr<member_interface>;
		using pointer			= value_type*;
		using reference			= value_type&;

		Iterator(pointer ptr) : m_ptr(ptr) {}

		reference operator*() const { return *m_ptr; }
		pointer operator->() { return m_ptr; }

		// Prefix increment
		Iterator& operator++() { m_ptr++; return *this; }

		// Postfix increment
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
		friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }

	private:
		pointer m_ptr;
	};

	Iterator begin() { return Iterator(&m_data[0]); }
	Iterator end() { return Iterator(&m_data[m_size]); }

private:
	std::unique_ptr<member_interface>* m_data;
	size_t m_size;
};

#define INTROSPECTION(type, members)									\
	typedef type self_t;												\
	static inline member_holder GetData()								\
	{																	\
		static std::unique_ptr<member_interface> data[] = {				\
			members														\
		};																\
		return member_holder(data, sizeof(data)/sizeof(data[0]));		\
	}																	\
																		\
	friend std::ostream& operator<< (std::ostream& out, const type &obj)		\
	{																	\
		out << "{\n";													\
		for (auto& data : type::GetData())								\
			data->Serialize(out, &obj);									\
		return out << "}";												\
	}																	\
																		\
	friend std::istream& operator>>(std::istream& in, type& obj)				\
	{																	\
		std::string openBrace;											\
		std::getline(in, openBrace);									\
		if (openBrace != "{") {											\
			in.setstate(std::ios::failbit);								\
			return in;													\
		}																\
																		\
		for (auto& member : type::GetData())							\
			member->Deserialize(in, &obj);								\
																		\
		return in;														\
	}


#define MEMBER(name)													\
	member_instance(#name, &self_t::name),

