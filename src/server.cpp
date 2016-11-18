#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"

#include <bits/stdc++.h>
using namespace std;

const int MAXBUF = 20;
static int cntid = 0;

string itos(int num) {
	string ret;
	if (num == 0) {
		return string("0");
	}
	while (num) {
		ret.push_back(num % 10 + '0');
		num /= 10;
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

string blessId() {
	string ret = itos(cntid++);
	return ret;
}

void instantStream(string id, int period) {
//	printf("instant event %s in period %d is running\n", id.c_str(), period);
	while (1) {
		printf("instant event %s in period %d is occurred\n", id.c_str(), period);
		this_thread::sleep_for(chrono::milliseconds(period));
	}
}

void persistentStream(string id, int period, int length) {
//	printf("persistent event %s in period %d and length %d is running\n", id.c_str(), period, length);
	while (1) {
		printf("persistent event %s in period %d and length %d is occurred\n", id.c_str(), period, length);
		this_thread::sleep_for(chrono::milliseconds(period));
	}
}

void streamServer(string &file) {
	char buf[MAXBUF] = {0};
	vector<int> instant;
	vector<pair<int, int>> persistent;
	FILE *fin = fopen(file.c_str(), "r");
	while (fgets(buf, sizeof(buf), fin) != NULL) {
		int a, b;
		// need a assert() function?
		if (buf[0] == '0') {
			sscanf(buf, "%*d%d", &a);
//			printf("instant event's conf: %d\n", a);
			instant.push_back(a);
		} else {
			sscanf(buf, "%*d%d%d", &a, &b);
//			printf("persistent event's conf: %d %d\n", a, b);
			persistent.push_back(make_pair(a, b));
		}
		memset(buf, 0, sizeof(buf));
	}
	fclose(fin);
	///
	vector<thread> iths;
	vector<thread> pths;
	for (int i = 0; i < (int)instant.size(); ++i) {
		iths.push_back(thread(instantStream, blessId(), instant[i]));
	}
	for (int i = 0; i < (int)persistent.size(); ++i) {
		pths.push_back(thread(persistentStream, blessId(), persistent[i].first, persistent[i].second));
	}
	//
	for (int i = 0; i < (int)iths.size(); ++i) {
		iths[i].join();
	}
	for (int i = 0; i < (int)pths.size(); ++i) {
		pths[i].join();
	}
}

void generateStream(vector<string> &files) {
	vector<thread> ths;
	for (int i = 0; i < (int)files.size(); ++i) {
		ths.push_back(thread(streamServer, ref(files[i])));
	}

	for (int i = 0; i < (int)ths.size(); ++i) {
		ths[i].join();
	}
}

void listenConnection() {
}

int main(int argc, char **argv) {
	vector<string> files;
	for (int i = 1; i < argc; ++i) {
		files.push_back(string(argv[i]));
	}

	thread th1(generateStream, ref(files));
	thread th2(listenConnection);
	th1.join();
	th2.join();
	return 0;

}
