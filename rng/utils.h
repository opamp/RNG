#ifndef UTILS_HEADER
#define UTILS_HEADER
#include <vector>
#include <string>

using namespace std;

vector<string> split(const string& src, const char delim = ',') {
	vector<string> rtn;

	string sstr = "";
	for (int i = 0; i < src.length(); i++) {
		if (src[i] == delim) {
			rtn.push_back(sstr);
			sstr = "";
			continue;
		}
		else {
			sstr += src[i];
		}
	}

	if (sstr.length() != 0) {
		rtn.push_back(sstr);
	}
	return rtn;
}
#endif