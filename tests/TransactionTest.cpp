#include <Account.h>
#include <Transaction.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/* using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Throw; */

class MockTransaction : public Transaction
{
public:
	MOCK_METHOD(void, SaveToDataBase, (Account&, Account&, int), (override));
};

TEST(TransactionTests, SameAccounts)
{
    Account acc(42, 1000);
	Transaction tr;
	EXPECT_THROW(tr.Make(acc, acc, 200), std::logic_error);
}

TEST(TransactionTests, SumLessThanZero)
{
    Account from(42, 1000);
    Account to(24, 500);
	Transaction tr;
	EXPECT_THROW(tr.Make(from, to, -200), std::invalid_argument);
}

TEST(TransactionTests, SumLessThanOneHundred)
{
    Account from(42, 1000);
    Account to(24, 500);
	Transaction tr;
	EXPECT_THROW(tr.Make(from, to, 50), std::logic_error);
}

TEST(TransactionTests, TransactionIfEnoughToDebit)
{
    Account from(42, 1000);
    Account to(24, 500);
	Transaction tr;
	EXPECT_TRUE(tr.Make(from, to, 300));
    EXPECT_TRUE(from.GetBalance() == (700 - fee));
    EXPECT_TRUE(to.GetBalance() == 800);
}

TEST(TransactionTests, TransactionIfNotEnoughToDebit)
{
    Account from(42, 350);
    Account to(24, 500);
	Transaction tr;
    tr.set_fee(51);
	EXPECT_EQ(tr.Make(from, to, 300), false);
    EXPECT_TRUE(from.GetBalance() == 350);
    EXPECT_TRUE(to.GetBalance() == 500);
}