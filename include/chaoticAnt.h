#pragma once
#include<bits/stdc++.h>
#include "Colony.h"

class chaoticAnt : public Colony{
	private:
	public: 
		double * endPheromones;
		vector<pair<double,int>> newEndPheromones; 
		double * chaosPheromones;
		vector<pair<double,deque<int>>> newChaosPheromones; 

		chaoticAnt(Sequence & newSeq);
		chaoticAnt(Sequence & newSeq, double ** linkedPheromones, vector<pair<double,deque<int>>> * linkedNewPheromones);
		~chaoticAnt();
		virtual void ant() override;
		virtual void pheremoneApplyEvent() override; 
		virtual void pheromoneEvaporationEvent() override; 
		virtual void filterPheromoneTrailEvent(int lastBest) override;

};
