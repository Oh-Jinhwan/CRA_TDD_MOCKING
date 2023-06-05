#include <string>
using namespace std;

class Cal
{
public :
	int getSum(int a, int b) {
		return a + b;
	}

	int getValue() {
		return x;
	}
private:
	int x = 100;
};

#define interface struct
interface DataBaseAPI
{
public:
	virtual string getDBName() = 0;
};

// LogSystem이 의존하고 있는 모듈 (이 모듈을 Mock으로 만든다)
class MyDataBaseAPI : public DataBaseAPI
{
public :
	string getDBName() override
	{
		// 이 함수를 호출할 때 마다 서버 트래픽을 발생시킴
		return name;
	}
private:
	string name = "MyDB";
};

// 테스트 하고자 하는 모듈
class LogSystem
{
public :
	LogSystem(DataBaseAPI *db) : DB(db) {}
	string getLogMessage(string content)
	{
		string msg = "";
		msg += string("[") + DB->getDBName() + string("] ");
		msg += content + string("\n");
		return msg;
	}
private:
	DataBaseAPI *DB;
};