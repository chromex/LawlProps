#ifndef LAWLPROPS_H
#define LAWLPROPS_H

#include <string>
#include <map>
#include <typeinfo>
#include <LawlJSON.h>

namespace LawlProps
{
	class TypeMetaBase;
	struct PropertyEntry
	{
		size_t offset;
		TypeMetaBase* type;
		bool pointer;
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
		virtual bool HasDeserializer() const = 0;
		virtual bool HasSerializer() const = 0;
		virtual void Deserialize(const std::string& str, void* obj) const = 0;
		virtual std::string Serialize(void* obj) const = 0;

		virtual void* Instantiate()=0;

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
			pe.pointer = false;
		}

		template< typename MemberBase, typename MemberType >
		static void AddPointerMember(const char* name, const ObjType& obj, const MemberType& member)
		{
			PropertyMap& properties = Instance()->_properties;

			if(properties.end() != properties.find(name))
			{
				throw std::exception();
			}

			PropertyEntry& pe = properties[name];
			pe.offset = (size_t)((const char*)&member - (const char*)&obj);
			pe.type = TypeMeta<MemberBase>::Instance();
			pe.pointer = true;
		}

		template< typename T >
		static T& Get(const ObjType& obj, const std::string& name)
		{
			PropertyMap& properties = Instance()->_properties;
			PropertyMap::const_iterator entry = properties.find(name);
			if(properties.end() == entry)
			{
				throw std::exception();
			}
			if(TypeMeta<T>::Instance()->GetID() != entry->second.type->GetID())
			{
				throw std::exception();
			}
			return *reinterpret_cast<T*>((size_t)&obj + entry->second.offset);
		}

		static void SetSerializer(std::string (*serializer)(const ObjType*))
		{
			Instance()->_serializer = serializer;
		}

		static void SetDeserializer(void (*deserializer)(const std::string&, ObjType*))
		{
			Instance()->_deserializer = deserializer;
		}

		static void SetAllocator(ObjType* (*allocator)())
		{
			Instance()->_allocator = allocator;
		}

		static TypeMeta<ObjType>* Instance()
		{
			static TypeMeta<ObjType>* inst = GetInstance();
			return inst;
		}

		virtual void* Instantiate()
		{
			if(0 == _allocator)
				return new ObjType;
			else
				return _allocator();
		}

		virtual bool HasDeserializer() const
		{
			return 0 != _deserializer;
		}

		virtual void Deserialize(const std::string& str, void* obj) const
		{
			if(0 != _deserializer)
			{
				_deserializer(str, (ObjType*)obj);
			}
		}

		virtual bool HasSerializer() const
		{
			return 0 != _serializer;
		}

		virtual std::string Serialize(void* obj) const
		{
			if(0 != _serializer)
			{
				return _serializer((const ObjType*)obj);
			}
			else
			{
				return "";
			}
		}

	private:
		TypeMeta()
		{}
		TypeMeta(const TypeMeta& other)
		{}
		TypeMeta(const std::string& name, size_t id)
			: TypeMetaBase(name, id)
			, _allocator(0)
			, _serializer(0)
			, _deserializer(0)
		{}
		TypeMeta& operator=(const TypeMeta& other)
		{
			return *this;
		}

		// Members
		ObjType* (*_allocator)();
		std::string (*_serializer)(const ObjType*);
		void (*_deserializer)(const std::string&, ObjType*);

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

	void Parse(LJObject& object, void* ptr);

	void* Instantiate(const std::string& className);

	template< typename ObjType >
	ObjType* Create(LJObject& object)
	{
		// Missing or invalid class specifier
		if(object.end() == object.find("Class") || !object["Class"].IsString())
		{
			return 0;
		}

		// Mismatch between specified type and actual
		if(TypeMeta<ObjType>::Name() != object["Class"].string())
		{
			return 0;
		}

		void* instance = Instantiate(object["Class"].string());

		Parse(object, instance);

		return static_cast<ObjType*>(instance);
	}

	template< typename ObjType >
	ObjType* Load(const char* data)
	{
		LJValue results;
		ParseJSON(data, results);

		if(!results.IsObject())
			return 0;

		return Create<ObjType>(results.object());
	}

	template< typename ObjType, typename ConType >
	void LoadArray(const char* data, ConType& container)
	{
		LJValue results;
		try
		{
			ParseJSON(data, results);
		}
		catch(std::exception&)
		{
			return;
		}

		if(!results.IsArray())
			return;

		LJArray& arr = results.array();
		for(LJArray::iterator entry = arr.begin(); entry != arr.end(); ++entry)
		{
			if(!entry->IsObject())
				continue;

			container.push_back(Create<ObjType>(entry->object()));
		}
	}

	void Export(LJObject& object, TypeMetaBase* meta, void* ptr);

	template< typename ObjType >
	std::string Save(ObjType* obj)
	{
		LJObject json;

		Export(json, TypeMeta<ObjType>::Instance(), reinterpret_cast<void*>(obj));

		std::string results;
		Serialize(json, results);
		return results;
	}

	template< typename ObjType, typename ConType >
	std::string SaveArray(ConType& container)
	{
		LJArray arr;

		for(ConType::const_iterator entry = container.begin(); entry != container.end(); ++entry)
		{
			LJObject json;
			Export(json, TypeMeta<ObjType>::Instance(), reinterpret_cast<void*>(*entry));
			arr.push_back(json);
		}

		std::string results;
		Serialize(arr, results);
		return results;
	}
}

#endif