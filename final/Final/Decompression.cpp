#include "PGM.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <stdlib.h> 
#include<queue>
#include <map> 
#include<vector>
#include <fstream>  
#include <cstdlib>
#define MAX_SIZE 1000
using namespace std;
#define FALSE 0;
#define TRUE 1;



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

void readFreqTable (std::map <int,int> &receivedfreq)
{
    std::ifstream rfile ("image.frq");//, std::ios::binary);
    if (!rfile)
    {
        std::cout<< "failed to open image.frq"<< std::endl;
        exit(1);
    }
    std::string key,value;
    while(rfile>>key>>value)
    {
        
        //rfile>> key >> value;        
    
        // std::cout<<key<< " "<< value<< std::endl;
        receivedfreq.insert(std::pair<int,int>(std::stoi(key),std::stoi(value)));
    }
    rfile.close();
}
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

int main()
{

    streampos size;
  char * memblock;

  ifstream file ("Image.enc", ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
    size = file.tellg();
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();

    cout << "the entire file content is in memory";

    delete[] memblock;
  }
  else cout << "Unable to open file";
  
	int pause;
	PGM image;
	map<unsigned char, int> freq_table;
	
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
        string decodedString = decode_file(root, encodedString);
    cout << "The decoded string is :" << decodedString.substr(0, 1000) << endl;
    writeDecodedFile(decodedString);
	cin >> pause;
	return 0;
}