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

TESTS(TransactionTests, SameAccounts)
{
    Account acc(42, 1000);
	Transaction tr;
	EXPECT_THROW(tr.Make(acc, acc, 200), std::logic_error);
}

TESTS(TransactionTests, SumLessThanZero)
{
    Account from(42, 1000);
    Account to(24, 500);
	Transaction tr;
	EXPECT_THROW(tr.Make(from, to, -200), std::invalid_argument);
}

TESTS(TransactionTests, SumLessThanOneHundred)
{
    Account from(42, 1000);
    Account to(24, 500);
	Transaction tr;
	EXPECT_THROW(tr.Make(from, to, 50), std::logic_error);
}

