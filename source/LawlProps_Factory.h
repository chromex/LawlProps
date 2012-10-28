#ifndef LAWLPROPS_FACTORY_H
#define LAWLPROPS_FACTORY_H

#include <string>
#include <map>
#include <typeinfo>

namespace LawlProps
{
	struct Property
	{
		size_t offset;
		size_t type;
	};

	struct FactoryClass
	{
		typedef std::map<std::string, Property> PropertyMap;

		std::string name;
		PropertyMap properties;

		std::string ToString() const;
	};

	class Factory
	{
	public:
		~Factory();

		static std::string ToString();

		template< typename ObjType, typename MemberType >
		static void AddClassMember(ObjType& proto, const char* name, MemberType& member);

		template< typename MemberType, typename ObjType >
		static MemberType& Get(ObjType* obj, const char* name);

		template< typename MemberType, typename ObjType >
		static void Set(ObjType* obj, const char* name, const MemberType& value);

		static std::string TypeName(size_t type);

	private:
		// Helpers
		std::string _ToString() const;
		void _AddClassMember(const std::string& className, const std::string& propertyName, size_t offset, size_t propertyType);

		FactoryClass* AddOrCreateClass(const std::string& className);
		size_t GetPropertyOffset(size_t classType, size_t memberType, const std::string& propertyName) const;
		static std::string Demangle(const char* name);
		size_t TypeID(const std::string& name);

		// State
		typedef std::map<size_t, FactoryClass*> FactoryClassMap;
		FactoryClassMap _classMap;
		typedef std::map<std::string, size_t> TypeIDMap;
		TypeIDMap _typeIDs;

		// Singleton
		Factory();
		Factory(Factory& other) {}
		Factory& operator=(const Factory& other) {}
		static Factory& Inst();
	};

	template< typename ObjType, typename MemberType >
	void AddClassMember(ObjType& proto, const char* name, MemberType& member)
	{
		Factory::AddClassMember(proto, name, member);
	}

	template< typename ObjType, typename MemberType >
	void Factory::AddClassMember(ObjType& proto, const char* name, MemberType& member)
	{
		Inst()._AddClassMember(
			Demangle(typeid(ObjType).name()),
			name,
			(size_t)((const char*)&member - (const char*)&proto),
			Inst().TypeID(Demangle(typeid(MemberType).name()))
		);
	}

	template< typename MemberType, typename ObjType >
	MemberType& Get(ObjType* obj, const char* name)
	{
		return Factory::Get(obj, name);
	}

	template< typename MemberType, typename ObjType >
	MemberType& Factory::Get(ObjType* obj, const char* name)
	{
		size_t offset = Inst().GetPropertyOffset(
			Inst().TypeID(Demangle(typeid(ObjType).name())),
			Inst().TypeID(Demangle(typeid(MemberType).name())),
			name
		);

		return *((MemberType*)(((char*)obj) + offset));
	}

	template< typename MemberType, typename ObjType >
	void Set(ObjType* obj, const char* name, const MemberType& value)
	{
		Factory::Set(obj, name, value);
	}

	template< typename MemberType, typename ObjType >
	void Factory::Set(ObjType* obj, const char* name, const MemberType& value)
	{
		size_t offset = Inst().GetPropertyOffset(
			Inst().TypeID(Demangle(typeid(ObjType).name())),
			Inst().TypeID(Demangle(typeid(MemberType).name())),
			name
		);

		*((MemberType*)((const char*)obj + offset)) = value;
	}
}

#endif