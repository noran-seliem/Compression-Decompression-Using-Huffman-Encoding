#include<iostream>
#include<queue>
#include <map> 
#include<vector>
#include <iterator> 
#include <cstdlib>
#include <cstdio>
#include <fstream>  
#include <cstdlib>
#include <cstring>
#include <string>
#include <stdlib.h> 
#define MAX_SIZE 1000
#define FALSE 0;
#define TRUE 1;

using namespace std;
int numberOfColumns, numberOfRows, numberOfBands, highVal, totalPixels, header;
unsigned char *image;
vector<unsigned char> v;

// Define a PGM class
class PGM {
public:
    //variables ... image headers
    int numberOfColumns, numberOfRows, numberOfBands, highVal, totalPixels, header, highHisto;
    unsigned char* image, * histogram, * outimageHistogram;
    vector<unsigned char> v;
    //functions
    void readImage();
    void frequancy(map<unsigned char, int>& freq_table);
    void writeFreqTable(map<unsigned char, int> freq_table);
};

void readImage(string inputFileName) {
	FILE* fpIn;
	char* string;
	int doneReading = FALSE;
	char c;

	fpIn = fopen(inputFileName.c_str(), "rb");
	if (fpIn == NULL) {
		cerr << "Such a file does not exist...";
		exit(EXIT_FAILURE);
	}
	string = (char*)calloc(256, 1);
	while (!doneReading && (c = (char)fgetc(fpIn)) != '\0')
		switch (c) {
		case 'P':
			c = (char)fgetc(fpIn);
			switch (c) {
			case '2':
				header = 2;
			    numberOfBands = 1;
				//pgm plain
				break;
			case '5':
				header = 5;
				numberOfBands = 1;
				//pgm Normal
				break;
			case '3':
				header = 3;
				numberOfBands = 3;
				//ppm plain
				break;
			case '6':
				header = 6;
				numberOfBands = 3;
				//ppm Normal
				break;
			}
			c = (char)fgetc(fpIn);
			if (c != 0x0A) {
				ungetc(c, fpIn);
			}
			else {
				ungetc(c, fpIn);
				fgets(string, 256, fpIn);
			}
			break;
		case '#':
			fgets(string, 256, fpIn);
			cout << "File you entered is " << string << "\n";
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			ungetc(c, fpIn);
			fscanf(fpIn, "%d %d %d", &(numberOfColumns), &(numberOfRows), &(highVal));
			doneReading = TRUE;
			fgetc(fpIn);
			break;
		}
	totalPixels = numberOfRows * numberOfColumns * numberOfBands;
	image = (unsigned char*)malloc(totalPixels);
	fread(image, 1, totalPixels, fpIn);
	int k = 0;
	for (int i = 0; i < numberOfRows; i++) {
		for (int j = 0; j < numberOfColumns; j++)
		{
			char c = (unsigned char)*(image + i * numberOfColumns + j);
			v.push_back(c);
			k++;
		}
	}
	fclose(fpIn);
	cout << "Reading the image " << inputFileName << " was sucessfull...\n";
}


void PGM::frequancy(map<unsigned char, int>& freq_table)
{
	cout << "Calculating frequency table..." << endl;
	for (int i = 0; i < this->v.size(); i++)
	{
		freq_table[v[i]]++;
	}
	cout << "Frequency table finished." << endl;
}

void PGM::writeFreqTable(map<unsigned char, int> freq_table) {
	FILE* fpOut;
	map<unsigned char, int>::iterator it;
	cout << "Writing frequency table ..." << endl;
	fpOut = fopen("frequencyTable.txt", "wb");
	for (it = freq_table.begin(); it != freq_table.end(); it++)
	{
		fprintf(fpOut, "%c %d\r\n", it->first, it->second);
	}
	cout << "Writing frequency table finished." << endl;
	fclose(fpOut);
}

map<unsigned char, int>  readFreqTable(string freqTableFileName) {
	ifstream inFile(freqTableFileName);
	unsigned char key;
	int freq;
	map<unsigned char, int> freq_table;

	while (inFile >> key >> freq)
		freq_table[key] = freq;

	inFile.close();
	return freq_table;
}

// Define HuffmanCode components

map<char, string> codemap;

class Tree_node {
public:
    unsigned char data;
    int freq;
    Tree_node* left;
    Tree_node* right;
    Tree_node(unsigned char Data, int frequency)
    {
        data = Data;
        freq = frequency;
        left = right = 0;
    }
};

Tree_node* root;

class Comparison {
public:
    bool operator() (Tree_node* l, Tree_node* r)
    {
        return l->freq > r->freq;
    }
};

