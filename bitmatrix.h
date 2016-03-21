#ifndef BITMATRIX
#define BITMATRIX
#include <iostream>
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"
#include <cmath>
#include <ctime>
#include <chrono> 
#include <vector>
#include <cassert>
#include "doublematrix.h"
#include "boost/random/uniform_real_distribution.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/properties.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/config.hpp"
#include "boost/graph/prim_minimum_spanning_tree.hpp"
#include "boost/graph/depth_first_search.hpp"
#include <functional>
#include <fstream>
using namespace std;
using namespace boost;
//GLOBAL adjacency list, I Hope to find
//A better solution soon. 
//SEE struct detect_loops
static vector<vector<int>> alist;
//GLOBAL for operation count between partitions, dont have time
//for a better solution at the moment,too tired.
static int partitionoperationcount;
//TYPEDEFS FOR BOOST GRAPH LIBRARY
typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef property<vertex_name_t, int> VertexNameProperty;
typedef adjacency_list<vecS, vecS, undirectedS,VertexNameProperty, EdgeWeightProperty> Graph;
typedef graph_traits<Graph>::vertex_descriptor bVertex;
typedef graph_traits<Graph>::edge_descriptor bEdge;
typedef graph_traits<Graph>::adjacency_iterator Tadjit;
typedef std::pair<Tadjit, Tadjit> viterator;



//SETTING RANDOM SEED FOR RANDOMIZING MATRICIES
unsigned seed=std::chrono::system_clock::now().time_since_epoch().count();
boost::random::mt19937 gen (seed);
int random01() {

    boost::random::uniform_int_distribution<> dist(0, 1);
 
    return dist(gen);
}
int random0tomax(int max) {

    boost::random::uniform_int_distribution<> dist(0, max);
 
    return dist(gen);
}


class Bitmatrix{
private:


//BOOST GRAPH LIBRARY THINGS
vector<bVertex> bVertecies;
vector<bEdge> bEdges;
property_map<Graph, edge_weight_t>::type HammingDistance = get(edge_weight, g);
property_map<Graph, vertex_name_t>::type vertexname = get(vertex_name, g);
Graph g,mst;

////
//partitioning things
vector<Bitmatrix> partitions;

////

int rows;
int columns;
//TRUE IF THE MST HAS BEEN CALCULATED, FALSE OTHERWISE
bool mstcalculated; 
bool partitionmstcalculated;
//v is the bitmatrix itself. 
std::vector<boost::dynamic_bitset<> > v;

//VISITOR CLASS FOR Depth first search
//DISCOVER_VERTEX is called at every vertex visited
//NOTE: COULD NOT SAVE ANYTHING INSIDE OF HERE AFTER
// Depth_firsT_Search was called, so the only solution
//to hold the new alist was to make alist a global.
//alterations of members of an object of this class
//inside of the depth first search call
//would not retain after depth_first_search finished?
struct detect_loops : public boost::dfs_visitor<>
{
	
  detect_loops(int rows){
  	alist.resize(rows);
  }
  void discover_vertex(bVertex v, const Graph& g) {
     Tadjit iter,iterend;
     tie(iter, iterend)=adjacent_vertices(v,g);
     for(;iter!=iterend;++iter){
     	alist[v].push_back(*iter);
 

     }
	  return;

  }
  
};
public:
Bitmatrix(){
	rows=0;
	columns=0;
}
Bitmatrix(int rows, int columns){
	this->rows=rows;
	this->columns=columns;
	mstcalculated=false;
	partitionmstcalculated=false;

	v.resize(rows);
	for(int i=0;i<rows;i++)
		v[i].resize(columns);

}
void initialize(int rows, int columns){
	this->rows=rows;
	this->columns=columns;
	this->mstcalculated=false;
	this->partitionmstcalculated=false;

	v.resize(rows);
	for(int i=0; i<rows; i++)
		v[i].resize(columns);

}
void setzero(){
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=1;

	v[1][2]=0;

}
void setone(){
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=1;
}
void randomizedense(int rate){
	mstcalculated=false;
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++){
			int randomnum=random0tomax(rate);
			if(randomnum)
				v[i][j]=1;
			else 
				v[i][j]=0;
		}

}
void randomize1spercent(int rate){
	mstcalculated=false;
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++){
			int randomnum=random0tomax(100);
			if(randomnum<rate)
				v[i][j]=1;
			else 
				v[i][j]=0;
		}

}
void randomizesparse(int rate){
	mstcalculated=false;
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++){
			int randomnum=random0tomax(rate);
			if(!randomnum)
				v[i][j]=1;
			else 
				v[i][j]=0;
		}

}
void setelement(int row, int column, int value){
	v[row][column]=value;
	return;
}
void randomize(){
	mstcalculated=false;
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=random01();

}

