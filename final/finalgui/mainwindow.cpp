#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QLabel>
#include<iostream>
#include<queue>
#include <map>
#include <cstdlib>
#include <cstdio>
#include<vector>
#include<string>
#include <fstream>
#include <bits/stdc++.h>
#include <queue>
#define MAX_SIZE 1000
#include <cstdlib>
#define FALSE 0;
#define TRUE 1;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    {
           QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.pgm )"));
           if (QString::compare(filename, QString()) !=0)
           {
               QImage image;
               bool valid = image.load(filename);
               if(valid)
               {
                   image = image.scaledToWidth(ui->label->width(), Qt::SmoothTransformation);

                   ui->label->setPixmap(QPixmap::fromImage(image));
               }
             else

                 {
                   //error handling
                }
          }
       }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.pgm )"));

    int numberOfColumns, numberOfRows, numberOfBands, highVal, totalPixels, header;
    unsigned char *image;
    vector<unsigned char> v;
    map<char, string> codemap;


    void readImage(){
        FILE * fpIn;
        char * string;
        int doneReading = FALSE;
        char c;

        fpIn = fopen("NORMAL2-IM-1427-0001.pgm", "rb");

        if (fpIn == NULL){
            cerr << "Such a file does not exist...";
            exit;
        }
        string = (char *)calloc(256, 1);
        while (!doneReading && (c = (char)fgetc(fpIn)) != '\0')
            switch (c){
            case 'P':
                c = (char)fgetc(fpIn);
                switch (c){
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
                if (c != 0x0A){
                    ungetc(c, fpIn);
                }
                else{
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
        image = (unsigned char *)malloc(totalPixels);
        fread(image, 1, totalPixels, fpIn);
        cout << "Reading the image " << " was sucessfull...\n";
        cout << "Wrote the image into " << "...\n";
        cout << totalPixels << endl;
        int k = 0;
        for (int i = 0; i < numberOfRows; i++) {
            for (int j = 0; j < numberOfColumns; j++)
            {
                char c = (unsigned char)*(image + i*numberOfColumns + j);
                v.push_back(c);
                k++;
                //printf("%c ", v[k]);
            }
        }

    fclose(fpIn);
    }

    void writeImage(){
        FILE * fpOut;

        fpOut = fopen("Image2.pgm", "wb");
        if (fpOut == NULL){
            cerr << "Error couldn't write the image " << "...\n";
            exit;
        }
        fprintf(fpOut, "P%d\n%d %d\n%d\n", header, numberOfColumns, numberOfRows, highVal);
        for (int i = 0; i<totalPixels; i++)
            fprintf(fpOut, "%c", v[i]);
        /*for (int i = 0; i <  numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++)
        {
        char c= (unsigned char) *(image + i*numberOfColumns + j);
        fprintf(fpOut, "%c", c);
        }
        }*/
        cout << "Wrote the image into " << "...\n";
        fclose(fpOut);
    }

    void writeEncoding(string s){
        FILE * fpOut;

        fpOut = fopen("image.enc", "wb");
        if (fpOut == NULL){
            cerr << "Error couldn't write the image " << "...\n";
            exit;
        }

         int num= 8-((s.size())%8);
        fprintf(fpOut, "%c", num);
        cout<<endl<<num<<endl;
        /*string w="";
        for(int i=0;i <s.size();i++)
        {
            if((s.size())%8==0)
            {
                w+=s[i] ;
                int a= std::stoi(w,0,2);
                char c = (char)a;
                 w="";
                std::cout <<c;
                fprintf(fpOut, "%c", c);
            }
            else
                w+=s[i] ;
        }
        if(w!="")
        {
            int num= 8-((s.size())%8);
            for(int j=0; j<num;j++)
            { s=s+"0"; }
            char c = (char)std::stoi(w,0,2);
            std::cout <<c;
            fprintf(fpOut, "%c", c);

        }*/


        if((s.size())%8==0)
          {
              std::string w="";

         for(int i=0;i<s.size();i++)
        {
            if((i+1)%8==0)

          {     w+=s[i] ;
           int b= std::stoi(w,0,2);

            char c = (char)b;
            //char W = static_cast<char>(a);
             w="";
             std::cout <<c;
             fprintf(fpOut, "%c", c);
          }

           else
          {  w+=s[i];}
        }
          }

          else
          {
              int num= 8-((s.size())%8);
              for(int j=0; j<num;j++)
              { s=s+"0"; }
              {
              std::string w="";

         for(int i=0;i<s.size();i++)
        {
            if((i+1)%8==0)

          {     w+=s[i] ;
           int b= std::stoi(w,0,2);

            char c = (char)b;
            //char W = static_cast<char>(a);
             w="";
             std::cout <<c;
             fprintf(fpOut, "%c", c);
          }

           else
          {  w+=s[i];}
        }
          }
      }

    fclose(fpOut);
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
        result.append(32 - temp.size(), '0');													//append '0' ahead to let the result become fixed length of 8
        for (int i = temp.size() - 1; i >= 0; i--)
        {
            result += temp[i];
        }
        return result;

    }

    void frequancy(map<unsigned char, int> &freq_table)
    {
        for (int i = 0; i <v.size(); i++)
        {
            freq_table[v[i]]++;
        }
        FILE * fpOut;

        fpOut = fopen("image.frq","wb");
        if (fpOut == NULL){
            cerr << "Error couldn't write the image " << "...\n";
            exit;
        }
        map<unsigned char, int>::iterator it;
        for (it = freq_table.begin(); it != freq_table.end(); it++)
        {
             cout<<it->first<<" "<<it->second<<endl;
             fprintf(fpOut, "%c %d\n", it->first, it->second);
        }
        cout<<"hear";
        fclose(fpOut);


    }


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
    class Comparison {
    public:
        bool operator() (Tree_node* a, Tree_node* b)
        {
            return a->freq > b->freq;
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
        Tree_node* root = build_Tree(pq);
        int arr[MAX_SIZE], top = 0;
        print_Code(root, arr, top);
    }

    int main()
    {
        readImage();
        writeImage();
        map<unsigned char, int> freq_table;
        map<int, unsigned char> freq_table2;
        frequancy(freq_table);
        /*map<unsigned char, int>::iterator it;
        FILE * fpOut;

        fpOut = fopen("xx.pgm", "wb");
        if(fpOut == NULL){
        cerr<<"Error couldn't write the image "<<"...\n";
        exit;
        }
        for (it = freq_table.begin(); it != freq_table.end(); it++)
        {
        fprintf(fpOut, "%c --> %d\n", it->first, it->second);
        } */

       int r = numberOfRows,w=numberOfColumns;
        Huffman_Code(freq_table);
        string codestr = decimal_to_binary(w) + decimal_to_binary(r) + "11111111";
        for (int i = 0; i < v.size(); i++){
            codestr += codemap[v[i]];
        }
        cout<<codestr;

        writeEncoding(codestr);
           //codestr
        delete image;
        return 0;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.pgm )"));
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
}
