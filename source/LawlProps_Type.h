#ifndef LAWLPROPS_TYPE_H
#define LAWLPROPS_TYPE_H

#include <string>
#include <map>
#include <typeinfo>

namespace LawlProps
{
	class TypeMetaBase;
	struct PropertyEntry
	{
		size_t offset;
		TypeMetaBase* type;
	};

	typedef std::map<std::string, PropertyEntry> PropertyMap;

	class TypeMetaBase
	{
	public:
		TypeMetaBase(const std::string name, size_t id)
			: _name(name)
			, _id(id)
		{}

		size_t GetID() const {return _id;}
		const std::string& GetName() const {return _name;}
		const PropertyMap& GetPropertyMap() const {return _properties;}

	protected:
		std::string _name;
		size_t _id;
		PropertyMap _properties;
	};

	class TypeStore
	{
	public:
		static std::string Demangle(const char* name);
		static size_t UniqueID(const std::string& name);

		static TypeStore& Instance();

		TypeMetaBase* GetTypeMetaInstance(const std::string& name);
		void SetTypeMetaInstance(const std::string& name, TypeMetaBase* ptr);
		std::string ToString() const;

	private:
		typedef std::map<std::string, TypeMetaBase*> NameMap;
		NameMap _nameMap;
	};

	template< typename ObjType >
	class TypeMeta : public TypeMetaBase
	{

	public:
		static size_t ID() { return Instance()->_id; }
		static const std::string& Name() { return Instance()->_name; }

		template< typename MemberType >
		static void AddMember(const char* name, const ObjType& obj, const MemberType& member)
		{
			PropertyMap& properties = Instance()->_properties;

			if(properties.end() != properties.find(name))
			{
				throw std::exception();
			}

			PropertyEntry& pe = properties[name];
			pe.offset = (size_t)((const char*)&member - (const char*)&obj);
			pe.type = TypeMeta<MemberType>::Instance();
		}

		static TypeMeta<ObjType>* Instance()
		{
			static TypeMeta<ObjType>* inst = GetInstance();
			return inst;
		}

	private:
		TypeMeta()
		{}
		TypeMeta(const std::string& name, size_t id)
			: TypeMetaBase(name, id)
		{}
		TypeMeta(const TypeMeta& other)
		{}
		TypeMeta& operator=(const TypeMeta& other)
		{
			return *this;
		}

		// Helpers

		static TypeMeta<ObjType>* GetInstance()
		{
			std::string name = TypeStore::Demangle(typeid(ObjType).name());

			TypeMetaBase* ptr = TypeStore::Instance().GetTypeMetaInstance(name);
			if(0 == ptr)
			{
				ptr = new TypeMeta<ObjType>(name, TypeStore::UniqueID(name));
				TypeStore::Instance().SetTypeMetaInstance(name, ptr);
			}

			return (TypeMeta<ObjType>*)ptr;
		}
	};
}

#endif