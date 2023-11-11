#include "pch.h"

#include "Relationships.h"
#include "gtest/gtest.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);

  Claim cl = storage.get<Claim>(1);

  auto res = get_Card_1<Doctor, int, &Doctor::id, Claim, &Claim::fkey_doctor>(cl);
	
}