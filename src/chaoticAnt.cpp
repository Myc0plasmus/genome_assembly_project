#include<bits/stdc++.h>
#include "chaoticAnt.h"
#include "edge.h"
#include<glog/logging.h>


chaoticAnt::chaoticAnt(Sequence & newSeq) : Colony(newSeq){
	endPheromones = new long double[newSeq.graphSize]();
	chaosPheromones = new long double[newSeq.graphSize]();
}

chaoticAnt::chaoticAnt(Sequence & newSeq, vector<vector<long double>> * linkedPheromones, vector<pair<long double,deque<int>>> * linkedNewPheromones) : Colony(newSeq,linkedPheromones,linkedNewPheromones){
	endPheromones = new long double[newSeq.graphSize]();
	chaosPheromones = new long double[newSeq.graphSize]();
}

chaoticAnt::~chaoticAnt() {
	delete [] endPheromones;
	delete [] chaosPheromones;
}

void chaoticAnt::pheremoneApplyEvent(bool debug){
	for(auto it : this->newEndPheromones){
		this->endPheromones[it.second]+=it.first;
	}
	this->newEndPheromones.clear();
	for(auto path : this->newChaosPheromones){
		for(auto it : path.second){
			chaosPheromones[it] += path.first;
		}
	}
	this->newChaosPheromones.clear();
}

void chaoticAnt::pheromoneEvaporationEvent(bool debug){
	for(int i =0;i<this->seq.graphSize;i++){
		// this->chaosPheromones[i] -= (this->chaosPheromones[i] == 0)?0:0.001;
		if(this->chaosPheromones[i] != 0){
			if(this->chaosPheromones[i] <= 0.001 || this->chaosPheromones[i] - 0.001 < 0.001 ){ 
				chaosPheromones[i] = 0;
			}
			else{
				chaosPheromones[i] -= 0.001;
			}
		}
		// this->endPheromones[i] -= (this->endPheromones[i] == 0)?0:0.001;
		if(this->endPheromones[i] != 0){
			if(this->endPheromones[i] <= 0.001 || this->endPheromones[i] - 0.001 < 0.001 ){ 
				endPheromones[i] = 0;
			}
			else{
				endPheromones[i] -= 0.001;
			}
		}
	}
}

void chaoticAnt::debugLogActive(bool * active, bool debug){
	if(debug){
		// stringstream SS;
		// cout<<"active vertices"<<endl;
		// for(int i =0;i<this->seq.graphSize;i++){
		// 	 cout<<i<<": "<<active[i]<<endl;
		// }
		cout<<endl;
		int activeNum = 0;
		for(int i =0;i<this->seq.graphSize;i++){
			activeNum += active[i];
		}
		LOG(INFO)<<"active vertices: "<<activeNum<<" out of "<<this->seq.graphSize<<endl;
	}
}

void chaoticAnt::filterPheromoneTrailEvent(int lastBest,bool debug){
	sort(this->newEndPheromones.begin(), this->newEndPheromones.end(),greater<pair<double,int>>());
	sort(this->newChaosPheromones.begin(), this->newChaosPheromones.end(),greater<pair<double,deque<int>>>());
	auto eraseChaosStartRange = this->newChaosPheromones.begin() + lastBest + 1;
	auto eraseEndStartRange = this->newEndPheromones.begin() + lastBest + 1;
	// if( eraseStartRange != this->newPheromones.end()) 
	// cout<<"before erase"<<endl;
	this->newEndPheromones.erase(eraseEndStartRange,this->newEndPheromones.end());
	this->newChaosPheromones.erase(eraseChaosStartRange,this->newChaosPheromones.end());
}

float chaoticAnt::getRouletteScore(vector<float> & roulette, bool debug){
		LOG_IF(INFO,debug)<<"Getting roulette score:";
		LOG_IF(INFO,debug)<<"roulette size: "<<roulette.size();
		LOG_IF(INFO,debug)<<"roulette back: "<<roulette.back();
		float rouletteScore = ((!roulette.empty() && roulette.back() == 0 && roulette.back()<0.001) || roulette.empty())?(-1):(rand() % (int)(roulette.back() * 1000)+1);
		rouletteScore /= 1000;
		LOG_IF(INFO,debug)<<"roulette score: "<<rouletteScore<<endl;
		return rouletteScore;
}

