// https://github.com/kobake/leveldb-vc140-nuget
// Add this to linker addition definition leveldb.lib;shlwapi.lib; !!!!
// Add this to command com parameters of compiler "/ D_SCL_SECURE_NO_WARNINGS"
// Add this / D_CRT_SECURE_NO_WARNINGS


#include "stdafx.h"

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <iostream>
#include <clocale>
#include <locale>
#include <codecvt>

#include "leveldb/db.h"
#include "leveldb/iterator.h"
#include "leveldb/options.h"

//#include "utf8.h"
//#include <vector>
//
//#include <list>

using namespace std;


void savetofile(string name, string value) {
	std::ofstream file(name);
	std::string my_string = value;
	file << my_string;
	file.close();
}

void savetofilew(string name, wstring value) {
	std::wofstream file(name);
	std::wstring my_string = value;
	file << my_string;
	file.close();
}


extern "C"
{

	leveldb::DB* db;
	std::string v;
	unsigned char* buffer = NULL;

	__declspec(dllexport) bool DbOpen(char* path) {
		cout << "DbOpen path: " << path << endl;
		leveldb::Options options;
		options.create_if_missing = false;
		leveldb::Status status = leveldb::DB::Open(options, path, &db);
		if (status.ok()) {
			cout << "DbOpen Ok!" << endl;
			return true;
		}
		else {
			cout << "DbOpen Error!" << endl;
			return false;
		}
	}



	__declspec(dllexport) bool DbKeyOpen(char* key) {
		cout << "DbKeyOpen " << key << endl;
		std::string key_ = key;
		key_[52] = '\x0';
		key_[53] = '\x1';
		leveldb::Slice k = key_;

		leveldb::Status status = db->Get(leveldb::ReadOptions(), k, &v);
		if (status.ok()) {
			cout << "DbKeyOpen Ok!" << endl;
			//v.erase(0, 1); // Delete artifact. not forget about this when store back to DB !!!!!
			return true;
		}
		else {
			cout << "DbKeyOpen Error!" << endl;
			return false;
		}
	}

	
	__declspec(dllexport) const unsigned char* DbGet(size_t *bufferLen)
	{

		// we found the key, so set the buffer length 
		*bufferLen = v.size();

		// initialize the buffer
		buffer = new unsigned char[*bufferLen];

		// set the buffer
		memset(buffer, 0, *bufferLen);

		// copy the data
		memcpy((void*)(buffer), v.c_str(), *bufferLen);

		cout << "===>> " << *bufferLen  << endl;
		
		return buffer;
	}


	__declspec(dllexport) bool DbSaveBinary(char* s) {
		cout << "DbSaveBinary Ok!" << endl;
		savetofile(s, v);
		return true;
	}


	__declspec(dllexport) int DbKeyClose() {
		cout << "DbKeyClose Ok!" << endl;
		delete[] buffer;
		return 0;
	}


	__declspec(dllexport) int DbClose(int i) {
		cout << "DbClose Ok!" << endl;
		delete db;
		return 0;
	}



	



}



