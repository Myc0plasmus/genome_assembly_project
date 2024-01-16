#include<bits/stdc++.h>
#include "AntColonyOptimization.h"
#include "Sequence.h"
#include<glog/logging.h>

using namespace std;

static int NUM_OF_ANTS = 50;
static int SMOOTHING_LOG_BASE = 3;
static int SMOOTHING_LOWEST =  0.01;
static int EVAPORATION_RATE = 0.01;
static int STOP_TIME = 5;

AntColonyOptimization::AntColonyOptimization(Sequence & newSeq) : seq(newSeq){
	this->evaporationRate = EVAPORATION_RATE; 
	this->smoothingLowest = SMOOTHING_LOWEST;
	this->smoothingLogBase = SMOOTHING_LOG_BASE;
	this->numOfAnts = NUM_OF_ANTS;
	this->stopTime = STOP_TIME;
	this->pheromones = new long double* [this->seq.graphSize+10]();
	for(int i =0;i<this->seq.graphSize;i++) pheromones[i] = new long double [this->seq.graphSize+10]();
	// newPheromones = new double* [this->seq.graphSize];
	// for(int i =0;i<this->seq.graphSize;i++) newPheromones[i] = new double [this->seq.graphSize]();
	// for(int i =0;i<this->seq.graphSize;i++){
	// 	for(int j=0;j<this->seq.graphSize;j++) cout<<pheromones[i][j]<<" ";
	// 	cout<<endl;
	// }
		
}


AntColonyOptimization::AntColonyOptimization(Sequence & newSeq, int numOfAnts) : AntColonyOptimization(newSeq)
{
	this->numOfAnts = numOfAnts;
}AntColonyOptimization::


AntColonyOptimization::AntColonyOptimization(Sequence & newSeq, int numOfAnts,double newEvaporationRate) : AntColonyOptimization(newSeq, numOfAnts)
{
	this->evaporationRate = newEvaporationRate;
}

AntColonyOptimization::AntColonyOptimization(Sequence & newSeq,int numOfAnts, double newEvaporationRate, double newSmoothingLowest, int newSmoothingLogBase) : AntColonyOptimization(newSeq, numOfAnts, newEvaporationRate)
{
	this->smoothingLowest = newSmoothingLowest;
	this->smoothingLogBase = newSmoothingLogBase;
}

AntColonyOptimization::~AntColonyOptimization(){
	for(int i =0;i<this->seq.graphSize;i++) delete [] pheromones[i];
	delete [] pheromones;
	// for(int i =0;i<this->seq.graphSize;i++) delete [] newPheromones[i];
	// delete [] newPheromones;
}
void AntColonyOptimization::resetEssentialParts(){
	this->pheromones = new long double* [this->seq.graphSize+10]();
	for(int i =0;i<this->seq.graphSize;i++) pheromones[i] = new long double [this->seq.graphSize+10]();
	// for(int i =0;i<this->seq.graphSize;i++)
	// 	for(int j =0;j<this->seq.graphSize;j++)
	// 		pheromones[i][j] = 0;
	this->resetSequence();
}
int AntColonyOptimization::getNumOfAnts(){
	return this->numOfAnts;
}
void AntColonyOptimization::resetSequence(){
	this->seq.shredSequence();
	this->seq.createGraphWithFixedCover();
}

void AntColonyOptimization::setNumOfAnts(int numOfAnts){
	this->numOfAnts = numOfAnts;
	this->resetEssentialParts();
}

void AntColonyOptimization::resetNumOfAnts(){
	this->numOfAnts = NUM_OF_ANTS;
}

void AntColonyOptimization::setEvaporationRate(int evaporationRate){
	this->evaporationRate = numOfAnts;
	this->resetEssentialParts();
}

void AntColonyOptimization::resetEvaporationRate(){
	this->evaporationRate = EVAPORATION_RATE;
}

void AntColonyOptimization::setSmoothingLogBase(int smoothingLogBase){
	this->smoothingLogBase = smoothingLogBase;
	this->resetEssentialParts();

}

void AntColonyOptimization::resetSmoothingLogBase(){
	this->smoothingLogBase = SMOOTHING_LOG_BASE;
}

void AntColonyOptimization::setSmoothingLowest(int smoothingLowest){
	this->smoothingLowest = smoothingLowest;
	this->resetEssentialParts();
}

void AntColonyOptimization::resetSmoothingLowest(){
	this->smoothingLowest = SMOOTHING_LOG_BASE;
}


