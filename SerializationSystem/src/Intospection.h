#pragma once

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


#define INTROSPECTION(type, members)								\
	typedef type self_t;											\
	static inline std::unique_ptr<member_interface> data[] = {		\
		members														\
	};																\


#define MEMBER(name) \
	member_instance(#name, &self_t::name),
