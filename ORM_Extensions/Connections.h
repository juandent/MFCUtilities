#pragma once

#include <exception>

#include "Data.h"

#include "Typelist.h"

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

template <typename DepPair, typename TargetPair>
struct TableConnection;

template<typename T, typename Tuple>
struct TypeInTuple
{
	constexpr static bool exists()
	{
		constexpr unsigned size = std::tuple_size<Tuple>::value;
		bool match = matches<size>();
		return match;
	}

private:
	template<unsigned N>
	constexpr static bool matches()
	{
		using V = std::tuple_element_t < N - 1, Tuple>;
		bool match = std::is_same_v<T, V>;
		return match || matches<N - 1>();
	}
	template<>
	constexpr static bool matches<0>()
	{
		return false;
	}

};

// T and RefBy are TableKeys
// Target is special, has primary key
template <typename T, typename ...RefBy>
struct TableDef
{
	using Target = T;
	static std::tuple<RefBy...> reference_list;

	template<typename U>     requires (TypeInTuple<U,decltype(reference_list)>::exists())	          // U must be in reference_list
	struct connection
	{
		using type = TableConnection<U, T>;
	};

	static bool has_links(const typename Target::Table& targetRec)
	{
		Target::setRecord(&targetRec);
		auto pk = Target::getKeyValue();
		if( pk == -1)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<Target::Table>() << " no está almacenado";
			throw std::exception(ss.str().c_str());
		}

		constexpr size_t size = std::tuple_size_v<decltype(reference_list)>;

		bool has = has_links<size>();
		if(has)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<Target::Table>() << " tiene dependientes";
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

#if 1
// T is TableKey we are looking if present in Tuple
// Tuple is a std::tuple<TableKeys>
//  

template<typename T, typename Tuple>
struct TableKeyInTuple
{
	using tableKey = T;
	constexpr static int index()
	{
		constexpr unsigned size = std::tuple_size<Tuple>::value;
		int index = find<size>();
		return index;
	}

	// using type = typename std::enable_if<position() != -1, tableKey>::type;

private:
	template<unsigned N>
	constexpr static int find()
	{
		using V = std::tuple_element_t < N-1, Tuple>;
		V* pV;
		T* pT;
		bool match = std::is_same_v<T, V>;
		if (match)
			return N-1;
		else 
			return find<N - 1>();
	}
	template<>
	constexpr static int find<0>()
	{
		return -1;
	}
};



// T is a TableKey, also known as T::Target or tableKey
// tblDefs are TableDef types
//		tblDefs[x]::reference_list is list of TableKeys
// we are going to search TableKey T in each tblDefs[x]::reference_list
//	for each found we will create a TableConnection<T, tblDefs[x]::Target>
//	we add such connection to TableConnections list by appending

template<typename T, typename...tblDefs >
struct FKDependents
{
	inline static std::array<int, sizeof...(tblDefs)> m_positions;
	using tableKey = T;
	static std::tuple<tblDefs...> tblDefs_list;

	static constexpr int findInTblDefs_list()
	{
		static constexpr int size = std::tuple_size_v<decltype(tblDefs_list)>;
		tableKey* pT;
		return find<size>();
	}

	using Collection = Loki::Typelist<Loki::NullType, Loki::NullType>;

	static constexpr  int collection_size()
	{
		static constexpr int array_size = std::tuple_size_v<decltype(tblDefs_list)>;
		int count = check_array<array_size>();
		return count;
	}

private:
	template<int N>
	static constexpr int find()
	{
		// pick one TableDef
		using tableDef = std::tuple_element_t<N-1, decltype(tblDefs_list)>;
		using tableKeys = decltype(tableDef::reference_list);

		const int n = N-1;
		tableDef* pTD;
		tableKeys* pTKs;

		int index = TableKeyInTuple < tableKey, tableKeys>::index();
		if( index != -1)
		{
			// create pair
			// (tableDef::Target, tableKey)
			typename tableDef::Target* pTDT;
			tableKey* pTK;

			// append pair to type collection
			int i = 0;
		}

		m_positions[N-1] = index;
		return find<N - 1>();
	}
	template<>
	static constexpr int find<0>()
	{
		return collection_size();
		// static constexpr int array_size = std::tuple_size_v<decltype(tblDefs_list)>;
		// int count = check_array<array_size>();
		// return count;
	}

	template<int N>
	static constexpr int check_array()
	{
		int count = m_positions[N - 1] != -1 ? 1 : 0;
		return count + check_array<N - 1>();
	}
	template<>
	static constexpr int check_array<0>()
	{
		return 0;
	}
};

#endif

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
