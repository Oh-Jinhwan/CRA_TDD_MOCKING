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

// LogSystem�� �����ϰ� �ִ� ��� (�� ����� Mock���� �����)
class MyDataBaseAPI : public DataBaseAPI
{
public :
	string getDBName() override
	{
		// �� �Լ��� ȣ���� �� ���� ���� Ʈ������ �߻���Ŵ
		return name;
	}
private:
	string name = "MyDB";
};

// �׽�Ʈ �ϰ��� �ϴ� ���
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