#include "bitmatrix.h"
#include <fstream>
#include <chrono>
//#include <vector>
//#include "boost/dynamic_bitset/dynamic_bitset.hpp"
//#include <iostream>
using namespace std;
int main()
{
	//TESTING GRAY CODE VS MST ALGORITHM
	//WILL TEST VARYING ROWS:COLUMN RATIO, 
	//sparsity, number of elements.
    //and partition size.
	//default values (that we keep constant):
	//ROWS/COLUMNS==1
	//NUMBER OF ELEMENTS = 4096*4096 = 16777216
	//sparsity 50%
	//partition size 8
	ofstream file1;
	int totalElements = 262144;
	int rows=512;
	int columns=512;
	int sparsity = 1;
	int partitionsize=8;
	int rowsvscolumns=1;
	
	//VARY ROWS:COLUMN RATIO
	file1.open("RatioTest4.txt");
	file1<<"rows 	cols 	rows/columns 	columns/rows 	RegularOp	MSTpartOp	GrayPartOp\n";	
	for(int columns=8; columns<=8192; columns*=2){
		int totalElements=256*256;
		if(columns%partitionsize!=0) continue;
		int rows = totalElements/columns;
		cout<<"rows: "<<rows<<" columns: "<<columns<<" rows*columns="<<rows*columns<<endl;
		Bitmatrix mat(rows, columns);
		cout<<"make vect\n";
		Doublematrix vect(columns,1);
		cout<<"make result vects\n";
		Doublematrix resultReg(rows,1),resultGray(rows,1),resultGraypart(rows,1),resultMST(rows,1),resultMSTpart(rows,1);
		file1<<rows<<"	"<<columns<<"	"<<(double)rows/columns<<"	"<<(double)columns/rows<<"	";
		vect.randomize(0,100);
		cout<<"randomize mat\n";
		mat.randomizedense(sparsity);
		cout<<"reg\n";
		resultReg=mat.mult(vect,file1);
		cout<<"multmstpart\n";
		resultMSTpart=mat.multmstpartitioned(vect,file1,partitionsize);
		cout<<"multgraypart\n";
		resultGraypart=mat.multGrayParts(vect, file1,partitionsize);
		assert(resultGraypart.eq(resultReg)&&"GrayCode code failed");
		assert(resultReg.eq(resultMSTpart)&&"mst partitioned messed up");
		file1<<endl;
	}	
	file1.close();
	
	/*
	//VARY partition size NEED TO FIGURE OUT HOW TO DO THIS TEST.
	file1.open("partitionTest2.txt");
	file1<<"rows 	cols 	partitionsize	totalElements	RegularOp	MSTpartOp	GrayPartOp\n";	
	for(int partitionsize=4; partitionsize<17; partitionsize++){
		int totalElements = 262144;
		int rows=512;
		int columns=512;
		if(partitionsize==10) partitionsize=16;
		if(columns%partitionsize!=0){
			rows=partitionsize*partitionsize*partitionsize;
			columns=rows;
			totalElements=columns*rows;
		}
		cout<<partitionsize<<endl;
		Bitmatrix mat(rows, columns);
		cout<<"make vect\n";
		Doublematrix vect(columns,1);
		cout<<"make result vects\n";
		Doublematrix resultReg(rows,1),resultGray(rows,1),resultGraypart(rows,1),resultMST(rows,1),resultMSTpart(rows,1);
		file1<<rows<<"	"<<columns<<"	"<<partitionsize<<"	"<<totalElements<<"	";
		vect.randomize(0,100);
		cout<<"randomize mat\n";
		mat.randomizedense(sparsity);
		cout<<"reg\n";
		resultReg=mat.mult(vect,file1);
		cout<<"multmstpart\n";
		resultMSTpart=mat.multmstpartitioned(vect,file1,partitionsize);
		cout<<"multgraypart\n";
		resultGraypart=mat.multGrayParts(vect, file1,partitionsize);
		assert(resultGraypart.eq(resultReg)&&"GrayCode code failed");
		assert(resultReg.eq(resultMSTpart)&&"mst partitioned messed up");
		file1<<endl;
	}	
	file1.close();
	*/
	/*
	//VARY SIZE
	file1.open("sizeTest2.txt");
	file1<<"rows 	cols 	size 	RegularOp	MSTpartOp	GrayPartOp\n";	
	for(int columns=2; columns<=512; columns*=2){
		if(columns%partitionsize!=0) continue;
		int rows = columns;
		int totalElements=rows*columns;
		cout<<"rows: "<<rows<<" columns: "<<columns<<" rows*columns="<<rows*columns<<endl;
		Bitmatrix mat(rows, columns);
		cout<<"make vect\n";
		Doublematrix vect(columns,1);
		cout<<"make result vects\n";
		Doublematrix resultReg(rows,1),resultGray(rows,1),resultGraypart(rows,1),resultMST(rows,1),resultMSTpart(rows,1);
		file1<<rows<<"	"<<columns<<"	"<<totalElements<<"	";
		vect.randomize(0,100);
		cout<<"randomize mat\n";
		mat.randomizedense(sparsity);
		cout<<"reg\n";
		resultReg=mat.mult(vect,file1);
		cout<<"multmstpart\n";
		resultMSTpart=mat.multmstpartitioned(vect,file1,partitionsize);
		cout<<"multgraypart\n";
		resultGraypart=mat.multGrayParts(vect, file1,partitionsize);
		assert(resultGraypart.eq(resultReg)&&"GrayCode code failed");
		assert(resultReg.eq(resultMSTpart)&&"mst partitioned messed up");
		file1<<endl;
	}
	file1.close();
	*/
	/*
	//VARY sparsity
	file1.open("SparsityTest2.txt");
	file1<<"rows 	cols 	sparsity 	RegularOp	MSTpartOp	GrayPartOp\n";	
	for(int sparsity=0; sparsity<=105; sparsity+=5){
		if(columns%partitionsize!=0) continue;
		cout<<"percent of 1s: "<<sparsity<<endl;
		Bitmatrix mat(rows, columns);
		cout<<"make vect\n";
		Doublematrix vect(columns,1);
		cout<<"make result vects\n";
		Doublematrix resultReg(rows,1),resultGray(rows,1),resultGraypart(rows,1),resultMST(rows,1),resultMSTpart(rows,1);
		file1<<rows<<"	"<<columns<<"	"<<sparsity<<"	";
		vect.randomize(0,100);
		cout<<"randomize mat\n";
		mat.randomize1spercent(sparsity);
		cout<<"reg\n";
		resultReg=mat.mult(vect,file1);
		cout<<"multmstpart\n";
		resultMSTpart=mat.multmstpartitioned(vect,file1,partitionsize);
		cout<<"multgraypart\n";
		resultGraypart=mat.multGrayParts(vect, file1,partitionsize);
		assert(resultGraypart.eq(resultReg)&&"GrayCode code failed");
		assert(resultReg.eq(resultMSTpart)&&"mst partitioned messed up");
		file1<<endl;
	}
	file1.close();
	*/
	return 0;
}