void chaoticAnt::fillWeights(int v, vector<vector<int>> & weights, vector<bool> & active, int & minScore,bool debug){

	vertice * graph = this->seq.graph.get();	
	int nodeIdx =0;
	LOG_IF(INFO,debug)<<"filling weights"<<endl;
	for(auto node : graph[v].edges){
		if(!active[node.neighbour]){
			if(node.val < minScore) minScore = node.val;
			weights[node.val].push_back(nodeIdx);
		}
		nodeIdx++;
	}
}

void chaoticAnt::fillRoulette(int v, vector<float> & roulette, vector<vector<int>> & weights, bool debug){
	LOG_IF(INFO,debug)<<"filling roulette"<<endl;
		for(int i =1;i<=this->seq.oligo_size;i++){
			for(auto node : weights[i]){
				float weight = ((!roulette.empty())?roulette.back():0);
				weight += ((float)(1)/(float)(i*i*weights[i].size()));
				weight += pheromones->at(v).at(node);
				roulette.push_back(weight);
			}
		}
}

void chaoticAnt::fillOnesRoulette(int v, int & minScore, vector<float> & roulette,vector<bool> &active, bool debug){
	LOG_IF(INFO, debug)<<"Starting to fill roulette";
	vertice * graph = this->seq.graph.get();	
	if(graph[v].edges.empty()) return;
	minScore = graph[v].edges.front().val;
	for(auto node : graph[v].edges){
		if(node.val != minScore) break;
		else if(!active[node.neighbour]) roulette.push_back(((!roulette.empty())?roulette.back():0)+1);
	}
}

void chaoticAnt::pickNextVertice(int & v,int minScore, vector<float> & roulette, vector<vector<int>> & weights, deque<int> & randJump, vector<bool> & active, bool debug){
	vertice * graph = this->seq.graph.get();	
	vector<vector<int>> & adjacencyMatrix = this->seq.adjacencyMatrix;
	float rouletteScore = this->getRouletteScore(roulette, debug);
	bool chooseRandom = false;
	LOG_IF(WARNING,rouletteScore > roulette.back())<<"Roulette score "<<rouletteScore<<" is bigger than roulette range "<<roulette.back();
	if(rouletteScore > roulette[(int)roulette.size()- 2] ){
		v = -1;
		
		LOG_IF(INFO,debug)<<"next node not found"<<endl;
	}
	else if(roulette.size() <= 2 || rouletteScore > roulette[(int)roulette.size()-3]){
	
		LOG_IF(INFO,debug)<<"choosing random node"<<endl;
		
		roulette.clear();
		
		if(debug){
			stringstream SS;
			SS<<"available vertices"<<endl;
			for(int i =0;i<this->seq.graphSize;i++) if(!active[i]) SS<<i<<endl;
			SS<<endl;
			LOG(INFO)<<SS.str();
		}
		for(int i=0;i<this->seq.graphSize;i++) if(!active[i]) roulette.push_back(i);
		if(!roulette.empty()){
			randJump.push_back(v);
			LOG_IF(INFO,debug)<<"before new v"<<endl;
			v = roulette[rand() % (int)roulette.size()];
			if(!adjacencyMatrix[randJump.back()][v]){
				for(int cover = this->seq.cover+1;cover<=this->seq.oligo_size;cover++){
					if(graph[randJump.back()].label.substr(cover,this->seq.oligo_size) == graph[v].label.substr(0,this->seq.oligo_size - cover)){
									adjacencyMatrix[randJump.back()][v] = cover;	
									break;
					}
				}
				LOG_IF(INFO,debug)<<"before adding new edge"<<endl;
				graph[randJump.back()].edges.push_back(edge(adjacencyMatrix[randJump.back()][v],v));
				sort(graph[randJump.back()].edges.begin(), graph[randJump.back()].edges.end());

			}
		}
		else{
			if(debug)cout<<"there is no next node"<<endl;
			v=-1;
		}
	}
	else{
		LOG_IF(INFO,debug)<<"switching to next node"<<endl;
		LOG_IF(INFO,debug)<<"v edges size: "<<graph[v].edges.size()<<endl;
		int res = findInRoulette(rouletteScore,roulette);
		LOG_IF(INFO,debug)<<"res: "<<res<<endl;
		v = graph[v].edges[res].neighbour;
	}
}


