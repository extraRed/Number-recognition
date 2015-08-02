#include<cstdio>
#include<iostream>
#include<Windows.h>
#include<cstdlib>
#include<cmath>
#include<bitset>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<string>
#include<time.h>
#include<queue>
using namespace std;
struct Node
{
	int num;
	bitset<1024> x;
	int dis;
};
class MyKNN
{
public:
	MyKNN();
	void readin(string BMP_filename);
	void changeto10();
	void remove_ilegalpoint();
	void changetobit();
	void printout();
	float testcode();
	int readFile();
	int get_dis(const Node &p,const Node &q);
	//int Cmp(const Node &p,const Node &q);
	void build_tree(Node node[],int lef,int righ,int id,int depth);
	void search_tree(Node n,int id,int depth);
	int kdd(bitset<1024> given,int showinfo);
	int knn_code();
	void classify();
	int trainsamples;
};
