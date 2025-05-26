#include <Account.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockAccount : public Account 
{
public:
	MockAccount(int id, int balance) : Account(id, balance) {}

	MOCK_METHOD(int, GetBalance, (), (const, override));
	MOCK_METHOD(void, ChangeBalance, (int), (override));
	MOCK_METHOD(void, Lock, (), (override));
	MOCK_METHOD(void, Unlock, (), (override));
};

TEST(AccountTests, GetBalance)
{
    Account acc(24, 42);
    EXPECT_EQ(acc.GetBalance(), 42);
}

TEST(AccountTests, ChangeBalanceIfLocked)
{
    Account acc(24, 42);
    acc.Lock();
    acc.ChangeBalance(958);
    EXPECT_EQ(acc.GetBalance(), 1000);
}

TEST(AccountTests, ChangeBalanceIfLockedWithNegativeDiff)
{
    Account acc(24, 42);
    acc.Lock();
    acc.ChangeBalance(-1000);
    EXPECT_EQ(acc.GetBalance(), -958);
}

TEST(AccountTests, ChangeBalanceIfUnlocked)
{
    Account acc(24, 42);
    EXPECT_THROW(acc.ChangeBalance(-1000), std::runtime_error);
}

TEST(AccountTests, LockTwice)
{
    Account acc(24, 42);
    account.Lock();
	EXPECT_THROW(acc.Lock(), std::runtime_error);
}

TEST(AccountTests, UnlockTwice)
{
    Account acc(24, 42);
	EXPECT_THROW(acc.Unlock(), std::runtime_error);
}

TEST(AccountTests, LockUnlock)
{
    Account acc(24, 42);
    acc.Lock();
	EXPECT_NO_THROW(acc.Unlock());
}

