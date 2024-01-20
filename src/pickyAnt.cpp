#include<bits/stdc++.h>
#include "pickyAnt.h"
#include<glog/logging.h>


void pickyAnt::fillRoulette(int v, int pathNum, vector<float> & roulette, map<int,int> & beginMap,vector<bool> & active, vector<deque<int>> & paths, bool debug){
	vertice * graph = this->seq.graph.get();	
	vector<vector<int>> & adjacencyMatrix = this->seq.adjacencyMatrix;
	for(auto node : graph[v].edges){
		float weight = 0;
		bool pathBegin = false;
		//I know this looks bad, however it'll rarely run 
		if((int)paths.size() > 1){
			for(int i = 1;i<(int)paths.size();i++) if(node.neighbour == paths[i].front() && i != pathNum && adjacencyMatrix[node.neighbour][paths[i].front()] == 1 ){
				beginMap[node.neighbour] = i;
				pathBegin = true;
				break;
			}
		}
		if(!active[node.neighbour] || pathBegin){
			weight += (node.val == 1)?1:0;
			weight += pheromones->at(v).at(node.neighbour);
		}

		roulette.push_back( ((!roulette.empty())?roulette.back():0) + weight);
	}
	if(debug){
		stringstream SS;
		SS<<"roulette:"<<endl;
		for(auto it : roulette) SS<<it<<" ";
		SS<<endl;
		LOG(INFO)<<SS.str();
	}
}

float pickyAnt::getRouletteScore(vector<float> & roulette, bool debug){
		float rouletteScore = ((!roulette.empty() && roulette.back() == 0 && roulette.back()<0.01) || roulette.empty())?(-1):(rand() % (int)(roulette.back() * 100)+1);
		rouletteScore /= 100;
		LOG_IF(INFO,debug)<<"roulette score: "<<rouletteScore<<endl;
		return rouletteScore;
}

void pickyAnt::pickNextVertice(int & v, int & pathNum, vector<float> & roulette, map<int,int> & beginMap, vector<bool> & active, vector<deque<int>> & paths, bool debug){
		vertice * graph = this->seq.graph.get();	
		float rouletteScore = getRouletteScore(roulette, debug);
		bool chooseRandom = false;
		if(rouletteScore < 0){
			v = -1;
			LOG_IF(INFO,debug)<<"next node not found"<<endl;
			chooseRandom = true;
		}
		else{
			LOG_IF(INFO,debug)<<"switching to next node"<<endl;
			v = graph[v].edges[findInRoulette(rouletteScore,roulette)].neighbour;
			if(beginMap.contains(v)){
				chooseRandom = true;
				for(auto node : paths.back()) paths[beginMap[v]].push_front(node);
				paths.pop_back();
			}
		}
		if(chooseRandom){
			LOG_IF(INFO,debug)<<"choosing random node"<<endl;
			roulette.clear();
			if(debug){
				stringstream SS;
				SS<<"available vertices"<<endl;
				for(int i =0;i<this->seq.graphSize;i++) if(!active[i])  SS<<i<<endl;
				SS<<endl;
				LOG(INFO)<<SS.str();
			}
			for(int i=0;i<this->seq.graphSize;i++) if(!active[i]) roulette.push_back(i);
			if(!roulette.empty()){
				v = roulette[rand() % (int)roulette.size()];
				paths.push_back(deque<int>());
				pathNum = paths.size() -1;
			}
			else v=-1;
		}
}
void pickyAnt::debugLogPathsSize(bool debug, vector<deque<int>> & paths, string comment){
	if(debug){
		stringstream SS;
		SS<<"paths size "<<comment<<": "<<paths.size()<<endl;
		SS<<"paths sizes: "<<endl;
		for(auto path : paths) SS<<path.size()<<" ";
		SS<<endl;
		LOG(INFO)<<SS.str();
	}
}

