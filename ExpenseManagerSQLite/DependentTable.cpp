#include "stdafx.h"
#include "DependentTable.h"






bool safe_remove_pais(int pkey)
{
	DependentTable<Banco> depTable;

	
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Banco>(where(is_equal(&Banco::fkey_pais, pkey)));

	if( count == 0)
	{
////		storage.remove<Pais>(pkey);
	}

	//auto preciseLilKimsCount = storage.count<Banco>(where(is_equal( Fkey, pkey)));
	
	return false;
}

