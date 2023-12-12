#include<bits/stdc++.h>
#include "AntColonyOptimization.h"
#include "Sequence.h"

using namespace std;

AntColonyOptimization::AntColonyOptimization(Sequence * newSeq){
	this->seq = newSeq;
	pheromones = new double* [this->seq->graphSize];
	for(int i =0;i<this->seq->graphSize;i++) pheromones[i] = new double [this->seq->graphSize]();
	newPheromones = new double* [this->seq->graphSize];
	for(int i =0;i<this->seq->graphSize;i++) newPheromones[i] = new double [this->seq->graphSize]();
	// for(int i =0;i<this->seq->graphSize;i++){
	// 	for(int j=0;j<this->seq->graphSize;j++) cout<<pheromones[i][j]<<" ";
	// 	cout<<endl;
	// }
		
}

AntColonyOptimization::~AntColonyOptimization(){
	for(int i =0;i<this->seq->graphSize;i++) delete [] pheromones[i];
	delete [] pheromones;
	for(int i =0;i<this->seq->graphSize;i++) delete [] newPheromones[i];
	delete [] newPheromones;
}

void AntColonyOptimization::ant(){
	// srand(time(0));
	int v_first = this->seq->firstElemIdx;
	vertice * graph = this->seq->graph.get();	
	int ** adjacencyMatrix = this->seq->adjacencyMatrix;
	// cout<<"graphSize: "<<this->seq->graphSize<<endl;
	bool * active = new bool[this->seq->graphSize]();
	vector<deque<int>> paths;
	vector<float> roulette;
	int pathNum = 0;
	int v = v_first;
	paths.push_back(deque<int>());
	int blocker =0;
	while(v!=-1){
		// cout<<"starting loop"<<endl;
		// cout<<"current v: "<<v<<endl;
		// cout<<"active vertices"<<endl;
		// 
		// for(int i =0;i<this->seq->graphSize;i++){
		// 	cout<<i<<": "<<active[i]<<endl;
		// }
		// cout<<endl;

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
		// cout<<"roulette.back: "<<(roulette.back()<0.01)<<endl;
		// cout<<"roulette:"<<endl;
		// for(auto it : roulette) cout<<it<<" ";
		// cout<<endl;
		float rouletteScore = ((!roulette.empty() && roulette.back() == 0 && roulette.back()<0.01) || roulette.empty())?(-1):(rand() % (int)(roulette.back() * 100)+1);
		rouletteScore /= 100;
		// cout<<"roulette score: "<<rouletteScore<<endl;
		bool chooseRandom = false;
		if(rouletteScore < 0){
			v = -1;
			// cout<<"next node not found"<<endl;
			chooseRandom = true;
		}
		else{
			// cout<<"switching to next node"<<endl;
			for(int i =0;i<(int)roulette.size();i++){ 
				if((!i || (i && rouletteScore > roulette[i-1])) && rouletteScore <= roulette[i]) 
				{
					v = graph[v].edges[i].neighbour;
					break;
				}
			}
			if(beginMap.contains(v)){
				chooseRandom = true;
				for(auto node : paths.back()) paths[beginMap[v]].push_front(node);
				paths.pop_back();
			}
		}
		if(chooseRandom){
			// cout<<"choosing random node"<<endl;
			roulette.clear();
			// cout<<"available vertices"<<endl;
			// for(int i =0;i<this->seq->graphSize;i++) if(!active[i]) cout<<i<<endl;
			// cout<<endl;
			for(int i=0;i<this->seq->graphSize;i++) if(!active[i]) roulette.push_back(i);
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
		// if(blocker == 100) break;
	}
	cout<<"finished first loop"<<endl;
	cout<<"paths size: "<<paths.size()<<endl;
	cout<<"paths sizes: "<<endl;
	for(auto path : paths) cout<<path.size()<<" ";
	cout<<endl;
	delete [] active;
	active = new bool[paths.size()]();
	active[0] = true;
	// for(int i = paths.size()-1;i>=0;i--){ 
	// 	if(paths[i].size() == 1) paths.erase(paths.begin()+i);
	// }
	cout<<"paths size: "<<paths.size()<<endl;
	cout<<"paths sizes: "<<endl;
	// for(auto path : paths) cout<<path.size()<<" ";
	for(int i =0;i<(int)paths.size();i++){
		if(!i){
			cout<<"first piece: "<<graph[paths[i].back()].label<<endl;
			for(auto it : paths[i]) cout<<it<<" ";
			cout<<endl<<endl;
		}
		else{
			cout<<paths[i].front()<<": "<<graph[paths[i].front()].label<<endl;
			for(auto it : paths[i]) cout<<it<<" ";
			cout<<endl;
		}
	}
	cout<<endl;

	cout<<"connecting pieces"<<endl;
	int numOfIter = (int)paths.size();
	for(int x =1;x<=numOfIter;x++){
		vector<int> * weights = new vector<int> [this->seq->oligo_size+1]();
		cout<<"initiating merge"<<endl;
		for(int i = 1;i<(int)paths.size();i++){
			cout<<"i: "<<i<<endl;
			check_again:
			cout<<"init check"<<endl;
			cout<<"on check i: "<<i<<endl;
			cout<<"paths.size(): "<<paths.size()<<endl;
			cout<<"paths[i] size: "<<paths[i].size()<<endl;
			cout<<"paths[i].front(): "<<paths[i].front()<<endl;
			
			int score = adjacencyMatrix[paths.front().back()][paths[i].front()];
			cout<<"initial score: "<<score<<endl;
			
			if(score == 0){
				for(int cover = this->seq->cover+1;cover<=this->seq->oligo_size;cover++){
					if(graph[paths.front().back()].label.substr(cover,this->seq->oligo_size) == graph[paths[i].front()].label.substr(0,this->seq->oligo_size - cover)){
						score = cover;	
						break;
					}
				}
				cout<<"final score: "<<score<<endl;
				if(score == 10 && paths[i].size() == 1) {
					cout<<"useless path: "<<i<<endl;
					cout<<"useless paths size: "<<paths.size()<<endl;
					paths.erase(paths.begin()+i);
					cout<<"size after: "<<paths.size()<<endl;
					cout<<"deleted elem"<<endl;
					if(paths.size() == 1 || i >= (int)paths.size()) break;
					else goto check_again;
				}
			}
			weights[score].push_back(i);
			
		}
		vector<int>weightOrder;
		cout<<"weights:"<<endl;
		for(int i=1;i<=(int)this->seq->oligo_size;i++) cout<<weights[i].size()<<" ";
		cout<<endl;
		for(int i=1;i<=(int)this->seq->oligo_size;i++){
			if(!weights[i].empty())
			for(auto vertice : weights[i]){
				weightOrder.push_back(vertice);
				float weight = (float)(1)/(float)(i*(int)weights[i].size()); 
				cout<<"initial weight"<<weight<<endl;
				weight += pheromones[paths.front().back()][paths[vertice].front()];
				cout<<"added pheromones"<<endl;
				roulette.push_back( ((!roulette.empty())?roulette.back():0) + weight);
				cout<<"add roulette"<<endl;
			}
		}
		cout<<"roulette:"<<endl;
		for(auto it : roulette) cout<<it<<" ";
		cout<<endl;
		delete [] weights;
		float rouletteScore = ( (!roulette.empty() && roulette.back() == 0 && roulette.back() < 0.01) || roulette.empty())?(-1):(rand() % (int)(roulette.back() * 100)+1);
		rouletteScore /= 100;
		
		cout<<"chosen score: "<<rouletteScore<<endl;
		if(rouletteScore>0){
			for(int i =0;i<(int)roulette.size();i++){ 
				if((!i || (i && rouletteScore > roulette[i-1])) && rouletteScore <= roulette[i]) 
				{
					v = weightOrder[i];
					break;
				}
			}

			cout<<"chosen v: "<<v<<endl;
			for(auto node : paths[v]) paths.front().push_back(node);
			paths.erase(paths.begin() + v);
		}

		roulette.clear();
		cout<<"intermediary paths sizes: "<<endl;
	 	for(auto path : paths) cout<<path.size()<<" ";
		cout<<endl<<endl;
	}
	cout<<"final paths sizes: "<<endl;
	for(auto path : paths) cout<<path.size()<<" ";
	cout<<endl;
	float score = 0;
	cout<<paths.front().front()<<" ";
	for(int i = 1;i<(int)paths.front().size();i++){
		score += adjacencyMatrix[paths.front()[i-1]][paths.front()[i]];
		cout<<paths.front()[i]<<" ";
	}
	cout<<endl;
	float modelScore = this->seq->seqLen - this->seq->oligo_size;

	score = (score > modelScore)?(modelScore/score):(score/modelScore);	
	for(int i =1;i<(int)paths.front().size();i++){
		this->newPheromones[paths.front()[i-1]][paths.front()[i]] += 0.1 * score;
	}
	delete [] active;
}
void AntColonyOptimization::simplePath(){
	cout<<"simple path: "<<endl<<endl;
	int v_first = this->seq->firstElemIdx;	
	vertice * graph = this->seq->graph.get();	
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
	cout<<"expected size: "<<this->seq->graphSize<<endl;
	cout<<"path size: "<<path.size()<<endl;
	cout<<"path:"<<endl;
	for(auto it : path) cout<<it<<" ";
	cout<<endl;
}