void pickyAnt::debugLogPathsConnections(bool debug, vector<deque<int>> & paths){
	stringstream SS;
	vertice * graph = this->seq.graph.get();	
	if(debug){ 
		for(int i =0;i<(int)paths.size();i++){
			if(!i){
				SS<<"first piece: "<<graph[paths[i].back()].label<<endl;
				for(auto it : paths[i])  SS<<it<<" ";
				SS<<endl<<endl;
				
			}
			else{
				cout<<paths[i].front()<<": "<<graph[paths[i].front()].label<<endl;
				for(auto it : paths[i])  cout<<it<<" ";
				 cout<<endl;
			}
		}
		SS<<endl;
		LOG(INFO)<<SS.str();
	}
}

void pickyAnt::fillFragmentWeights(vector<vector<int>> & weights,vector<deque<int>> & paths, bool debug){
	LOG_IF(INFO, debug)<<"filling weights of the fragments' beginnings:"<<endl;
	vertice * graph = this->seq.graph.get();	
	vector<vector<int>> & adjacencyMatrix = this->seq.adjacencyMatrix;
	for(int i = 1;i<(int)paths.size();i++){
		LOG_IF(INFO,debug)<<"i: "<<i<<endl;
		check_again:
		if(debug){
			stringstream SS;
			SS<<"init check"<<endl;
			SS<<"on check i: "<<i<<endl;
			SS<<"paths.size(): "<<paths.size()<<endl;
			SS<<"paths[i] size: "<<paths[i].size()<<endl;
			SS<<"paths[i].front(): "<<paths[i].front()<<endl;
			LOG(INFO)<<SS.str();
		}	
		int score = adjacencyMatrix[paths.front().back()][paths[i].front()];
		LOG_IF(INFO,debug)<<"initial score: "<<score<<endl;
		
		if(score == 0){
			for(int cover = this->seq.cover+1;cover<=this->seq.oligo_size;cover++){
				if(graph[paths.front().back()].label.substr(cover,this->seq.oligo_size) == graph[paths[i].front()].label.substr(0,this->seq.oligo_size - cover)){
					score = cover;	
					break;
				}
			}
			LOG_IF(INFO,debug)<<"final score: "<<score<<endl;
			if(score == 10 && paths[i].size() == 1) {
				LOG_IF(INFO,debug)<<"useless path: "<<i<<endl;
				LOG_IF(INFO,debug)<<"useless paths size: "<<paths.size()<<endl;
				paths.erase(paths.begin()+i);
				LOG_IF(INFO,debug)<<"size after: "<<paths.size()<<endl;
				LOG_IF(INFO,debug)<<"deleted elem"<<endl;
				if(paths.size() == 1 || i >= (int)paths.size()) break;
				else goto check_again;
			}
			else adjacencyMatrix[paths.front().back()][paths[i].front()] = score;
		}
		weights[score].push_back(i);
		
		
	}
}


void pickyAnt::fillWeightRoulette(vector<float> & roulette, vector<vector<int>> & weights, vector<int> & weightOrder, vector<deque<int>> & paths, bool debug){

	LOG_IF(INFO,debug)<<"initiating merge"<<endl;
	for(int i=1;i<=(int)this->seq.oligo_size;i++){
			if(!weights[i].empty())
			for(auto node : weights[i]){
				weightOrder.push_back(node);
				float weight = (float)(1)/(float)(i*(int)weights[i].size()); 
				LOG_IF(INFO,debug)<<"initial weight"<<weight<<endl;
				weight += pheromones->at(paths.front().back()).at(paths[node].front());
				LOG_IF(INFO,debug)<<"added pheromones"<<endl;
				roulette.push_back( ((!roulette.empty())?roulette.back():0) + weight);
				LOG_IF(INFO,debug)<<"add roulette"<<endl;
			}
		}
		if(debug){
			stringstream SS;
			SS<<"roulette:"<<endl;
			for(auto it : roulette)  SS<<it<<" ";
			SS<<endl;
			LOG(INFO)<<SS.str();
		}
}

