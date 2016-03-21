#ifndef DOUBLEMATRIX
#define DOUBLEMATRIX
#include <iostream>
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/normal_distribution.hpp"
#include "boost/random/variate_generator.hpp"
#include "boost/random/uniform_real_distribution.hpp"
#include <ctime>
#include <chrono> 
#include <vector>
#include <cmath>
#include "bitmatrix.h"
using namespace std;
//typedef boost::mt19937                     ENG;    // Mersenne Twister
//typedef boost::normal_distribution<double> DIST;   // Normal Distribution
//typedef boost::variate_generator<ENG,DIST> GEN;    // Variate generator

unsigned seed2=std::chrono::system_clock::now().time_since_epoch().count();
boost::random::mt19937 gen2 (seed2);
double randomint(int min,int  max) {


    boost::random::uniform_int_distribution<> dist(min, max);
 
    return dist(gen2);
}

double randomdouble(double min,double  max) {

    boost::random::uniform_real_distribution<> dist(min, max);
 
    return dist(gen2);
}


class Doublematrix{
private:
unsigned long long int rows;
unsigned long long int columns; 
std::vector<std::vector<double> > v;
public:
Doublematrix(){
	rows=0;columns=0;
}
Doublematrix(int rows){
	this->rows=rows;
	this->columns=1;
	v.resize(rows);
	for(int i=0;i<rows;i++){
		v[i].resize(1);
		v[i][0]=0;
	}

}
Doublematrix(int rows, int columns){
	this->rows=rows;
	this->columns=columns;
	v.resize(rows);
	for(int i=0;i<rows;i++){
		v[i].resize(columns);
		v[i][0]=0;
	}

}
void initialize(int rows, int columns){
	this->rows=rows;
	this->columns=columns;
	v.resize(rows);
	for(int i=0;i<rows;i++){
		v[i].resize(columns);
		v[i][0]=0;
	}
}
void setzero(){
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=0;

}
void setone(){
for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=1;
}
void randomize(double lower, double upper){
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=randomint(lower,upper);
}
void rprint(){
	for(int i=0;i<rows;i++){
		for(int j=0;j<columns;j++)
			cout<<v[i][j];
		cout<<std::endl;
	}
}
int getrows(){

	return rows;
}
double operator()(int r){
	return v[r][0];
}
double operator()(int r, int c){
	return v[r][c];
}
double getelement(int r,int c){
	return v[r][c];
}
void operator =(Doublematrix in){
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=in.getelement(i,j);

}
int addto(vector<Doublematrix> d, int amount){
	setzero();
	int counter=0;
	for(int m=0;m<amount;m++)
		for(int i=0;i<rows;i++)
			for(int j=0;j<columns;j++){
				if(d[m].getelement(i,j)){
								counter++;
								v[i][j]+=d[m].getelement(i,j);
							}
			}
		return counter;

}
bool eq(Doublematrix in){
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++){
			if(v[i][j]!=in.getelement(i,j))
				return false;
		}
		return true;
}
void setelement(int r, int c, double input){

	v[r][c]=input;
}
};

#endif