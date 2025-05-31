#include <Account.h>
#include <Transaction.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
/* using ::testing::Return;
using ::testing::InSequence;
using ::testing::Throw; */

class MockAccount : public Account 
{
public:
	MockAccount(int id, int balance) : Account(id, balance) {}

	MOCK_METHOD(int, GetBalance, (), (const, override));
	MOCK_METHOD(void, ChangeBalance, (int), (override));
	MOCK_METHOD(void, Lock, (), (override));
	MOCK_METHOD(void, Unlock, (), (override));
};

/* class MockTransaction : public Transaction
{
public:
	MOCK_METHOD(void, SaveToDataBase, (Account&, Account&, int), (override));
}; */

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
    EXPECT_TRUE(from.GetBalance() == (700 - tr.fee()));
    EXPECT_TRUE(to.GetBalance() == 800);
}

TEST(TransactionTests, TransactionIfNotEnoughToDebit)
{
    Account from(42, 350);
    Account to(24, 500);
	Transaction tr;
    tr.set_fee(51);
    EXPECT_EQ(tr.fee(), 51);
	EXPECT_EQ(tr.Make(from, to, 300), false);
    EXPECT_TRUE(from.GetBalance() == 350);
    EXPECT_TRUE(to.GetBalance() == 500);
}

TEST(Transaction_test, test_Make_CheckCallAccountMethods)
{
    MockAccount from(42, 1000);
    MockAccount to(24, 500);
    Transaction tr;

    EXPECT_CALL(from, GetBalance()).Times(2);
    EXPECT_CALL(to, GetBalance()).Times(1);

    //EXPECT_CALL(from, ChangeBalance(_)).Times(1);
    //EXPECT_CALL(to, ChangeBalance(_)).Times(1);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    tr.Make(from, to, 300);
}

/* TEST(Transaction_test, test_Make_CheckCallSaveToDataBase)
{
    Account from(42, 1000);
    Account to(24, 500);
    MockTransaction tr;
    EXPECT_CALL(tr, SaveToDataBase()).Times(1);
    tr.Make(from, to, 300);
} */