#pragma once
#include<bits/stdc++.h>
#include "Sequence.h"
#include "Colony.h"


class AntColonyOptimization{
	private: 
		Sequence & seq;
		double ** pheromones;
		vector<pair<double,deque<int>>> newPheromones;
		
	public:
		double evaporationRate;
		double smoothingLowest;
		int smoothingLogBase;
		int numOfAnts;
		int stopTime;
		AntColonyOptimization(Sequence & newSeq);	

		AntColonyOptimization(Sequence & newSeq,double newEvaporationRate);	
		AntColonyOptimization(Sequence & newSeq,double newEvaporationRate, double newSmoothingLowest, int newSmoothingLogBase);	
		~AntColonyOptimization();
		void pheromoneEvaporation();
		void applyPheromones();
		void pheremoneSmoothing();
		void simplePath();
		template<class T> vector<pair<double,string>> commenceACO(){
			assert((is_base_of<Colony,T>::value));
			clock_t start = clock();	
			vector<pair<double,string>> res;
			vertice * graph = this->seq.graph.get();
			int ** adjacencyMatrix = this->seq.adjacencyMatrix;
			T colonyType(this->seq, this->pheromones, &this->newPheromones);
			
			int breaker = 0;
			while(1){
				// cout<<"loop num: "<<breaker<<endl ;
				this->pheromoneEvaporation();
				for(int i =0;i<this->numOfAnts;i++){
					// cout<<"ant num: "<<i<<endl;
					colonyType.ant();
				}
				clock_t stop = clock();
				float timePassed = (float)(stop - start) / CLOCKS_PER_SEC;
				cout<<timePassed<<endl;
				if(timePassed > this->stopTime) break;
				this->applyPheromones();
				this->pheremoneSmoothing();
				
				// breaker++;
				// if(breaker == 2) break;
				
			}
			cout<<"removing duplicates"<<endl;
			
			for(int base = 0;base < (int)this->newPheromones.size();base++){
					for(int checked = base+1;checked < (int)this->newPheromones.size();checked++){
						check_again:
						if(this->newPheromones[base].second == this->newPheromones[checked].second){
							this->newPheromones.erase(this->newPheromones.cbegin() + checked);
							if(checked < (int)this->newPheromones.size())goto check_again;
							else break;
						}
					}
			}
			cout<<"creating result"<<endl;
			for(auto path : this->newPheromones){
				string reconstruction = "";
				for(int i = 0;i<(int)path.second.size();i++){
					// cout<<"reconstruction pre: "<<reconstruction<<endl;
					string label = graph[path.second[i]].label;
					// cout<<"label: "<<label<<endl;
					if(reconstruction == "") reconstruction = label;
					else{
						// cout<<"edge score: "<<adjacencyMatrix[i-1][i]<<endl;
						reconstruction.append(label.substr(label.length()-adjacencyMatrix[path.second[i-1]][path.second[i]],this->seq.oligo_size));	
					}
					// cout<<"reconstruction pre: "<<reconstruction<<endl;
				}
				res.push_back(make_pair(path.first,reconstruction));

			}
			return res;
		}
};