void rprint(){
	for(int i=0;i<rows;i++){
		for(int j=0;j<columns;j++)
			cout<<v[i][j];
		cout<<std::endl;
	}
}
void printHDgraph(){
	//boost::dynamic_bitset<> temp;
	for(int i=1;i<rows;i++){
		for(int j=0;j<i;j++){
			std::cout<<(v[i]^v[j]).count();			
			//wow this works^^^^^
			//temp=(v[i]^v[j]);
			//std::cout<<temp.count();
		}
		std::cout<<endl;


	}
}
int getHD(int i, int j){
	//	cout<<"g";

	return ((v[i]^v[j]).count());
}


int operator()(int r, int c){
	return v[r][c];
}
//Bitmatrix operator * (Doublematrix x){}
int getelement(int r, int c){
	return v[r][c];
}
boost::dynamic_bitset<> rowXORrow(int index1, int index2){
	return v[index1]^v[index2];
}
int getcolumns(){return columns;}
int getrows(){return rows;}

//BOOST STUFF FOR MST
Graph getGraph(){
	return g;
}
void fillgraph(){
	bVertecies.resize(rows);
	bEdges.resize(rows*rows);

	//ADD ALL VERTECIES TO THE GRAPH
	for(int i=0;i<rows;i++){
		bVertecies[i]=add_vertex(g);
		vertexname[bVertecies[i]]=i;

	}

	//ADD ALL POSSIBLE EDGES TO THE GRAPH
	int edgecounter=0;
	for(int i=0;i<rows;i++)
		for (int j=i+1;j<rows;j++){
			bEdges[edgecounter]=(add_edge(bVertecies[i],bVertecies[j],g)).first;
			HammingDistance[bEdges[edgecounter]]=getHD(i,j);
			edgecounter++;
	}
	//graph should be filled now with all vertecies
	//and all possible edges in g




}

void boostcalcmst(){
	fillgraph();
	std::vector < graph_traits < Graph >::vertex_descriptor > p(num_vertices(g));
	prim_minimum_spanning_tree(g, &p[0]);
	for(int i=0;i<rows;i++){
		bVertecies[i]=add_vertex(mst);
		vertexname[bVertecies[i]]=i;

	}
	for (std::size_t i = 0; i != p.size(); ++i){
		if(p[i]!=i)
			bEdges[i]=(add_edge(bVertecies[p[i]],bVertecies[i],mst)).first;

	}
	mstcalculated=true;

}


void boostcalcalist(){
	  alist.clear();
	  detect_loops vis(rows);
	  depth_first_search(mst, visitor(vis));
}


Doublematrix multGrayJustDelcarations(Doublematrix d,ostream& myfile){
	boost::dynamic_bitset<> differset;
	int grayRows =1 << columns;
	Doublematrix scratch(grayRows,1);
	Doublematrix output(rows, 1);
	std::vector<boost::dynamic_bitset<> > gray(grayRows, boost::dynamic_bitset<>(columns));
	return output;
}

