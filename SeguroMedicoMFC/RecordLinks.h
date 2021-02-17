#pragma once
#include "PersistentClasses.h"
struct Patient;
struct Specialty;
struct Doctor;
struct Medication;
struct Claim;
struct INSResponse;
struct Invoice;
struct INSResponseLine;


class RecordLinks
{
	// template<typename T, T ...Counts>
	// static bool allNonZero(T...counts)
	// {
	// 	bool all_none_zero = (counts && ...);
	// }
	template<typename ...Counts>
	static bool allNonZero(Counts ... counts) requires (std::is_same_v<Counts, int> && ...)
	{
		//		static_assert((std::is_same_v<Counts, int> && ...));
		return (counts && ...);
	}
#if 0
	template<int N, int vec&[N]>
	static bool anyNonZero( int vec[N])
	{
		for( int i=0; i < N; ++i)
		{
			if (array[i] != 0)
				return true;
		}
		return false;
	}
#endif
public:
	static bool has_links(const Patient& patient);
	static bool has_links(const Specialty& specialty);
	static bool has_links(const Doctor& doctor);
	static bool has_links(const Medication& med);
	static bool has_links(const Claim& claim);
	static bool has_links(const INSResponse& trans);
	static bool has_links(const Invoice& invoice);
	static bool has_links(const INSResponseLine& trans);

      static bool foreignKeysExist(const INSResponseLine &line);
        static bool foreignKeysExist(const Patient& patient);
	static bool foreignKeysExist(const Specialty& specialty);
	static bool foreignKeysExist(const Doctor& doctor);
	static bool foreignKeysExist(const Medication& med);
	static bool foreignKeysExist(const Claim& claim);
	static bool foreignKeysExist(const INSResponse& ins_response);
	static bool foreignKeysExist(const Invoice& invoice);

};
