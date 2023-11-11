#include "stdafx.h"


import Util;

#include "DependentTable.h"





template<typename DependentTable, int DependentTable::* foreignKey, typename Table, int Table::* primaryKey >
bool exists_Link(Table record)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();
	auto count = storage.count<DependentTable>(where((c(&DependentTable::foreignKey) == (record.*primaryKey))));
	return count > 0;
}




bool exists_link()
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto category_pk = storage.select(columns(&Categoria::id_categoria));

	int pk = std::get<0>(category_pk[0]);
	
	auto category = storage.get<Categoria>(pk);

	// LinkInfo<Transaccion, c(&Transaccion::fkey_category), Categoria, &Categoria::id_categoria> link;
	// bool exists = link.existsLink(category);
	// return exists;
	return false;
}

