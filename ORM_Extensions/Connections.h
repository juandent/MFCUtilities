#pragma once

#include <exception>

#include "Data.h"

// Type: Fondo identified by: Fondo.id
// is referenced by	: Rendimiento using Rendimiento::fkey_fondo
//					: Inversion using Inversion::fkey_fondo
// 

template <typename T, int T::* K>
struct TableKey
{
	using Table = T;
	using KeyType = decltype(K);

	static constexpr KeyType Key = K;

	static void setRecord(const Table* const record)
	{
		tableData = record;
	}
	static int getKeyValue()
	{
		if (tableData == nullptr) throw std::exception{ "tableData is null" };
		return tableData->*Key;
	}
private:
	inline static const Table* tableData = nullptr;
};


template <typename T, typename ...RefBy>
struct TableDef
{
	using Target = T;
	static std::tuple<RefBy...> reference_list;

	static bool has_links(const typename Target::Table& targetRec)
	{
		Target::setRecord(&targetRec);
		auto pk = Target::getKeyValue();
		if( pk == -1)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<Fondo>() << " no está almacenado";
			throw std::exception(ss.str().c_str());
		}

		constexpr size_t size = std::tuple_size_v<decltype(reference_list)>;

		bool has = has_links<size>();
		if(has)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<Fondo>() << " tiene dependientes";
			throw std::exception(ss.str().c_str());
		}
		return has;
	}
private:
	template<size_t index>
	static bool has_links()
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		// is a TableKey
		using DependentClass = std::tuple_element_t<index - 1, decltype(reference_list)>;

		bool has = storage.count<DependentClass::Table>(where(is_equal(DependentClass::Key, Target::getKeyValue()))) > 0;

		return has || has_links<index - 1>();
	}
	template<>
	static bool has_links<0>()
	{
		return false;
	}

};

// connection between 2 TableKey's:
// Dependent pair
//		DepPair::Table = Rendimiento
//		DepPair::Key =   getKeyValue()
// Target pair
//		TargetPair::Table = Fondo
//		TargetPair::Key = &Fondo.id

template <typename DepPair, typename TargetPair>
struct TableConnection
{
	using Dependent = DepPair;
	using Target = TargetPair;

	static bool foreignKeyExists(const typename Dependent::Table& dep)
	{
		Dependent::setRecord(&dep);

		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		int count = storage.count<typename Target::Table>(where(is_equal(Target::Key, Dependent::getKeyValue())));
		return count > 0;
	}
};


// T and Conns are TableConnection instances
template <typename T, typename ...Conns>
struct TableConnections
{
	using DependentTable = typename T::Dependent::Table;
	static std::tuple<T, Conns...> connections_list;

	static bool foreignKeysExist(const DependentTable& dep)
	{
		constexpr size_t size = std::tuple_size_v<decltype(connections_list)>;
		bool exists = foreignKeyExists<size>(dep);

		if( ! exists)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<DependentTable>() << " contiene dangling FKs";
			throw std::exception(ss.str().c_str());
		}
		return exists;
	}
private:
	template<size_t index>
	static bool foreignKeyExists(const DependentTable& dep)
	{
		// is a TableConnection
		using Connection = std::tuple_element_t<index - 1, decltype(connections_list)>;
		bool exists = Connection::foreignKeyExists(dep);

		return exists && foreignKeyExists<index - 1>(dep);
	}
	template<>
	static bool foreignKeyExists<0>(const DependentTable& dep)
	{
		return true;
	}
};