int grayRows;
int numParts;
std::vector<boost::dynamic_bitset<> > gray;

void initGrayCode(){
	grayRows = 1<<columns;
	gray.resize(grayRows, boost::dynamic_bitset<>(columns));
	for(int i=0; i<grayRows; i++){
		for(int j=0; j<columns;j++){
			gray[i][j]=(i>>j)&1;
		}
		//calculating the ith greycode row from the binary representation of i
		//formula given in Hackers Delight
		gray[i]=gray[i]^(gray[i]>>1);
	}
}
Doublematrix multGrayParts(Doublematrix d, ostream& myfile, int partSize){
	//split the multGray algorithm into partitions of partSize.
	//To make this simple we assume that the number of columns is a 
	//multiple of partSize. 
	int start;
	int opcount=0; 
	assert(columns%partSize==0&&"columns must be a multiple of partSize"); 
	Doublematrix output(rows, 1);
	output.setzero();
	numParts = columns / partSize;
	int grayPartRows = 1<<partSize;
	std::vector<boost::dynamic_bitset<> > grayPart(grayPartRows, boost::dynamic_bitset<>(partSize));
	for(int i=0; i<grayPartRows; i++){
		for(int j=0; j<partSize;j++){
			grayPart[i][j]=(i>>j)&1;
		}
		//calculating the ith greycode row from the binary representation of i
		//formula given in Hackers Delight
		grayPart[i]=grayPart[i]^(grayPart[i]>>1);
	}
	std::vector<Doublematrix> scratch(numParts, Doublematrix(grayPartRows,1));
	boost::dynamic_bitset<> differset;
	//calculate scratch y's
	for(int k=0; k<numParts; k++){
		//for each partition calculate scratch[k]
		//opcount+=1;
		start = k * partSize;
		for(int i=0; i<grayPartRows; i++){

			if(i==0)
				scratch[k].setelement(0,0,0);
			else{
				differset=grayPart[i]^grayPart[i-1];
				for(int j=0; j<partSize;j++){
				if(differset[j]){
					opcount+=1;
					if(grayPart[i][j])//current has as 1 and previous has a 0
						scratch[k].setelement(i,0,scratch[k].getelement(i-1,0)+d.getelement(start+j,0));
					else //current has a 0 and previous has a 1
						scratch[k].setelement(i,0,scratch[k].getelement(i-1,0)-d.getelement(start+j,0));
					break;
					}
				}
			}
		}
	}

	//for each partition of the actual matrix, compute the sub result and add to the total. 
	boost::dynamic_bitset<> G(partSize),index, entireRow;
	int index_int;
	for(int k=0; k<numParts;k++){
		for(int i=0; i<rows; i++){
			entireRow=v[i];
			//opcount+=1;
			start = k*partSize;
			for(int j=0, jk=start; j<partSize; j++, jk++)
				G[j]=entireRow[jk];
			index=G;
			//row i is the row in the operand matrix
			//reverse the gray code operation to find the index in the grey code corresponding to
			//the matix row
			for(int j=1; j<partSize+i; j++){
				index=index^(G>>j);
			}
			index_int=0;
			//convert the index from the bitset to an integer
			for(int j=0; j<index.size(); j++){
				index_int=index_int|(index[j]<<j);
			}
			//use the calculated index to fill the ith entry in the result matrix
			if(scratch[k].getelement(index_int,0)){
				opcount++;
				output.setelement(i,0,scratch[k].getelement(index_int,0)+output.getelement(i,0));
			}
		}
	}

	myfile<<opcount<<"	";
	return output;
}
Doublematrix multGray(Doublematrix d,ostream& myfile){
	boost::dynamic_bitset<> differset;
	Doublematrix output(rows, 1);
	int opcount=0;
	//generate grey code and calculate scratch y 
	//gray.resize(grayRows);

	initGrayCode();
	Doublematrix scratch(grayRows,1);
	for(int i=0; i<grayRows; i++){	
		if(i==0)
			scratch.setelement(0,0,0);
		else{
			differset=gray[i]^gray[i-1];
			for(int j=0; j<columns;j++){
			if(differset[j]){
				opcount++;
				if(gray[i][j])//current has as 1 and previous has a 0
					scratch.setelement(i,0,scratch.getelement(i-1,0)+d.getelement(j,0));
				else //current has a 0 and previous has a 1
					scratch.setelement(i,0,scratch.getelement(i-1,0)-d.getelement(j,0));
				break;
				}
			}
		}
	}
	//calculate the output vector
	//find the index in the scratch y that corresponds to each row in the 
	//operand matrix
	boost::dynamic_bitset<> G,index;
	int index_int;
	
	for(int i=0; i<rows; i++){
		G=v[i];
		index=G;
		//row i is the row in the operand matrix
		//reverse the gray code operation to find the index in the grey code corresponding to
		//the matix row
		for(int j=1; j<columns; j++){
			index=index^(G>>j);
		}
		index_int=0;
		//convert the index from the bitset to an integer
		for(int j=0; j<index.size(); j++){
			index_int=index_int|(index[j]<<j);
		}
		//use the calculated index to fill the ith entry in the result matrix
		output.setelement(i,0,scratch.getelement(index_int,0));
	}
	myfile<<opcount<<"	";
	return output;

}

