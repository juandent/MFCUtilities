#pragma once
#include "DbContext.h"
#include "Data.h"
#include "DbSet.h"

class Context :
    public DbContext
{
public:
    DbSet<Claim, &Claim::id> claims;
};

