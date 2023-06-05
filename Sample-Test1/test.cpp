#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../CRA_TDD_MOCKING/arr.cpp"
#include "../CRA_TDD_MOCKING/cal.cpp"
using namespace testing;
using namespace std;

/* ����
 * 1) State Verification : EXPECT_EQ, THAT������ �����ϴ°�
 *
 * 2) Behavior Verification : ȣ��Ƚ�� / ������� ȣ�� ���� �� Ȯ�� ����
 * Cardinality (Behavior�� ������ gMock�� ����̸�)
 *
 * 3) Stubbing : a�� ������ b�� return�ǵ��� ����
 * Actions(Stub�� �����ϱ� ���� gMock�� ����̸�)
 */

// State Verification : EXPECT_EQ, THAT������ �����ϴ°�
TEST(TestCaseName, TestName) {
	// EXPECT_THAT�� Matcher (ex.Eq, Ne, AnyOf)�� �� �� ����
	EXPECT_THAT(1, Eq(1));
	EXPECT_THAT(1, Ne(2));
	EXPECT_THAT(1, AnyOf(1, 3, 5, 8, 9));
	EXPECT_THAT(10, Gt(2));
	EXPECT_THAT(10, Lt(14));

	double pi = 3.14;
	EXPECT_THAT(pi, DoubleNear(3.1415, 0.1)); // ������� �� 0.1 ���� ���� �ִ��� �˻�

	int* p1 = nullptr;
	int* p2 = new int();
	EXPECT_THAT(p1, IsNull());
	EXPECT_THAT(p2, NotNull());

	int x = 10;
	int& p = x;
	EXPECT_THAT(p, Ref(x));

	vector<int> arr = { 1,3,5,7,9 };
	EXPECT_THAT(arr, Contains(3));

	vector<int> arr1 = { 1,3,5,7,9 };
	vector<int> arr2 = { 1,3,5,7,9 };
	EXPECT_EQ(arr1, arr2);
	EXPECT_THAT(arr, ContainerEq(arr2));

	vector<int> emptyArr;
	EXPECT_THAT(emptyArr, IsEmpty());

	string str = "LORD OF KFC";
	EXPECT_THAT(str, StartsWith("LORD"));
	EXPECT_THAT(str, EndsWith("KFC"));

	string str1 = "ABCD";
	string str2 = "ABCD";
	EXPECT_THAT(str1, StrEq(str2));

	string str3 = "aB13z";
	EXPECT_THAT(str3, MatchesRegex("a.*z"));

	// EXPECT_THAT(1, Eq(2)) << "Matcher Massge";
}

TEST(TestCaseName, CalTest) {
	vector<int> inputArr = { 1,2,3,2 };
	vector<int> outputArr = { 2,3,5,3 };
	EXPECT_THAT(Arr().run(inputArr), ContainerEq(outputArr));

	vector<int> inputArr2 = { 9,9,3,3 };
	vector<int> outputArr2 = { 0,0,5,5 };
	EXPECT_THAT(Arr().run(inputArr2), ContainerEq(outputArr2));

	vector<int> inputArr3 = { 10,4,4,10 };
	EXPECT_THROW(Arr().run(inputArr3), invalid_argument);
}

// ����� ���� ��¥ ��ü ����
class MockCalculator : public Cal
{
public:
	// Mocking�� ����� �޼��� ����
	MOCK_METHOD(int, getSum, (int a, int b), ());
};

// Behavior Verification
// Cardinality (Behavior�� ������ gMock�� ����̸�)
TEST(CalTest, CalMock) {
	MockCalculator mock_cal;
	// getSum(1,2)�� 2ȸ ȣ��Ǿ�߸� �Ѵ�. (test���� �� �̸� �����ʿ�)
	EXPECT_CALL(mock_cal, getSum(1, 2)).Times(2);

	int ret1 = mock_cal.getSum(1, 2);
	int ret2 = mock_cal.getSum(1, 2);

	// �׽�Ʈ�� ������ ����� ȣ��Ǿ����� �����Ѵ�.
}

TEST(CalTest, CalMock2) {
	MockCalculator mock_cal;
	EXPECT_CALL(mock_cal, getSum(_, _)).Times(2); // _, _�� ���ϵ�ī��
	int ret = mock_cal.getSum(1, 2);
	ret = mock_cal.getSum(1, 2);

	MockCalculator mock_cal2;
	EXPECT_CALL(mock_cal2, getSum).Times(2); // ��� ���ڰ� ���ϵ�ī���� �Լ��̸��� ��� ��
	ret = mock_cal2.getSum(1, 2);
	ret = mock_cal2.getSum(1, 2);

	MockCalculator mock_cal3;
	EXPECT_CALL(mock_cal3, getSum); // 1ȸ�� ȣ��
	ret = mock_cal3.getSum(1, 2);

	MockCalculator mock_cal4;
	EXPECT_CALL(mock_cal4, getSum(Ne(4), Eq(1))); // 4�� �ƴѼ��� 1�� �޾Ƽ� 1ȸ�� ȣ��
	ret = mock_cal4.getSum(10, 1);

	MockCalculator mock_cal5;
	EXPECT_CALL(mock_cal5, getSum)
		//.Times(0); // ȣ��Ǹ� �ȵ�
		//.Times(2); // ��Ȯ�� 2�� ȣ��
		//.Times(AtLeast(2)); // 2�� �̻� ȣ��Ǿ����
		//.Times(AtMost(2));  // 2�� ���� ȣ��Ǿ����
		.Times(Between(2, 4)); // 2�� �̻� 4�� ���� ȣ��Ǿ����

	ret = mock_cal5.getSum(10, 1);
	ret = mock_cal5.getSum(10, 1);

	MockCalculator mock_cal6;

	// ������� ȣ��Ǿ����
	{
		InSequence Seq;
		EXPECT_CALL(mock_cal6, getSum(1, 1));
		EXPECT_CALL(mock_cal6, getSum(1, 2));
		EXPECT_CALL(mock_cal6, getSum(1, 3));
	}

	ret = mock_cal6.getSum(1, 1);
	ret = mock_cal6.getSum(1, 2);
	ret = mock_cal6.getSum(1, 3);
}

