#pragma once



// bool fk_check = false;
		//
		// storage.on_open = [&fk_check](sqlite3* p) {
		// 	if (!fk_check)
		// 	{
		// 		internal::perform_void_exec(p, "PRAGMA foreign_keys = 0");
		// 	}
		// 	else {
		// 		internal::perform_void_exec(p, "PRAGMA foreign_keys = 1");
		// 	}
		// };

template<typename Storage>
struct update_schema
{
	Storage& storage;
	update_schema(Storage& storage) : storage{storage}
	{
		storage.foreign_key(false);
		// storage.pragma.foreign_keys(false);
	}
	~update_schema()
	{
		storage.foreign_key(true);
		// storage.pragma.foreign_keys(true);
	}
	
};