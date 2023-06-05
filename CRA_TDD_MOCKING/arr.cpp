#include <stdexcept>
#include <vector>
using namespace std;

class Arr
{
public:
	vector<int> run(vector<int> vArr)
	{
		vector<int> arr = vArr;
		for (int i = 0; i < vArr.size(); i++)
		{
			if (arr[i] >= 10 || arr[i] == 4) throw invalid_argument("invalid");
			arr[i]++;
			if (arr[i] == 10) arr[i] = 0;
			if (arr[i] == 4) arr[i] = 5;
		}
		return arr;
	}
};
