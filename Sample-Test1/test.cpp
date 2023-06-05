#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../CRA_TDD_MOCKING/arr.cpp"
#include "../CRA_TDD_MOCKING/cal.cpp"
using namespace testing;
using namespace std;

/* 정리
 * 1) State Verification : EXPECT_EQ, THAT등으로 검증하는것
 *
 * 2) Behavior Verification : 호출횟수 / 순서대로 호출 여부 등 확인 가능
 * Cardinality (Behavior을 구현한 gMock의 기능이름)
 *
 * 3) Stubbing : a를 넣으면 b가 return되도록 세팅
 * Actions(Stub을 구현하기 위한 gMock의 기능이름)
 */

// State Verification : EXPECT_EQ, THAT등으로 검증하는것
TEST(TestCaseName, TestName) {
	// EXPECT_THAT는 Matcher (ex.Eq, Ne, AnyOf)를 쓸 수 있음
	EXPECT_THAT(1, Eq(1));
	EXPECT_THAT(1, Ne(2));
	EXPECT_THAT(1, AnyOf(1, 3, 5, 8, 9));
	EXPECT_THAT(10, Gt(2));
	EXPECT_THAT(10, Lt(14));

	double pi = 3.14;
	EXPECT_THAT(pi, DoubleNear(3.1415, 0.1)); // 절대오차 값 0.1 범위 내에 있는지 검사

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

// 상속을 통해 가짜 객체 생성
class MockCalculator : public Cal
{
public:
	// Mocking에 사용할 메서드 지정
	MOCK_METHOD(int, getSum, (int a, int b), ());
};

// Behavior Verification
// Cardinality (Behavior을 구현한 gMock의 기능이름)
TEST(CalTest, CalMock) {
	MockCalculator mock_cal;
	// getSum(1,2)가 2회 호출되어야만 한다. (test수행 전 미리 설정필요)
	EXPECT_CALL(mock_cal, getSum(1, 2)).Times(2);

	int ret1 = mock_cal.getSum(1, 2);
	int ret2 = mock_cal.getSum(1, 2);

	// 테스트가 끝나면 제대로 호출되었는지 검증한다.
}

TEST(CalTest, CalMock2) {
	MockCalculator mock_cal;
	EXPECT_CALL(mock_cal, getSum(_, _)).Times(2); // _, _는 와일드카드
	int ret = mock_cal.getSum(1, 2);
	ret = mock_cal.getSum(1, 2);

	MockCalculator mock_cal2;
	EXPECT_CALL(mock_cal2, getSum).Times(2); // 모든 인자가 와일드카드라면 함수이름만 적어도 됨
	ret = mock_cal2.getSum(1, 2);
	ret = mock_cal2.getSum(1, 2);

	MockCalculator mock_cal3;
	EXPECT_CALL(mock_cal3, getSum); // 1회만 호출
	ret = mock_cal3.getSum(1, 2);

	MockCalculator mock_cal4;
	EXPECT_CALL(mock_cal4, getSum(Ne(4), Eq(1))); // 4가 아닌수와 1을 받아서 1회만 호출
	ret = mock_cal4.getSum(10, 1);

	MockCalculator mock_cal5;
	EXPECT_CALL(mock_cal5, getSum)
		//.Times(0); // 호출되면 안됨
		//.Times(2); // 정확히 2번 호출
		//.Times(AtLeast(2)); // 2번 이상 호출되어야함
		//.Times(AtMost(2));  // 2번 이하 호출되어야함
		.Times(Between(2, 4)); // 2번 이상 4번 이하 호출되어야함

	ret = mock_cal5.getSum(10, 1);
	ret = mock_cal5.getSum(10, 1);

	MockCalculator mock_cal6;

	// 순서대로 호출되어야함
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
	cout << mock_cal.getSum(3, 5); // 리턴은 0
}

// Stubbing : a를 넣으면 b가 return되도록 세팅
// Actions(Stub을 구현하기 위한 gMock의 기능이름)
TEST(CalTest, CalMock4) {
	MockCalculator mock_cal;
	EXPECT_CALL(mock_cal, getSum(1, 2))
		.WillRepeatedly(Return(100)); // 1,2가 들어오면 계속 100이 리턴되어야함.

	EXPECT_CALL(mock_cal, getSum(5, 6))
		.WillOnce(Return(1)) // 5,6가 들어오면 한 번만 1이 리턴되어야함.
		.WillOnce(Return(2)) // 5,6가 들어오면 한 번만 2이 리턴되어야함.
		.WillRepeatedly(Return(100)); // 5,6가 들어오면 그 이후로 계속 100이 리턴되어야함.

	cout << mock_cal.getSum(1, 2) << endl;
	cout << mock_cal.getSum(1, 2) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
	cout << mock_cal.getSum(5, 6) << endl;
}

// Stubbing과 Behavior 동시 검증
TEST(CalTest, CalMock5) {
	MockCalculator mock_cal;
	EXPECT_CALL(mock_cal, getSum(5, 6))
		.Times(2) // Behavior검증 세팅부터 먼저 해야함.
		.WillRepeatedly(Return(100)); // 5,6가 들어오면 그 이후로 계속 100이 리턴되어야함.

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
	cout << mock_cal.getValue() << endl; // Stub(return값지정안함)이 되지않아서 경고 표시
}

TEST(CalTest, CalMock7)
{
	NiceMock<MockCalculator2> mock_cal;

	EXPECT_CALL(mock_cal, getSum(1, 2))
		.WillRepeatedly(Return(10));

	cout << mock_cal.getSum(1, 2) << endl;
	cout << mock_cal.getValue() << endl; // Stub이 안되어도 경고 안뜸
}

TEST(CalTest, CalMock8)
{
	StrictMock<MockCalculator2> mock_cal;

	EXPECT_CALL(mock_cal, getSum(1, 2))
		.WillRepeatedly(Return(10));

	cout << mock_cal.getSum(1, 2) << endl;
	// cout << mock_cal.getValue() << endl; // Stub이 안되면 Fail(경고)처리됨
}

class DBMock : public MyDataBaseAPI
{
public :
	MOCK_METHOD(string, getDBName, (), (override)); // 인터페이스 mocking할때는 override써줘야함
};

TEST(CalTest, CalMock9)
{
	DBMock db_mock; // mock을 써서 트래픽 발생시키지 않게함.
	EXPECT_CALL(db_mock, getDBName).WillRepeatedly(Return(string("MockDB")));

	LogSystem system(&db_mock); // 밖에서 DB모듈을 주입, 이 때 mock을 주입시킴
	cout << system.getLogMessage("KFC");

	EXPECT_EQ(system.getLogMessage("KFC"), string("[MockDB] KFC\n"));
	EXPECT_THAT(system.getLogMessage("KFC"), StrEq(string("[MockDB] KFC\n")));
}