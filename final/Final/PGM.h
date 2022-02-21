#pragma once
#include<vector>
#include <map> 
#include <iterator> 
using namespace std;

class PGM {
public:
	//variables ... image headers
	int numberOfColumns, numberOfRows, numberOfBands, highVal, totalPixels, header, highHisto;
	unsigned char* image;
	vector<unsigned char> v;
	//functions
	void readImage();
	void writeImage();
	void frequancy(map<unsigned char, int>& freq_table);
	void writeFreqTable(map<unsigned char, int> freq_table);
	
};


