#ifndef LAWLPROPS_FACTORY_H
#define LAWLPROPS_FACTORY_H

#include "LawlProps_Type.h"

namespace LawlProps
{
	struct Property
	{
		size_t offset;
		size_t type;
		// Pointer to a method which can create an object of this type
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
		static bool HasClassMember(size_t type, const std::string& name);
		template< typename MemberType >
		static bool HasClassMember(size_t type, const std::string& name);

		template< typename MemberType, typename ObjType >
		static MemberType& Get(ObjType* obj, const char* name);

		template< typename MemberType, typename ObjType >
		static void Set(ObjType* obj, const char* name, const MemberType& value);

		static std::string TypeName(size_t type);
		static size_t TypeID(const std::string& name);
		static bool HasType(const std::string& name);
		template< typename ObjType >
		static bool HasType();
		
	private:
		// Helpers
		std::string _ToString() const;
		void _AddClassMember(const std::string& className, const std::string& propertyName, size_t offset, size_t propertyType);

		FactoryClass* AddOrCreateClass(const std::string& className);
		FactoryClass* GetClass(size_t type);
		size_t GetPropertyOffset(size_t classType, size_t memberType, const std::string& propertyName) const;
		size_t _TypeID(const std::string& name);
		bool _HasType(const std::string& name) const;

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
	void Factory::AddClassMember(ObjType& proto, const char* name, MemberType& member)
	{
		Inst()._AddClassMember(
			TypeMeta<ObjType>::Name(),
			name,
			(size_t)((const char*)&member - (const char*)&proto),
			TypeMeta<MemberType>::ID()
		);
	}

	template< typename MemberType >
	bool Factory::HasClassMember(size_t type, const std::string& name)
	{
		FactoryClass* fc = Inst().GetClass(type);

		FactoryClass::PropertyMap::const_iterator entry = fc->properties.find(name);
		if(entry == fc->properties.end())
			return false;

		return entry->second.type == TypeMeta<MemberType>::ID();
	}

	template< typename MemberType, typename ObjType >
	MemberType& Factory::Get(ObjType* obj, const char* name)
	{
		size_t offset = Inst().GetPropertyOffset(
			TypeMeta<ObjType>::ID(),
			TypeMeta<MemberType>::ID(),
			name
		);

		return *((MemberType*)(((char*)obj) + offset));
	}

	template< typename MemberType, typename ObjType >
	void Factory::Set(ObjType* obj, const char* name, const MemberType& value)
	{
		size_t offset = Inst().GetPropertyOffset(
			TypeMeta<ObjType>::ID(),
			TypeMeta<MemberType>::ID(),
			name
		);

		*((MemberType*)((const char*)obj + offset)) = value;
	}

	template< typename ObjType >
	bool Factory::HasType()
	{
		return Inst()._HasType(TypeMeta<ObjType>::Name());
	}
}

#endif