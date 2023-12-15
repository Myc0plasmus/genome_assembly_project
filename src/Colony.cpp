#include<bits/stdc++.h>
#include "Colony.h"
#include "Sequence.h"

using namespace std;

int Colony::findInRoulette(float val, vector<float> & roulette){
	int start = 0;
	int end = (int)roulette.size() - 1;
	while(true){
		if(start == end) return end;
		if(start == end - 1){
			return (val <= roulette[start])?start:end;
		}
		int middle = (start+end)/2;
		if(val <= roulette[middle]) end = middle;
		else start = middle;
	}
}

Colony::Colony(Sequence & newSeq) : seq(newSeq){
	newPheromones = new vector<pair<double,deque<int>>>();
	pheromones = new double* [this->seq.graphSize];
	for(int i =0;i<this->seq.graphSize;i++) pheromones[i] = new double [this->seq.graphSize]();
	this->loneInstance = true;	
}

Colony::Colony(Sequence & newSeq, double ** pheromones, vector<pair<double,deque<int>>> * newPheromones) : seq(newSeq){
	this->newPheromones = newPheromones;
	this->pheromones = pheromones;
	this->loneInstance = false;
}

Colony::~Colony(){
	if(loneInstance){
		delete this->newPheromones;
		for(int i =0;i<this->seq.graphSize;i++) delete [] pheromones[i];
		delete [] pheromones;
	}
}