Doublematrix multmst(Doublematrix d,ostream& myfile){
	//assert(columns==d.getrows());
	int operationcount=0;
	if(!mstcalculated)
		boostcalcmst();
	Doublematrix temp(rows,1);
	//cout<<1;
	double sum=0;
	//traverse the tree DFS or BFS
	//calcfirst row  as rootthen follow tree off
	int start=minpop();
	for(int i=0;i<columns;i++){
		if(v[start][i])
			sum+=d.getelement(i,0);
	}
	temp.setelement(start,0,sum);
	//cout<<2;
//	cout<<"sum of start row is: "<< sum<<endl;
	boost::dynamic_bitset<> marker;
	//cout<<3;
	vector<vector<int>> tempadjlist=alist;
	//cout<<4;
	vector<vector<int>> queues;
	vector<int> workingonqueue;
	
	queues.resize(rows);
	marker.resize(rows);
	//alist[start].back = last element
	//mark last element marker[^]=1 when visited
	//alist[start].pop_back ==delete last element
/*
ALGORITHM

CALCULATE START ROW
WORK ON START AND QUEUE ALL VERTECIES THAT ARE USED WITH START IN (THINGS TO WORK ON QUEUE)
MARK START
WHiLE THERE ARE THINGS TO WORK ON:
	DEQUEUE (a) FROM THINGS TO WORK ON
	work on a and queue all unmarked vertecies that are used with a in things to work on.
	mark a
END WHILE
*/
	workingonqueue.push_back(start);
	//cout<<5;
	do{
		int position=workingonqueue.back();
		//cout<<"position is: "<<position<<endl;
		//cout<<"workingonqueue is: "<<workingonqueue.size()<<endl;
		workingonqueue.pop_back();
		//cout<<6;
		//calculate all unmarked adjacent vertecies to position
		//while the temp adjlist is not empty
		boost::dynamic_bitset<> differmatrix;
		differmatrix.resize(columns);
		while(!tempadjlist[position].empty()){
		//if vertecy at the end is not marked
		int last=tempadjlist[position].back();
		//cout<<7;

			if(!marker[last]){
				//   work on the vertecy at the end
					sum=temp.getelement(position,0);
					//find where v[last] and v[position] differ
					differmatrix=v[last]^v[position];
					//<<"differmatrix is: "<<differmatrix<<endl;

					//in the all of the places that they differ:
						//if v[position][i]==1, subtract d[i] from sum
						//if v[position][i]==0, add [i] to sum
						for(int i=0;i<columns;i++)
						if(differmatrix[i]) {//they differ in this spot
								if (v[position][i]){
									//cout<< sum<< " - "<<d.getelement(i,0)<<endl;
									sum-=d.getelement(i,0);
									operationcount++;
								}
								else {
									//cout<< sum<< " + "<<d.getelement(i,0)<<endl;
									operationcount++;

									sum+=d.getelement(i,0);
								}
							}
					//assign sum to temp
					temp.setelement(last, 0, sum);
					//cout<<"setting next row to: "<<sum<<endl;
					//enqueue it to working on queue
				workingonqueue.push_back(last);
			}

		//delete end from tempadjlist
		tempadjlist[position].pop_back();

	
		}


		//mark position
		marker[position]=1;

	}
	while(!workingonqueue.empty());

	myfile<<operationcount<<"	";
	return temp;

}
Doublematrix mult(Doublematrix d, ostream& myfile){

	//cout<<endl<<"start"<<endl;
	assert(columns==d.getrows());
	int operationcount=0;
	Doublematrix temp(rows,1);
	double sum;
	for(int i=0;i<rows;i++){
		sum=0;
		//cout<<1<<endl;
		for(int j=0;j<columns;j++){
		//cout<<2<<endl;
			if(v[i][j]){
				sum+=d.getelement(j,0);
				operationcount++;
			}

			}
		//cout<<3<<endl;

		temp.setelement(i,0,sum);
		//cout<<temp.getelement(i,0)<<endl;

		//cout<<4<<endl;


	}
	myfile<< operationcount<<"	";
	//cout<<5<<endl;


	return temp;


}
Doublematrix operator *= (Doublematrix d){
	assert(columns==d.getrows());


}	


