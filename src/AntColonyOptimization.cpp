#include<bits/stdc++.h>
#include "AntColonyOptimization.h"
#include "Sequence.h"

using namespace std;

AntColonyOptimization::AntColonyOptimization(Sequence * newSeq){
	this->seq = newSeq;
	pheromones = new double* [this->seq->graphSize];
}

AntColonyOptimization::~AntColonyOptimization(){
		
}

void AntColonyOptimization::ant(int v_start){
	vertice * graph = this->seq->graph.get();	
	
}
