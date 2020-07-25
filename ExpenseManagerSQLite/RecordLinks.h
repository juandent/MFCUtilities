#pragma once


#include "Data_Tier.h"

class RecordLinks
{
public:
	static bool has_links(const Pais& pais);
	static bool has_links(const AccountOwner& owner);
	static bool has_links(const Concepto& concepto);

};


