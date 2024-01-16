#include<bits/stdc++.h>
#include "chaoticAnt.h"
#include "edge.h"


chaoticAnt::chaoticAnt(Sequence & newSeq) : Colony(newSeq){
	endPheromones = new long double[newSeq.graphSize]();
	chaosPheromones = new long double[newSeq.graphSize]();
}

chaoticAnt::chaoticAnt(Sequence & newSeq, long double ** linkedPheromones, vector<pair<long double,deque<int>>> * linkedNewPheromones) : Colony(newSeq,linkedPheromones,linkedNewPheromones){
	endPheromones = new long double[newSeq.graphSize]();
	chaosPheromones = new long double[newSeq.graphSize]();
}

chaoticAnt::~chaoticAnt() {
	delete [] endPheromones;
	delete [] chaosPheromones;
}

void chaoticAnt::pheremoneApplyEvent(){
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

void chaoticAnt::pheromoneEvaporationEvent(){
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

void chaoticAnt::filterPheromoneTrailEvent(int lastBest){
	sort(this->newEndPheromones.begin(), this->newEndPheromones.end(),greater<pair<double,int>>());
	sort(this->newChaosPheromones.begin(), this->newChaosPheromones.end(),greater<pair<double,deque<int>>>());
	auto eraseChaosStartRange = this->newChaosPheromones.begin() + lastBest + 1;
	auto eraseEndStartRange = this->newEndPheromones.begin() + lastBest + 1;
	// if( eraseStartRange != this->newPheromones.end()) 
	// cout<<"before erase"<<endl;
	this->newEndPheromones.erase(eraseEndStartRange,this->newEndPheromones.end());
	this->newChaosPheromones.erase(eraseChaosStartRange,this->newChaosPheromones.end());
}

void chaoticAnt::ant(){
	// srand(time(0));
	bool firstLoopDebug = false;
	bool mergingLoopDebug = false;
	int v_first = this->seq.firstElemIdx;
	vertice * graph = this->seq.graph.get();	
	int ** adjacencyMatrix = this->seq.adjacencyMatrix;
	if(firstLoopDebug) cout<<"graphSize: "<<this->seq.graphSize<<endl;
	bool * active = new bool[this->seq.graphSize]();
	deque<int> path;
	deque<int> randJump;
	vector<float> roulette;
	int v = v_first;
	int blocker =0;
	float modelScore = this->seq.seqLen - this->seq.oligo_size;
	float pathScoreSum =0;
	float pathScore;
	while(v!=-1){
		// cout<<"something"<<endl;
		if(firstLoopDebug){
			cout<<"starting loop"<<endl;
			cout<<"current v: "<<v<<endl;
			// cout<<"active vertices"<<endl;
			// for(int i =0;i<this->seq.graphSize;i++){
			// 	 cout<<i<<": "<<active[i]<<endl;
			// }
			// cout<<endl;
			int activeNum = 0;
			for(int i =0;i<this->seq.graphSize;i++){
				activeNum += active[i];
			}
			cout<<"active vertices: "<<activeNum<<endl;
		}

		active[v] = true;
		path.push_back(v);
		if(firstLoopDebug) cout<<"updating path score"<<endl;
		if(v != v_first){
			pathScoreSum += pow(adjacencyMatrix[*(path.rbegin()+1)][path.back()],2);
			pathScore = (pathScoreSum > modelScore)?(modelScore/pathScoreSum):(pathScoreSum/modelScore);
			// if(pathScore > 1){
			// 	cout<<"pathScoreSum: "<<pathScoreSum<<endl;
			// 	cout<<"modelScore: "<<modelScore<<endl;
			// 	exit(1);
			// }
		}
		if(firstLoopDebug) cout<<"creating weights and variables"<<endl;
		vector<int> * weights = new vector<int>[this->seq.oligo_size+1];
		for(int i=1;i<this->seq.oligo_size;i++) weights[i] = vector<int>();
		int minScore = this->seq.oligo_size;
		int nodeIdx =0;
		if(firstLoopDebug) cout<<"filling weights"<<endl;
		for(auto node : graph[v].edges){
			if(!active[node.neighbour]){
				if(node.val < minScore) minScore = node.val;
				weights[node.val].push_back(nodeIdx);
			}
			nodeIdx++;
		}
		if(firstLoopDebug) cout<<"filling roulette"<<endl;
		for(int i =1;i<=this->seq.oligo_size;i++){
			for(auto node : weights[i]){
				float weight = ((!roulette.empty())?roulette.back():0);
				weight += ((float)(1)/(float)(i*i*weights[i].size()));
				weight += pheromones[v][node];
				roulette.push_back(weight);
			}
		}
		if(firstLoopDebug) cout<<"went past filling roulette"<<endl;
		//probability of finishing gets higher with the score
		float refinedPathScore = pathScore * (float)(path.size())/(float)(this->seq.graphSize);
		float endProb = (refinedPathScore > 0.9)?((refinedPathScore - 0.9)*10):0;
		//multiplying by probability of inverse prob ensures that sum of probabilities is less than one
		float chaosProb = (1 - endProb) * ((float)(minScore)/(float)(50));
		float s = (!roulette.empty() && endProb!=1)?(roulette.back() / (1 - (endProb+chaosProb))):1;
		roulette.push_back(((!roulette.empty())?roulette.back():0) + (chaosProb*s) + this->chaosPheromones[v]);
		// if(roulette.back()==0)
			// cout<<"special chaos:"<<endl<<"chaosProb: "<<chaosProb<<endl<<"endProb: "<<endProb<<endl<<"s: "<<s<<endl;
		roulette.push_back(((!roulette.empty())?roulette.back():0) + (endProb*s) + this->endPheromones[v]);
		
		//
		//add chaos and end pheromones
		//
		if(firstLoopDebug){
			cout<<"roulette:"<<endl;
			for(auto it : roulette) cout<<it<<" ";
			cout<<endl;
		}
		float rouletteScore = ((!roulette.empty() && roulette.back() == 0 && roulette.back()<0.01) || roulette.empty())?(-1):(rand() % (int)(roulette.back() * 100)+1);
		rouletteScore /= 100;
		if(firstLoopDebug) cout<<"roulette score: "<<rouletteScore<<endl;
		bool chooseRandom = false;
		if(rouletteScore > roulette[(int)roulette.size()- 2] ){
			v = -1;
			
			if(firstLoopDebug) cout<<"next node not found"<<endl;
		}
		else if(roulette.size() <= 2 || rouletteScore > roulette[(int)roulette.size()-3]){
			if(firstLoopDebug) cout<<"choosing random node"<<endl;
			roulette.clear();
			if(firstLoopDebug){
				cout<<"available vertices"<<endl;
				for(int i =0;i<this->seq.graphSize;i++) if(!active[i]) cout<<i<<endl;
				cout<<endl;
			}
			for(int i=0;i<this->seq.graphSize;i++) if(!active[i]) roulette.push_back(i);
			if(!roulette.empty()){
				randJump.push_back(v);
				if(firstLoopDebug) cout<<"before new v"<<endl;
				v = roulette[rand() % (int)roulette.size()];
				if(!adjacencyMatrix[randJump.back()][v]){
					for(int cover = this->seq.cover+1;cover<=this->seq.oligo_size;cover++){
						if(graph[randJump.back()].label.substr(cover,this->seq.oligo_size) == graph[v].label.substr(0,this->seq.oligo_size - cover)){
										adjacencyMatrix[randJump.back()][v] = cover;	
										break;
						}
					}
					if(firstLoopDebug) cout<<"before adding new edge"<<endl;
					graph[randJump.back()].edges.push_back(edge(adjacencyMatrix[randJump.back()][v],v));
				}
			}
			else{
				if(firstLoopDebug)cout<<"there is no next node"<<endl;
				v=-1;
			}
		}
		else{
			if(firstLoopDebug) cout<<"switching to next node"<<endl;
			if(firstLoopDebug) cout<<"v edges size: "<<graph[v].edges.size()<<endl;
			int res = findInWeightedRoulette(rouletteScore,roulette,weights);
			if(firstLoopDebug) cout<<"res: "<<res<<endl;
			v = graph[v].edges[res].neighbour;
		}

		roulette.clear();
		delete [] weights;
		if(firstLoopDebug) cout<<endl;
		// blocker++;
		// assert(blocker < this->seq.graphSize);
		// if(blocker == 100) break;
	}
	// for(int i =1;i<(int)paths.front().size();i++){
	// 	this->newPheromones[paths.front()[i-1]][paths.front()[i]] += 0.1 * score;
	// })
	pathScore *= (float)(path.size())/(float)(this->seq.graphSize);
	
	if(mergingLoopDebug) cout<<"pheromone score: "<<pathScore<<endl<<endl<<endl;
	// if(pathScore < 0.01) exit(1);
	this->newPheromones->push_back(make_pair((double)(0.1 * pathScore),path));
	this->newChaosPheromones.push_back(make_pair((double)(0.01 * pathScore),randJump));
	this->newEndPheromones.push_back(make_pair((double)(0.01 * pathScore),path.back()));
	delete [] active;
}