int minpop(){
	//returns the minimum index
	int minpop=v[0].count();
	int minindex=0;
	int temp;
	for(int i=1;i<rows;i++){
		temp=v[i].count();
		if(temp<minpop){
			minindex=i;
			minpop=temp;
		}
	}
	return minindex;
		
}
void partition(int numberOfPartitions){
	//fills partition vector, splits this.v into numberOfPartitions vertical strips

	//(1) initialize partitions
	assert(columns%numberOfPartitions==0);
	int partitionsize=columns/numberOfPartitions;
	assert(columns%partitionsize==0);
	partitions.resize(numberOfPartitions);
	for(int i=0; i<numberOfPartitions;i++)
		partitions[i].initialize(rows, partitionsize);
	//(2) fill each partition
	for(int i=0; i<numberOfPartitions;i++)
		for(int j=0;j<rows;j++)
			for (int k=partitionsize*i,m=0;m<partitionsize;m++,k++)
				partitions[i].setelement(j,m,getelement(j,k));
	


}
void calcMSTofPartitions(int numberOfPartitions){
	partition(numberOfPartitions);
	partitionmstcalculated=true;
	for(int i=0; i<numberOfPartitions;i++){
		partitions[i].boostcalcmst();
	}

}
Doublematrix multmstpartitioned(Doublematrix d,ostream& myfile, int partsize){//numberOfPartitions){
	int partitionsize=partsize;//=columns/numberOfPartitions;
	int numberOfPartitions;
	numberOfPartitions=columns/partsize;
	if(columns%partsize!=0)
		numberOfPartitions++;
	if(!partitionmstcalculated)
		calcMSTofPartitions(numberOfPartitions);
	vector<Doublematrix> temps;
	temps.resize(numberOfPartitions);
	for(int i=0; i<numberOfPartitions;i++){
		temps[i].initialize(rows,1);
	}
	partitionoperationcount=0;
	for(int m=0; m<numberOfPartitions;m++){
		partitions[m].boostcalcalist();
		//partitions[m].rprint();
			//cout<<1;
			double sum=0;
			//traverse the tree DFS or BFS
			//calcfirst row  as rootthen follow tree off
			int start=partitions[m].minpop();
			for(int i=0,n=partitionsize*m;i<partitionsize;n++,i++){
				//cout<<endl<<partitions[m].getelement(start,i)<<' '<<i;


				if(partitions[m].getelement(start,i)){
					partitionoperationcount++;
					sum+=d.getelement(n,0);
				}
			}
			//cout<<sum<<endl;
			temps[m].setelement(start,0,sum);
			//cout<<2;
		//	cout<<"sum of start row is: "<< sum<<endl;
			boost::dynamic_bitset<> marker;
			//cout<<3;
			vector<vector<int>> tempadjlist=alist;
			//cout<<4;
			vector<vector<int>> queues;
			vector<int> workingonqueue;
			
			queues.resize(rows);
			marker.resize(rows);
			//alist[start].back = last element
			//mark last element marker[^]=1 when visited
			//alist[start].pop_back ==delete last element
		/*
		ALGORITHM

		CALCULATE START ROW
		WORK ON START AND QUEUE ALL VERTECIES THAT ARE USED WITH START IN (THINGS TO WORK ON QUEUE)
		MARK START
		WHiLE THERE ARE THINGS TO WORK ON:
			DEQUEUE (a) FROM THINGS TO WORK ON
			work on a and queue all unmarked vertecies that are used with a in things to work on.
			mark a
		END WHILE
		*/
			workingonqueue.push_back(start);
			//cout<<5555555555<<endl;
			do{
				int position=workingonqueue.back();
				//cout<<m<<"position is: "<<position<<endl;
				//cout<<"workingonqueue is: "<<workingonqueue.size()<<endl;
				workingonqueue.pop_back();
				//cout<<666;
				//calculate all unmarked adjacent vertecies to position
				//while the temp adjlist is not empty
				boost::dynamic_bitset<> differmatrix;
				differmatrix.resize(partitionsize);
				while(!tempadjlist[position].empty()){
				//if vertecy at the end is not marked
				int last=tempadjlist[position].back();
			//	cout<<7;

					if(!marker[last]){
						//   work on the vertecy at the end
							sum=temps[m].getelement(position,0);
							//find where v[last] and v[position] differ
							differmatrix=partitions[m].rowXORrow(last,position); //v[last]^v[position];
							//<<"differmatrix is: "<<differmatrix<<endl;
						//	cout<<endl<<sum;
							//in the all of the places that they differ:
								//if v[position][i]==1, subtract d[i] from sum
								//if v[position][i]==0, add [i] to sum
								for(int i=0,n=partitionsize*m;i<partitionsize;n++,i++)
								if(differmatrix[i]) {//they differ in this spot
										if (partitions[m].getelement(position,i)){//v[position][i]){
										//	cout<< sum<< " - "<<d.getelement(n,0)<<endl;
											sum-=d.getelement(n,0);
											partitionoperationcount++;
										}
										else {
										//	cout<< sum<< " + "<<d.getelement(n,0)<<endl;
											partitionoperationcount++;

											sum+=d.getelement(n,0);
										}
									//		cout<<endl<<sum;

									}
							//assign sum to temp
							temps[m].setelement(last, 0, sum);
							//cout<<"setting next row to: "<<sum<<endl;
							//enqueue it to working on queue
						workingonqueue.push_back(last);
					}

				//delete end from tempadjlist
				tempadjlist[position].pop_back();

			
				}


				//mark position
				marker[position]=1;

			}
			while(!workingonqueue.empty());
		



	}
	//cout<<endl<<endl;
	//for(int i=0; i<numberOfPartitions;i++){
		//temps[i].rprint();cout<<endl;
	//}
	Doublematrix temp(rows,1);
	int mergenum=temp.addto(temps,numberOfPartitions);
	partitionoperationcount+=mergenum;
	myfile<<partitionoperationcount<<"	";
	return temp;

}


};


#endif


//