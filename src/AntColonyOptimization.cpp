#include<bits/stdc++.h>
#include "AntColonyOptimization.h"
#include "Sequence.h"
#include<glog/logging.h>

using namespace std;

static int NUM_OF_ANTS = 50;
static int SMOOTHING_LOG_BASE = 3;
static double SMOOTHING_LOWEST =  0.01;
static double EVAPORATION_RATE = 0.01;
static double STOP_TIME = 5;

AntColonyOptimization::AntColonyOptimization(Sequence & newSeq) : seq(newSeq){
	this->evaporationRate = EVAPORATION_RATE; 
	this->smoothingLowest = SMOOTHING_LOWEST;
	this->smoothingLogBase = SMOOTHING_LOG_BASE;
	this->numOfAnts = NUM_OF_ANTS;
	this->stopTime = STOP_TIME;
	this->attunePheromones();
	// LOG(INFO)<<"pheromone size pre: "<<pheromones.size();	
	// for(int i =0;i<this->seq.graphSize;i++){
	// 	this->pheromones[i].reserve(this->seq.graphSize);
	// 	fill(this->pheromones[i].begin(),this->pheromones[i].end(),0);
	// } 
	// LOG(INFO)<<"pheromone size past: "<<pheromones.size();	
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

}
void AntColonyOptimization::resetEssentialParts(){
	// if(this->seq.graphSize + 10 != (int)this->pheromones.size()){
	// 	LOG(INFO)<<"Pheromones needed attuning:"<<endl<<"previous size: "<<pheromones.size()<<endl<<"expected size: "<<(this->seq.graphSize+10);
	// 	this->attunePheromones();
	// }
	// for(int i =0;i<this->seq.graphSize;i++) fill(this->pheromones[i].begin(),this->pheromones[i].end(),0);
	this->attunePheromones();
	this->resetSequence();
}
int AntColonyOptimization::getNumOfAnts(){
	return this->numOfAnts;
}
void AntColonyOptimization::resetSequence(){
	// this->seq.shredSequence();
	if(!this->seq.hasSeq) return;
	this->seq.clearGraph();
	this->seq.createGraphWithFixedCover();
	
}

void AntColonyOptimization::attunePheromones(){
		this->pheromones.reserve(this->seq.graphSize+10);
		this->pheromones.assign(this->seq.graphSize+10, vector<long double>(this->seq.graphSize+10,0));
}

void AntColonyOptimization::setNumOfAnts(int newNumOfAnts){
	this->numOfAnts = newNumOfAnts;
	this->resetEssentialParts();
}

void AntColonyOptimization::resetNumOfAnts(){
	this->numOfAnts = NUM_OF_ANTS;
}

void AntColonyOptimization::setEvaporationRate(double newEvaporationRate){
	this->evaporationRate = newEvaporationRate;
	this->resetEssentialParts();
}
double AntColonyOptimization::getEvaporationRate(){
	return this->evaporationRate;
}
void AntColonyOptimization::resetEvaporationRate(){
	this->evaporationRate = EVAPORATION_RATE;
}

int AntColonyOptimization::getSmoothingLogBase(){
	return this->smoothingLogBase;
}
double AntColonyOptimization::getSmoothingLowest(){
	return this->smoothingLogBase;
}
void AntColonyOptimization::setSmoothingLogBase(int newSmoothingLogBase){
	this->smoothingLogBase = newSmoothingLogBase;
	this->resetEssentialParts();

}

void AntColonyOptimization::resetSmoothingLogBase(){
	this->smoothingLogBase = SMOOTHING_LOG_BASE;
}

void AntColonyOptimization::setSmoothingLowest(double newSmoothingLowest){
	this->smoothingLowest = newSmoothingLowest;
	this->resetEssentialParts();
}

void AntColonyOptimization::resetSmoothingLowest(){
	this->smoothingLowest = SMOOTHING_LOG_BASE;
}


void AntColonyOptimization::pheromoneEvaporation(Colony * colonyType){
	bool debug = false;
	colonyType->pheromoneEvaporationEvent(debug);
	vertice * graph = this->seq.graph.get();
	int v = this->seq.firstElemIdx;
	bool * active = new bool[this->seq.graphSize]();
	queue<int> q;
	q.push(v);
	while(!q.empty()){
		v = q.front();
		q.pop();
		// LOG(WARNING)<<"loop start";
		for(auto node : graph[v].edges){
			// LOG(WARNING)<<"v: "<<v;
			// LOG(WARNING)<<"node.neighbour: "<<node.neighbour;
			// LOG(WARNING)<<"graphSize: "<<this->seq.graphSize;
			LOG_IF(WARNING,v >= (int)this->pheromones.size())<<"evaporation: v:"<<v<<" is bigger than pheromone size: "<<pheromones.size();
			LOG_IF(WARNING,node.neighbour >= (int)this->pheromones.size())<<"evaporation: node neighbour:"<<node.neighbour<<" is bigger than pheromone size: "<<pheromones.size();
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
	bool debug = false;
	colonyType->pheremoneApplyEvent(debug);
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
	bool debug = false;
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
	colonyType->filterPheromoneTrailEvent(lastBest,debug);
}