void pickyAnt::ant(){
	bool firstLoopDebug = false;
	bool mergingLoopDebug = false;
	LOG_IF(INFO,firstLoopDebug)<<"commencing picky ant"<<endl;
	int v_first = this->seq.firstElemIdx;
	vertice * graph = this->seq.graph.get();	
	vector<vector<int>> & adjacencyMatrix = this->seq.adjacencyMatrix;
	LOG_IF(INFO,firstLoopDebug)<<"graphSize: "<<this->seq.graphSize<<endl;
	vector<bool> active(this->seq.graphSize,0);
	vector<deque<int>> paths;
	vector<float> roulette;
	int pathNum = 0;
	int v = v_first;
	paths.push_back(deque<int>());
	int blocker =0;
	while(v!=-1){
		// cout<<"something"<<endl;
		if(firstLoopDebug){
			stringstream SS;
			SS<<"starting loop"<<endl;
			SS<<"current v: "<<v<<endl;
			SS<<"active vertices"<<endl;
			for(int i =0;i<this->seq.graphSize;i++){
				 SS<<i<<": "<<active[i]<<endl;
			}
			SS<<endl;
			LOG(INFO)<<SS.str();
		}


		active[v] = true;
		paths[pathNum].push_back(v);
		map<int,int> beginMap;


		this->fillRoulette(v,pathNum,roulette,beginMap,active,paths,firstLoopDebug);
		
		this->pickNextVertice(v,pathNum,roulette,beginMap,active,paths,firstLoopDebug);

		roulette.clear();
		beginMap.clear();

		
		// blocker++;
		// assert(blocker < this->seq.graphSize);
		// if(blocker == 100) break;
	}
	LOG_IF(INFO,firstLoopDebug)<<"finished first loop"<<endl;
	this->debugLogPathsSize(mergingLoopDebug,paths);
	
	active.assign(paths.size(),0);
	active[0] = true;
	this->debugLogPathsConnections(mergingLoopDebug, paths);

	LOG_IF(INFO,mergingLoopDebug)<<"connecting pieces"<<endl;
	int numOfIter = (int)paths.size();
	for(int x =1;x<=numOfIter;x++){

		vector<vector<int>> weights(this->seq.oligo_size+1,vector<int>());

		this->fillFragmentWeights(weights,paths, mergingLoopDebug);
		vector<int>weightOrder;

		if(mergingLoopDebug){
			stringstream SS;
			SS<<"weights:"<<endl;
			for(int i=1;i<=(int)this->seq.oligo_size;i++) SS<<weights[i].size()<<" ";
			SS<<endl;
			LOG(INFO)<<SS.str();
		}

		this->fillWeightRoulette(roulette,weights,weightOrder,paths,mergingLoopDebug);	

		weights.assign(this->seq.oligo_size+1,vector<int>());
		float rouletteScore = getRouletteScore(roulette,mergingLoopDebug);
		
		LOG_IF(INFO,mergingLoopDebug)<<"chosen score: "<<rouletteScore<<endl;
		if(rouletteScore>0){
			v = weightOrder[findInRoulette(rouletteScore,roulette)];

			LOG_IF(INFO,mergingLoopDebug)<<"chosen v: "<<v<<endl;
			for(auto node : paths[v]) paths.front().push_back(node);
			paths.erase(paths.begin() + v);
		}

		roulette.clear();
		debugLogPathsSize(mergingLoopDebug,paths,"intermediate");
		LOG_IF(INFO,mergingLoopDebug)<<endl;
	}

	debugLogPathsSize(mergingLoopDebug,paths,"final");
	float score = 0;
	LOG_IF(INFO,mergingLoopDebug)<<paths.front().front()<<" ";
	for(int i = 1;i<(int)paths.front().size();i++){
		score += pow(adjacencyMatrix[paths.front()[i-1]][paths.front()[i]],2);
		LOG_IF(INFO,mergingLoopDebug)<<paths.front()[i]<<" ";
	}
	LOG_IF(INFO,mergingLoopDebug)<<endl;
	float modelScore = this->seq.seqLen - this->seq.oligo_size;
	float modelPathLength = this->seq.seqLen - this->seq.oligo_size + 1 ;

	score = (score > modelScore)?(modelScore/score):(score/modelScore);	
	score *= ((int)paths.front().size() > modelPathLength)?pow(modelPathLength/(float)(paths.front().size()),2):((float)(paths.front().size())/modelPathLength);

	LOG_IF(INFO,mergingLoopDebug)<<"pheromone score: "<<score<<endl;
	this->newPheromones->push_back(make_pair((double)(0.1 * score),paths.front()));
}