void AntColonyOptimization::pheromoneEvaporation(Colony * colonyType){
	colonyType->pheromoneEvaporationEvent();
	bool debug = false;
	vertice * graph = this->seq.graph.get();
	int v = this->seq.firstElemIdx;
	bool * active = new bool[this->seq.graphSize]();
	queue<int> q;
	q.push(v);
	while(!q.empty()){
		v = q.front();
		q.pop();
		for(auto node : graph[v].edges){
			if(this->pheromones[v][node.neighbour] != 0){
				if(this->pheromones[v][node.neighbour] <= this->evaporationRate || this->pheromones[v][node.neighbour] - this->evaporationRate < this->evaporationRate ){ 
					pheromones[v][node.neighbour] = 0;
				}
				else{
					pheromones[v][node.neighbour] -= this->evaporationRate;
				}
			} 
			if(!active[node.neighbour]){
				active[node.neighbour] = true;
				q.push(node.neighbour);
			}
		}
		
		
	}
	delete [] active;
	if(debug){
		cout<<"evaporate pheromones"<<endl;
		for(int i =0;i<this->seq.graphSize;i++){
			for(int j = 0;j<this->seq.graphSize;j++)
				cout<<this->pheromones[i][j]<<" ";
			cout<<endl;
		}
	}
}

void AntColonyOptimization::pheremoneSmoothing(){
	bool debug = false;
	vertice * graph = this->seq.graph.get();
	int v = this->seq.firstElemIdx;
	bool * active = new bool[this->seq.graphSize]();
	queue<int> q;
	q.push(v);
	while(!q.empty()){
		v = q.front();
		q.pop();
		for(auto node : graph[v].edges){
			if(debug){
				cout<<"pheromones[v][node.neighbour]: "<<pheromones[v][node.neighbour]<<endl;
				cout<<"this->smoothingLowest: "<<this->smoothingLowest<<endl;
				cout<<"pheromones[v][node.neighbour]/this->smoothingLowest: "<<pheromones[v][node.neighbour]/this->smoothingLowest<<endl;
				cout<<"log(pheromones[v][node.neighbour]/this->smoothingLowest): "<<log(pheromones[v][node.neighbour]/this->smoothingLowest)<<endl;
				cout<<"log(this->smoothingLogBase): "<<log(this->smoothingLogBase)<<endl;
			}
			if(pheromones[v][node.neighbour]) pheromones[v][node.neighbour] = this->smoothingLowest * (1 + (log(pheromones[v][node.neighbour]/this->smoothingLowest)/log(this->smoothingLogBase)) );
			if(!active[node.neighbour]){
				active[node.neighbour] = true;
				q.push(node.neighbour);
			}
		}
	}
	delete [] active;
	if(debug){
		cout<<"pheromone smoothing"<<endl;
		for(int i =0;i<this->seq.graphSize;i++){
			for(int j = 0;j<this->seq.graphSize;j++)
				cout<<this->pheromones[i][j]<<" ";
			cout<<endl;
		}
	}
}

void AntColonyOptimization::applyPheromones(Colony * colonyType)
{
	colonyType->pheremoneApplyEvent();
	bool debug = false;
	for(auto path : this->newPheromones){
		double pathPheromones = path.first;
		for(int i = 1;i<(int)path.second.size();i++){
			pheromones[path.second[i-1]][path.second[i]] += pathPheromones;
		}
	}
	this->newPheromones.clear();
	if(debug){
	cout<<"apply pheromones"<<endl;
		for(int i =0;i<this->seq.graphSize;i++){
			for(int j = 0;j<this->seq.graphSize;j++)
				cout<<this->pheromones[i][j]<<" ";
			cout<<endl;
		}
	}
}

void AntColonyOptimization::simplePath(){
	cout<<"simple path: "<<endl<<endl;
	int v_first = this->seq.firstElemIdx;	
	vertice * graph = this->seq.graph.get();	
	int v = v_first;
	vector<int>path;
	path.push_back(v);
	next_vertice:
	for(int i =1;i<=3;i++)
	for(auto it : graph[v].edges){
		if(it.val == i){
			v = it.neighbour;
			path.push_back(v);
			goto next_vertice;
		}
	}
	cout<<"expected size: "<<this->seq.graphSize<<endl;
	cout<<"path size: "<<path.size()<<endl;
	cout<<"path:"<<endl;
	for(auto it : path) cout<<it<<" ";
	cout<<endl;
}

void AntColonyOptimization::filterPheromoneTrails(Colony * colonyType){
	sort(this->newPheromones.begin(), this->newPheromones.end(), greater<pair<double,deque<int>>>());
	int nBest = (10<=this->newPheromones.size())?10:this->newPheromones.size();
	int lastBest;
	for(int i =0;i<nBest;i++){
		if(i){
			if(this->newPheromones[i-1].first - this->newPheromones[i].first > 0.1) break;
			else lastBest = i;
		}
		else lastBest = i;
	}
	auto eraseStartRange = this->newPheromones.begin() + lastBest + 1;
	// if( eraseStartRange != this->newPheromones.end()) 
	newPheromones.erase(eraseStartRange,this->newPheromones.end());
	colonyType->filterPheromoneTrailEvent(lastBest);
}