Tree_node* build_Tree(priority_queue<Tree_node*, vector<Tree_node*>, Comparison> pq)
{
    while (pq.size() != 1)
    {
        Tree_node* left = pq.top();  pq.pop();
        Tree_node* right = pq.top(); pq.pop();
        Tree_node* node = new Tree_node('$', left->freq + right->freq);
        node->left = left;
        node->right = right;
        pq.push(node);
    }
    return pq.top();
}
void print_Code(Tree_node* root, int arr[], int top)
{  
    if (root->left)
    {
        arr[top] = 0;
        print_Code(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        print_Code(root->right, arr, top + 1);
    }
    if (!root->left && !root->right)
    {
        string str1 = "";
        cout << root->data << " ";
        for (int i = 0; i < top; i++) {
            cout << arr[i];
            char a = '0' + arr[i];
            str1 += a;
        }
        cout << endl;
        codemap.insert({ root->data, str1 });
    }
    
}

void Huffman_Code(map<unsigned char, int> freq_table)
{
    map<unsigned char, int>::iterator it;
    priority_queue<Tree_node*, vector<Tree_node*>, Comparison> pq;
    for (it = freq_table.begin(); it != freq_table.end(); it++)
    {
        Tree_node* newNode = new Tree_node(it->first, it->second);
        pq.push(newNode);
    }
    root = build_Tree(pq);
    int arr[MAX_SIZE], top = 0;
    print_Code(root, arr, top); 
   
}

string decimal_to_binary(int in)
{
    string temp = "";
    string result = "";
    while (in)
    {
        temp += ('0' + in % 2);
        in /= 2;
    }
    result.append(32 - temp.size(), '0');				//append '0' ahead to let the result become fixed length of 8
    for (int i = temp.size() - 1; i >= 0; i--)
    {
        result += temp[i];
    }
    return result;

}

void writeEncodedFile(string s) {
    ofstream outFile("image.enc", ofstream::binary);
    outFile << s;
    outFile.close();
}

void writeEncoding(string s) {
	FILE* fpOut;

	fpOut = fopen("image.enc", "wb");
	if (fpOut == NULL) {
		cerr << "Error couldn't write the image " << "...\n";
		exit;
	}

	int num = 8 - ((s.size()) % 8);
	fprintf(fpOut, "%c", num);
	//cout << endl << num << endl;

	if ((s.size()) % 8 == 0)
	{
		std::string w = "";

		for (int i = 0;i < s.size();i++)
		{
			if ((i + 1) % 8 == 0)

			{
				w += s[i];
				int b = std::stoi(w, 0, 2);

				char c = (char)b;
				//char W = static_cast<char>(a);
				w = "";
				//std::cout << c;
				fprintf(fpOut, "%c", c);
			}

			else
			{
				w += s[i];
			}
		}
	}

	else
	{
		int num = 8 - ((s.size()) % 8);
		for (int j = 0; j < num;j++)
		{
			s = s + "0";
		}
		{
			std::string w = "";

			for (int i = 0;i < s.size();i++)
			{
				if ((i + 1) % 8 == 0)

				{
					w += s[i];
					int b = std::stoi(w, 0, 2);

					char c = (char)b;
					//char W = static_cast<char>(a);
					w = "";
					//std::cout << c;
					fprintf(fpOut, "%c", c);
				}

				else
				{
					w += s[i];
				}
			}
		}
	}

	fclose(fpOut);
}

// this function reads the encoded file and saves it as an encoded string
string readEncodedFile(string encodedFileName) {
	FILE* fpIn;
	string encodedString="";

	fpIn = fopen(encodedFileName.c_str(), "rb");
	if (fpIn == NULL) {
		cerr << "Such a file does not exist...";
		exit(EXIT_FAILURE);
	}
	while (fgetc(fpIn) != '\0'){
	encodedString+= fgetc(fpIn);
	}	
    fclose(fpIn);
	return encodedString;
}

// function iterates through the encoded string s 
// if s[i]=='1' then move to node->right 
// if s[i]=='0' then move to node->left 
// if leaf node append the node->data to our output string 
string decode_file(Tree_node* root, string s)
{
    string ans = "";
    Tree_node* curr = root;
    for (int i = 0;i < s.size();i++)
    {
        if (s[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;

        // reached leaf node 
        if (curr->left == 0 and curr->right == 0)
        {
            ans += curr->data;
            curr = root;
        }
    }
    // cout<<ans<<endl; 
    return ans + '\0';
}
void writeDecodedFile(string s){
    ofstream outFile("image.dec", ofstream::binary);
    outFile << s;
    outFile.close();
}
int main(int argc, char** argv)
{
	int pause;
	
	map<unsigned char, int> freq_table;
    // if only one argument is provided (i.e. image.) then compress the image
	// if more than one argument is passed and the flag -d is used then the encoded image and the frequency table are used to decompress the image
    if (argc == 2) {
        PGM image;

        image.readImage(argv [1]);
        image.frequancy(freq_table);
        image.writeFreqTable(freq_table);
        Huffman_Code(freq_table);
        string encodedString;
        int r = image.numberOfRows, w = image.numberOfColumns;
        string codestr = decimal_to_binary(w) + decimal_to_binary(r) + "11111111";
        for (int i = 0; i < image.v.size(); i++) {
            encodedString += codemap[image.v[i]];
        }
        cout << encodedString.size() << endl;
        writeEncoding(encodedString);
        cout << "The encoded string is :" << encodedString.substr(0, 1000) << endl;

	}
	else{
		string encodedString;
		encodedString = readEncodedFile(argv[1]);
		freq_table = readFreqTable(argv[3]);
		Huffman_Code(freq_table);
		string decodedString = decode_file(root, encodedString);
		writeDecodedFile(decodedString);
	}

	cin >> pause;
	return 0;
}