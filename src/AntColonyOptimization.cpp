#include<bits/stdc++.h>
#include "AntColonyOptimization.h"
#include "Sequence.h"

using namespace std;

AntColonyOptimization::AntColonyOptimization(Sequence * newSeq){
	this->seq = newSeq;
	pheromones = new double* [this->seq->graphSize];
	for(int i =0;i<this->seq->graphSize;i++) pheromones[i] = new double [this->seq->graphSize];
}

AntColonyOptimization::~AntColonyOptimization(){
	for(int i =0;i<this->seq->graphSize;i++) delete [] pheromones[i];
	delete [] pheromones;
}

void AntColonyOptimization::ant(){
	srand(time(0));
	int v_first = this->seq->firstElemIdx;
	vertice * graph = this->seq->graph.get();	
	int ** adjacencyMatrix = this->seq->adjacencyMatrix;
	bool * active = new bool [this->seq->graphSize];
	vector<list<int>> paths;
	vector<float> roulette;
	int pathNum = 0;
	int v = v_first;
	paths.push_back(list<int>());
	while(v!=-1){
		paths[pathNum].push_back(v);
		// bool nullWeight = true;
		map<int,int> beginMap;
		for(auto vertice : graph[v].edges){
			float weight = 0;
			bool pathBegin = false;
			
			//I know this looks bad, however it'll rarely run 
			if((int)paths.size() > 1){
				for(int i = 1;i<(int)paths.size();i++) if(vertice.neighbour == paths[i].front() && i != pathNum){
					beginMap[vertice.neighbour] = i;
					pathBegin = true;
					break;
				}
			}
			if(!active[vertice.neighbour] || pathBegin){
				weight += (vertice.val == 1)?1:0;
				weight += pheromones[v][vertice.neighbour];
			}
			// if(nullWeight && !weight) nullWeight = false;

			roulette.push_back( ((!roulette.empty())?roulette.back():0) + weight);
		}	
		v = (roulette.back() == 0 || roulette.empty())?(-1):(rand() % (int)(roulette.back() * 100)+1);
		bool chooseRandom = false;
		if(v == -1){
			chooseRandom = true;
		}
		else{
			for(int i =0;i<(int)roulette.size();i++) if(((float)(v)/(float)(100)) <= roulette[i]) v = i;
			if(beginMap.contains(v)){
				chooseRandom = true;
				for(auto vertice : paths.back()) paths[beginMap[v]].push_front(vertice);
				paths.pop_back();
			}
		}
		if(chooseRandom){
			roulette.clear();
			for(int i=0;i<this->seq->graphSize;i++) if(!active[i]) roulette.push_back(i);
			if(!roulette.empty()){
				v = roulette[rand() % (int)roulette.size()];
				paths.push_back(list<int>());
				pathNum = paths.size() -1;
			}
		}	
	}
	
}