TEST(CalTest, CalMock3) {
	MockCalculator mock_cal;
	cout << mock_cal.getSum(3, 5); // ������ 0
}

// Stubbing : a�� ������ b�� return�ǵ��� ����
// Actions(Stub�� �����ϱ� ���� gMock�� ����̸�)
TEST(CalTest, CalMock4) {
	MockCalculator mock_cal;
	EXPECT_CALL(mock_cal, getSum(1, 2))
		.WillRepeatedly(Return(100)); // 1,2�� ������ ��� 100�� ���ϵǾ����.

	EXPECT_CALL(mock_cal, getSum(5, 6))
		.WillOnce(Return(1)) // 5,6�� ������ �� ���� 1�� ���ϵǾ����.
		.WillOnce(Return(2)) // 5,6�� ������ �� ���� 2�� ���ϵǾ����.
		.WillRepeatedly(Return(100)); // 5,6�� ������ �� ���ķ� ��� 100�� ���ϵǾ����.

	cout << mock_cal.getSum(1, 2) << endl;
	cout << mock_cal.getSum(1, 2) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
}

// Stubbing�� Behavior ���� ����
TEST(CalTest, CalMock5) {
	MockCalculator mock_cal;
	EXPECT_CALL(mock_cal, getSum(5, 6))
		.Times(2) // Behavior���� ���ú��� ���� �ؾ���.
		.WillRepeatedly(Return(100)); // 5,6�� ������ �� ���ķ� ��� 100�� ���ϵǾ����.

	cout << mock_cal.getSum(5, 6) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
}

class MockString : public string
{
public :
	MOCK_METHOD(int, length, (), ());
};

TEST(MockTest, StringMock)
{
	MockString mock_str;

	EXPECT_CALL(mock_str, length())
		.WillRepeatedly(Return(1));

	cout << mock_str.length() << endl;
	cout << mock_str.length() << endl;
}

class MockVector : public vector<int>
{
public:
	MOCK_METHOD(int, size, (), ());
};
TEST(MockTest, VectorMock)
{
	MockVector mock_vector;

	EXPECT_CALL(mock_vector, size())
		.Times(2)
		.WillRepeatedly(Return(10000));

	EXPECT_THAT(mock_vector.size(), 10000);
	EXPECT_THAT(mock_vector.size(), 10000);
}

class MockCalculator2 : public Cal
{
public :
	MOCK_METHOD(int, getSum, (int a, int b), ());
	MOCK_METHOD(int, getValue, (), ());
};

TEST(CalTest, CalMock6)
{
	MockCalculator2 mock_cal;

	EXPECT_CALL(mock_cal, getSum(1, 2))
		.WillRepeatedly(Return(10));

	cout << mock_cal.getSum(1, 2) << endl;
	cout << mock_cal.getValue() << endl; // Stub(return����������)�� �����ʾƼ� ��� ǥ��
}

TEST(CalTest, CalMock7)
{
	NiceMock<MockCalculator2> mock_cal;

	EXPECT_CALL(mock_cal, getSum(1, 2))
		.WillRepeatedly(Return(10));

	cout << mock_cal.getSum(1, 2) << endl;
	cout << mock_cal.getValue() << endl; // Stub�� �ȵǾ ��� �ȶ�
}

TEST(CalTest, CalMock8)
{
	StrictMock<MockCalculator2> mock_cal;

	EXPECT_CALL(mock_cal, getSum(1, 2))
		.WillRepeatedly(Return(10));

	cout << mock_cal.getSum(1, 2) << endl;
	// cout << mock_cal.getValue() << endl; // Stub�� �ȵǸ� Fail(���)ó����
}

class DBMock : public MyDataBaseAPI
{
public :
	MOCK_METHOD(string, getDBName, (), (override)); // �������̽� mocking�Ҷ��� override�������
};

TEST(CalTest, CalMock9)
{
	DBMock db_mock; // mock�� �Ἥ Ʈ���� �߻���Ű�� �ʰ���.
	EXPECT_CALL(db_mock, getDBName).WillRepeatedly(Return(string("MockDB")));

	LogSystem system(&db_mock); // �ۿ��� DB����� ����, �� �� mock�� ���Խ�Ŵ
	cout << system.getLogMessage("KFC");

	EXPECT_EQ(system.getLogMessage("KFC"), string("[MockDB] KFC\n"));
	EXPECT_THAT(system.getLogMessage("KFC"), StrEq(string("[MockDB] KFC\n")));
}