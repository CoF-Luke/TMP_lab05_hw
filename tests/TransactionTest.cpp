/* #include <Account.h>
#include <Transaction.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;


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

TEST(Transaction_test, test_SaveToDataBase)
{
    Account from(42, 1000);
    Account to(24, 500);
    MockTransaction tr;
    EXPECT_CALL(tr, SaveToDataBase(_, _, _)).Times(1);
    tr.SaveToDataBase(from, to, 300);
} */




#include <gtest/gtest.h>
#include "banking/Account.h"
#include "banking/Transaction.h"

class TransactionTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TransactionTest, MakeThrowsIfSameAccount) {
    Account account(1, 1000);
    Transaction transaction;
    account.Unlock();
    EXPECT_THROW(transaction.Make(account, account, 300), std::logic_error);
}

TEST_F(TransactionTest, MakeThrowsIfNegativeSum) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    EXPECT_THROW(transaction.Make(from, to, -100), std::invalid_argument);
}

TEST_F(TransactionTest, MakeThrowsIfSumTooSmall) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    EXPECT_THROW(transaction.Make(from, to, 99), std::logic_error);
}

TEST_F(TransactionTest, MakeReturnsFalseIfFeeTooHigh) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction transaction;
    transaction.set_fee(60); // fee * 2 = 120 > 100
    from.Unlock();
    to.Unlock();
    EXPECT_FALSE(transaction.Make(from, to, 100));
}

TEST_F(TransactionTest, MakeSuccessWithMinimumSum) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    bool result = transaction.Make(from, to, 100);
    EXPECT_TRUE(result);
    EXPECT_EQ(from.GetBalance(), 899); // 1000 - (100 + 1 fee)
    EXPECT_EQ(to.GetBalance(), 600);   // 500 + 100
}

TEST_F(TransactionTest, MakeFailsWhenInsufficientFunds) {
    Account from(1, 100); // Not enough for 100 + fee (101)
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    bool result = transaction.Make(from, to, 100);
    EXPECT_FALSE(result);
    EXPECT_EQ(from.GetBalance(), 100); // Should remain unchanged
    EXPECT_EQ(to.GetBalance(), 500);   // Should remain unchanged
}

TEST_F(TransactionTest, FeeMethodsWork) {
    Transaction transaction;
    EXPECT_EQ(transaction.fee(), 1);
    transaction.set_fee(10);
    EXPECT_EQ(transaction.fee(), 10);
}

TEST_F(TransactionTest, BalancesUnchangedWhenTransactionFails) {
    Account from(1, 50); 
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    
    int from_initial = from.GetBalance();
    int to_initial = to.GetBalance();
    
    bool result = transaction.Make(from, to, 100);
    
    EXPECT_FALSE(result);
    EXPECT_EQ(from.GetBalance(), from_initial);
    EXPECT_EQ(to.GetBalance(), to_initial);
}