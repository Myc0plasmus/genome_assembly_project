#pragma once
#include<bits/stdc++.h>
#include "Colony.h"

class chaoticAnt : public Colony{
	private:
	public: 
		long double * endPheromones;
		vector<pair<long double,int>> newEndPheromones; 
		long double * chaosPheromones;
		vector<pair<long double,deque<int>>> newChaosPheromones; 

		chaoticAnt(Sequence & newSeq);
		chaoticAnt(Sequence & newSeq, vector<vector<long double>> * linkedPheromones, vector<pair<long double,deque<int>>> * linkedNewPheromones);
		~chaoticAnt();
		virtual void ant() override;
		virtual void pheremoneApplyEvent() override; 
		virtual void pheromoneEvaporationEvent() override; 
		virtual void filterPheromoneTrailEvent(int lastBest) override;

};
