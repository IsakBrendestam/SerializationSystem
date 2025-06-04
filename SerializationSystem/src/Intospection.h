#pragma once

#include <iterator>
#include <cstddef>

struct member_interface
{
	virtual ~member_interface() = default;

	virtual char const* Name() const = 0;
	virtual void Serialize(std::ostream& out, const void* instance) const = 0;
};

template<typename TClass, typename TMember>
struct member_t : member_interface
{
	member_t(char const* name, TMember TClass::*ptr) :
		m_name(name), m_ptr(ptr){}

	inline char const* Name() const override{ return m_name; }

	inline void Serialize(std::ostream& out, const void* instance) const
	{
		const TClass* obj = static_cast<const TClass*>(instance);
		const TMember& value = obj->*m_ptr;
		out << m_name << ":" << value << "\n"; // NOTE: This format can be changed
	}

private:
	TMember TClass::* m_ptr;
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
	Iterator end() { return Iterator(&m_data[m_size - 1]); }

private:
	std::unique_ptr<member_interface>* m_data;
	size_t m_size;
};

#define INTROSPECTION(type, members)								\
	typedef type self_t;											\
	static inline std::unique_ptr<member_interface> data[] = {		\
		members														\
	};																\


#define MEMBER(name) \
	member_instance(#name, &self_t::name),
