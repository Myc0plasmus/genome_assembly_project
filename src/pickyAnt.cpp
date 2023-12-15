#include<bits/stdc++.h>
#include "pickyAnt.h"

void pickyAnt::ant(){
	// srand(time(0));
	bool firstLoopDebug = false;
	bool mergingLoopDebug = false;
	int v_first = this->seq.firstElemIdx;
	vertice * graph = this->seq.graph.get();	
	int ** adjacencyMatrix = this->seq.adjacencyMatrix;
	if(firstLoopDebug) cout<<"graphSize: "<<this->seq.graphSize<<endl;
	bool * active = new bool[this->seq.graphSize]();
	vector<deque<int>> paths;
	vector<float> roulette;
	int pathNum = 0;
	int v = v_first;
	paths.push_back(deque<int>());
	int blocker =0;
	while(v!=-1){
		// cout<<"something"<<endl;
		if(firstLoopDebug) cout<<"starting loop"<<endl;
		if(firstLoopDebug) cout<<"current v: "<<v<<endl;
		if(firstLoopDebug) cout<<"active vertices"<<endl;

		for(int i =0;i<this->seq.graphSize;i++){
			if(firstLoopDebug) cout<<i<<": "<<active[i]<<endl;
		}
		if(firstLoopDebug) cout<<endl;

		active[v] = true;
		paths[pathNum].push_back(v);
		map<int,int> beginMap;
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
				weight += pheromones[v][node.neighbour];
			}

			roulette.push_back( ((!roulette.empty())?roulette.back():0) + weight);
		}
		// if(firstLoopDebug) cout<<"roulette.back: "<<(roulette.back()<0.01)<<endl;
		if(firstLoopDebug) cout<<"roulette:"<<endl;
		for(auto it : roulette) if(firstLoopDebug) cout<<it<<" ";
		if(firstLoopDebug) cout<<endl;
		float rouletteScore = ((!roulette.empty() && roulette.back() == 0 && roulette.back()<0.01) || roulette.empty())?(-1):(rand() % (int)(roulette.back() * 100)+1);
		rouletteScore /= 100;
		if(firstLoopDebug) cout<<"roulette score: "<<rouletteScore<<endl;
		bool chooseRandom = false;
		if(rouletteScore < 0){
			v = -1;
			if(firstLoopDebug) cout<<"next node not found"<<endl;
			chooseRandom = true;
		}
		else{
			if(firstLoopDebug) cout<<"switching to next node"<<endl;
			v = graph[v].edges[findInRoulette(rouletteScore,roulette)].neighbour;
			if(beginMap.contains(v)){
				chooseRandom = true;
				for(auto node : paths.back()) paths[beginMap[v]].push_front(node);
				paths.pop_back();
			}
		}
		if(chooseRandom){
			if(firstLoopDebug) cout<<"choosing random node"<<endl;
			roulette.clear();
			if(firstLoopDebug) cout<<"available vertices"<<endl;
			for(int i =0;i<this->seq.graphSize;i++) if(!active[i]) if(firstLoopDebug) cout<<i<<endl;
			if(firstLoopDebug) cout<<endl;
			for(int i=0;i<this->seq.graphSize;i++) if(!active[i]) roulette.push_back(i);
			if(!roulette.empty()){
				v = roulette[rand() % (int)roulette.size()];
				paths.push_back(deque<int>());
				pathNum = paths.size() -1;
			}
			else v=-1;
		}
		roulette.clear();
		beginMap.clear();
		// blocker++;
		// assert(blocker < this->seq.graphSize);
		// if(blocker == 100) break;
	}
	if(mergingLoopDebug) cout<<"finished first loop"<<endl;
	if(mergingLoopDebug) cout<<"paths size: "<<paths.size()<<endl;
	if(mergingLoopDebug) cout<<"paths sizes: "<<endl;
	for(auto path : paths) if(mergingLoopDebug) cout<<path.size()<<" ";
	if(mergingLoopDebug) cout<<endl;
	delete [] active;
	active = new bool[paths.size()]();
	active[0] = true;
	// for(int i = paths.size()-1;i>=0;i--){ 
	// 	if(paths[i].size() == 1) paths.erase(paths.begin()+i);
	// }
	if(mergingLoopDebug) cout<<"paths size: "<<paths.size()<<endl;
	if(mergingLoopDebug) cout<<"paths sizes: "<<endl;
	// for(auto path : paths) if(mergingLoopDebug) cout<<path.size()<<" ";
	for(int i =0;i<(int)paths.size();i++){
		if(!i){
			if(mergingLoopDebug) cout<<"first piece: "<<graph[paths[i].back()].label<<endl;
			for(auto it : paths[i]) if(mergingLoopDebug) cout<<it<<" ";
			if(mergingLoopDebug) cout<<endl<<endl;
		}
		else{
			if(mergingLoopDebug) cout<<paths[i].front()<<": "<<graph[paths[i].front()].label<<endl;
			for(auto it : paths[i]) if(mergingLoopDebug) cout<<it<<" ";
			if(mergingLoopDebug) cout<<endl;
		}
	}
	if(mergingLoopDebug) cout<<endl;

	if(mergingLoopDebug) cout<<"connecting pieces"<<endl;
	int numOfIter = (int)paths.size();
	for(int x =1;x<=numOfIter;x++){

		vector<int> * weights = new vector<int> [this->seq.oligo_size+1]();
		if(mergingLoopDebug) cout<<"initiating merge"<<endl;
		for(int i = 1;i<(int)paths.size();i++){
			if(mergingLoopDebug) cout<<"i: "<<i<<endl;
			check_again:
			if(mergingLoopDebug) cout<<"init check"<<endl;
			if(mergingLoopDebug) cout<<"on check i: "<<i<<endl;
			if(mergingLoopDebug) cout<<"paths.size(): "<<paths.size()<<endl;
			if(mergingLoopDebug) cout<<"paths[i] size: "<<paths[i].size()<<endl;
			if(mergingLoopDebug) cout<<"paths[i].front(): "<<paths[i].front()<<endl;
			
			int score = adjacencyMatrix[paths.front().back()][paths[i].front()];
			if(mergingLoopDebug) cout<<"initial score: "<<score<<endl;
			
			if(score == 0){
				for(int cover = this->seq.cover+1;cover<=this->seq.oligo_size;cover++){
					if(graph[paths.front().back()].label.substr(cover,this->seq.oligo_size) == graph[paths[i].front()].label.substr(0,this->seq.oligo_size - cover)){
						score = cover;	
						break;
					}
				}
				if(mergingLoopDebug) cout<<"final score: "<<score<<endl;
				if(score == 10 && paths[i].size() == 1) {
					if(mergingLoopDebug) cout<<"useless path: "<<i<<endl;
					if(mergingLoopDebug) cout<<"useless paths size: "<<paths.size()<<endl;
					paths.erase(paths.begin()+i);
					if(mergingLoopDebug) cout<<"size after: "<<paths.size()<<endl;
					if(mergingLoopDebug) cout<<"deleted elem"<<endl;
					if(paths.size() == 1 || i >= (int)paths.size()) break;
					else goto check_again;
				}
				else adjacencyMatrix[paths.front().back()][paths[i].front()] = score;
			}
			weights[score].push_back(i);
			
			
		}
		vector<int>weightOrder;
		if(mergingLoopDebug) cout<<"weights:"<<endl;
		for(int i=1;i<=(int)this->seq.oligo_size;i++) if(mergingLoopDebug) cout<<weights[i].size()<<" ";
		if(mergingLoopDebug) cout<<endl;
		for(int i=1;i<=(int)this->seq.oligo_size;i++){
			if(!weights[i].empty())
			for(auto node : weights[i]){
				weightOrder.push_back(node);
				float weight = (float)(1)/(float)(i*(int)weights[i].size()); 
				if(mergingLoopDebug) cout<<"initial weight"<<weight<<endl;
				weight += pheromones[paths.front().back()][paths[node].front()];
				if(mergingLoopDebug) cout<<"added pheromones"<<endl;
				roulette.push_back( ((!roulette.empty())?roulette.back():0) + weight);
				if(mergingLoopDebug) cout<<"add roulette"<<endl;
			}
		}
		if(mergingLoopDebug) cout<<"roulette:"<<endl;
		for(auto it : roulette) if(mergingLoopDebug) cout<<it<<" ";
		if(mergingLoopDebug) cout<<endl;
		delete [] weights;
		float rouletteScore = ( (!roulette.empty() && roulette.back() == 0 && roulette.back() < 0.01) || roulette.empty())?(-1):(rand() % (int)(roulette.back() * 100)+1);
		rouletteScore /= 100;
		
		if(mergingLoopDebug) cout<<"chosen score: "<<rouletteScore<<endl;
		if(rouletteScore>0){
			v = weightOrder[findInRoulette(rouletteScore,roulette)];

			if(mergingLoopDebug) cout<<"chosen v: "<<v<<endl;
			for(auto node : paths[v]) paths.front().push_back(node);
			paths.erase(paths.begin() + v);
		}

		roulette.clear();
		if(mergingLoopDebug) cout<<"intermediary paths sizes: "<<endl;
	 	for(auto path : paths) if(mergingLoopDebug) cout<<path.size()<<" ";
		if(mergingLoopDebug) cout<<endl<<endl;
	}
	if(mergingLoopDebug) cout<<"final paths sizes: "<<endl;
	for(auto path : paths) if(mergingLoopDebug) cout<<path.size()<<" ";
	if(mergingLoopDebug) cout<<endl;
	float score = 0;
	if(mergingLoopDebug) cout<<paths.front().front()<<" ";
	for(int i = 1;i<(int)paths.front().size();i++){
		score += adjacencyMatrix[paths.front()[i-1]][paths.front()[i]];
		if(mergingLoopDebug) cout<<paths.front()[i]<<" ";
	}
	if(mergingLoopDebug) cout<<endl;
	float modelScore = this->seq.seqLen - this->seq.oligo_size;

	score = (score > modelScore)?(modelScore/score):(score/modelScore);	
	// for(int i =1;i<(int)paths.front().size();i++){
	// 	this->newPheromones[paths.front()[i-1]][paths.front()[i]] += 0.1 * score;
	// }
	if(mergingLoopDebug) cout<<"pheromone score: "<<score<<endl;
	this->newPheromones->push_back(make_pair((double)(0.1 * score),paths.front()));
	delete [] active;
}