void chaoticAnt::ant(){
	// srand(time(0));
	bool firstLoopDebug = false;
	bool mergingLoopDebug = false;
	int v_first = this->seq.firstElemIdx;
	vertice * graph = this->seq.graph.get();	
	vector<vector<int>> & adjacencyMatrix = this->seq.adjacencyMatrix;
	LOG_IF(INFO,firstLoopDebug)<<"commencing chaotic ant"<<endl;
	LOG_IF(INFO,firstLoopDebug)<<"graphSize: "<<this->seq.graphSize<<endl;
	vector<bool> active(this->seq.graphSize,0);
	deque<int> path;
	deque<int> randJump;
	vector<float> roulette;
	int v = v_first;
	int blocker =0;
	float modelScore = this->seq.seqLen - this->seq.oligo_size;
	float pathScoreSum =0;
	float pathScore;
	float modelPathLength = this->seq.seqLen - this->seq.oligo_size + 1 ;
	float pathLengthScore;
	while(v!=-1){
		if(firstLoopDebug){	
			LOG(INFO)<<"starting loop"<<endl;
			LOG(INFO)<<"current v: "<<v<<endl;
		}

		active[v] = true;
		path.push_back(v);

		LOG_IF(INFO,firstLoopDebug)<<"updating path score"<<endl;
		if(v != v_first){
			LOG_IF(WARNING, 2 > (int)path.size() && firstLoopDebug)<<"Paths size "<<path.size()<<" is smaller than 2";
			pathScoreSum += pow(adjacencyMatrix[*(path.rbegin()+1)][path.back()],2);
			pathScore = (pathScoreSum > modelScore)?(modelScore/pathScoreSum):(pathScoreSum/modelScore);
		}
		LOG_IF(INFO,firstLoopDebug)<<"creating weights and variables"<<endl;
		vector<vector<int>> weights(this->seq.oligo_size+1,vector<int>());
		for(int i=1;i<this->seq.oligo_size;i++) weights[i] = vector<int>();
		
		int minScore = this->seq.oligo_size;
		// this->fillWeights(v,weights,active,minScore,firstLoopDebug)	;

		this->fillOnesRoulette(v,minScore,roulette,active,firstLoopDebug);
		
		LOG_IF(INFO,firstLoopDebug)<<"went past filling roulette"<<endl;
		//probability of finishing gets higher with the score
		pathLengthScore = ((int)path.size() > modelPathLength)?pow(modelPathLength/(float)(path.size()),2):((float)(path.size())/modelPathLength);
		float refinedPathScore = pathScore * pathLengthScore;
		float endProb = (pathLengthScore > 0.9)?((pathLengthScore - 0.9)*10):0;
		//multiplying by probability of inverse prob ensures that sum of probabilities is less than one
		float chaosProb = (1 - endProb) * ((float)(minScore)/(float)(1000));
		float s = (!roulette.empty() && endProb!=1)?(roulette.back() / (1 - (endProb+chaosProb))):1;
		roulette.push_back(((!roulette.empty())?roulette.back():0) + (chaosProb*s) + this->chaosPheromones[v]);
		roulette.push_back(((!roulette.empty())?roulette.back():0) + (endProb*s) + this->endPheromones[v]);
		
		if(firstLoopDebug){
			stringstream SS;
			SS<<"roulette:"<<endl;
			for(auto it : roulette) SS<<it<<" ";
			SS<<endl;
			LOG(INFO)<<SS.str();
		}
		
		this->pickNextVertice(v,minScore,roulette,weights,randJump,active, firstLoopDebug);
		

		roulette.clear();
		weights.assign(this->seq.oligo_size+1,vector<int>());
		LOG_IF(INFO,firstLoopDebug)<<endl;
		// blocker++;
		// assert(blocker < this->seq.graphSize);
		// if(blocker == 100) break;
	}
	// for(int i =1;i<(int)paths.front().size();i++){
	// 	this->newPheromones[paths.front()[i-1]][paths.front()[i]] += 0.1 * score;
	// })
	pathScore *= pathLengthScore;
	
	LOG_IF(INFO,mergingLoopDebug)<<"pheromone score: "<<pathScore<<endl<<endl<<endl;
	// if(pathScore < 0.01) exit(1);
	this->newPheromones->push_back(make_pair((long double)(0.1 * pathScore),path));
	this->newChaosPheromones.push_back(make_pair((long double)(0.01 * pathScore),randJump));
	this->newEndPheromones.push_back(make_pair((long double)(0.01 * pathScore),path.back()));
}
