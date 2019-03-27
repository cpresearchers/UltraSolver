#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
namespace cudacp {
class XVar {
	int id;
	int uid;
	string name;
	vector<int> vals;
	unordered_map<int, int> val_map;
	vector<int> anti_map;
	const int std_min = 0;
	const int std_max;

	XVar(const int id, const int uid, const string name, const int min_val, const int max_val);

	XVar(const int id, const int uid, const string name, vector<int> &v);

	~XVar();

	void show();
};


class XTab {

};
class XModel {
public:
	vector<XVar*> X;
	vector<XTab*> C;

};
}