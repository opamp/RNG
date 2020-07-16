#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <list>
#include <tuple>
#include <algorithm>
#include "utils.h"

using namespace std;

const string version_string = "1.4";

tuple<string, string> parse_mode(const char* mode_char_str) {
	string mode_str = mode_char_str;
	string mode_name = "";
	string mode_value = "";
	int tmp = 0;
	for (int i = 0; i < (int)mode_str.size(); i++) {
		char c = mode_str[i];
		if (c == '=') {
			tmp = 1;
			continue;
		}

		if (tmp == 0) {
			mode_name += c;
		}
		else {
			mode_value += c;
		}
	}
	return forward_as_tuple(mode_name, mode_value);
}

int text_to_bin(vector<string> val_arr, char* ret) {
	for (int i = 0; i < val_arr.size(); i++) {
		try {
			int b = stoi(val_arr[i], nullptr, 16);
			ret[i] = b;
		}
		catch (const std::invalid_argument& ex) {
			return 1;
		}
	}
	return 0;
}

bool is_binarymode(string mode) {
	if (mode == "binary" || mode == "bin") {
		return true;
	}
	else {
		return false;
	}
}


bool is_textmode(string mode) {
	if (mode == "text" || mode == "txt") {
		return true;
	}
	else {
		return false;
	}
}

bool is_textnlmode(string mode) {
	if (mode == "textnl" || mode == "txtnl") {
		return true;
	}
	else {
		return false;
	}
}

bool is_randomtextmode(string mode) {
	if (mode == "randomtext" || mode == "randtxt") {
		return true;
	}
	else {
		return false;
	}
}

bool is_randommode(string mode) {
	if (mode == "random" || mode == "rand") {
		return true;
	}
	else {
		return false;
	}
}

bool is_fastrandommode(string mode) {
	if (mode == "fastrandom" || mode == "fastrand") {
		return true;
	}
	else {
		return false;
	}
}

bool valid_mode(string mode) {
	return is_binarymode(mode) || is_textmode(mode) || is_textnlmode(mode) || is_randomtextmode(mode) || is_randommode(mode) || is_fastrandommode(mode);
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		cout << "rng - random number generator, VERSION: " << version_string << endl << endl;
		cout << "Usage:" << argv[0] << " random/randtxt/fastrand=seed/text=TEXT/textnl=TEXT/binary=1,2,3 count output" << endl;
		cout << "Example:" << endl;
		cout << "> rng text=hello 2 textfile.txt" << endl;
		cout << "> rng random 10 randombinary.bin" << endl;
		cout << "> rng binary=0,1,2 100 bigbinary.bin" << endl;
		return 0;
	}

	/*Parse mode_name and mode_value*/
	string mode_name;
	string mode_value;
	tie(mode_name, mode_value) = parse_mode(argv[1]);
	if (!valid_mode(mode_name)) {
		cerr << "Invalid mode" << endl;
		return 1;
	}

	/* parse binary data */
	char *bindata = NULL;
	int bindata_size;
	if (is_binarymode(mode_name)) {
		vector<string> val_arr = split(mode_value);
		bindata_size = val_arr.size();
		bindata = (char*)malloc(sizeof(char)*bindata_size);
		int f = text_to_bin(val_arr, bindata);
		if (f != 0) {
			cerr << "Failed to convert binary" << endl;
			return 1;
		}
	}

	/* parse count */
	string count_string = argv[2];
	long long count = 0;
	try {
		count = stoll(count_string);
	}
	catch (const std::invalid_argument& ex) {
		cerr << "Invalied count number -> " << count_string << endl;
		return 1;
	}
	if (count < 0) {
		cerr << "Count number < 0" << endl;
		return 1;
	}

	/* output part */
	ofstream fout;
	if (is_binarymode(mode_name) || is_randommode(mode_name) || is_fastrandommode(mode_name)) {
		fout.open(argv[3], ios::out | ios::binary | ios::app);
	}
	else {
		fout.open(argv[3], ios::out | ios::app);
	}
	if (fout.fail()) {
		cerr << "Failed to open file" << endl;
		return 1;
	}

	// random device
	random_device rd;
	mt19937 mt;

	if (is_fastrandommode(mode_name)) {
		int seed = stoi(mode_value);
		mt.seed(seed);
	}

	// output loop
	for (int i = 0; i < count; i++) {
		if (is_textmode(mode_name)) {
			fout << mode_value;
		}
		else if (is_textnlmode(mode_name)) {
			fout << mode_value << endl;
		}
		else if (is_binarymode(mode_name)) {
			fout.write(bindata, bindata_size);
		}
		else if (is_randomtextmode(mode_name)) {
			const char alphanum[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"!@#$%^&*()_+|~`=-][{}';': /><\\,.\n";
			fout << alphanum[rd() % (sizeof(alphanum) - 1)];
		}
		else if (is_fastrandommode(mode_name)) {
			uint_fast32_t ur = mt();
			char r = ur;
			fout.write(&r,sizeof(char));
		}
		else {
			char r = rd();
			fout.write(&r, sizeof(char));
		}
	}
	fout.close();
	return 0;
}
