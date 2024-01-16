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

int Colony::findInWeightedRoulette(float val, vector<float> roulette, vector<int> * weights){
	vector<float> elemNums;
	elemNums.reserve(this->seq.oligo_size+2);
	for(int i =1;i<=this->seq.oligo_size;i++ ){
		elemNums.push_back(((!elemNums.empty())?elemNums.back():0)+(int)weights[i].size());
	}
	elemNums.push_back(elemNums.back()+1);
	elemNums.push_back(elemNums.back()+1);

	int rouletteScore = this->findInRoulette(val, roulette);
	int weightIdx = this->findInRoulette(rouletteScore+1, elemNums) ;
	if(weightIdx+1>this->seq.oligo_size) {
		return rouletteScore;}
	return weights[weightIdx+1][(weightIdx)?(rouletteScore-elemNums[weightIdx-1]):rouletteScore];
	
	
}

Colony::Colony(Sequence & newSeq) : seq(newSeq){
	newPheromones = new vector<pair<long double,deque<int>>>();
	pheromones = new long double* [this->seq.graphSize];
	for(int i =0;i<this->seq.graphSize;i++) pheromones[i] = new long double [this->seq.graphSize]();
	this->loneInstance = true;	
}

Colony::Colony(Sequence & newSeq, long double ** linkedPheromones, vector<pair<long double,deque<int>>> * linkedNewPheromones) : seq(newSeq){
	this->newPheromones = linkedNewPheromones;
	this->pheromones = linkedPheromones;
	this->loneInstance = false;
}

Colony::~Colony(){
	if(loneInstance){
		delete this->newPheromones;
		for(int i =0;i<this->seq.graphSize;i++) delete [] pheromones[i];
		delete [] pheromones;
	}
}

void Colony::pheremoneApplyEvent(){

}

void Colony::pheromoneEvaporationEvent(){

}

void Colony::antColonyFinishEvent(){

}

void Colony::filterPheromoneTrailEvent(int lastBest){

}
