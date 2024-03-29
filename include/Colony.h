#pragma once
#include <bits/stdc++.h>
#include "Sequence.h"

class Colony{
	protected:
		Sequence & seq;
		vector<vector<long double>> * pheromones;
		bool loneInstance;
		vector<pair<long double,deque<int>>> * newPheromones;
		int findInRoulette(float val, vector<float> & roulette);
		int findInWeightedRoulette(float val, vector<float> & roulette, vector<vector<int>> & weights);	
	public:
		Colony(Sequence & newSeq);
		Colony(Sequence & newSeq, vector<vector<long double>> * linkedPheromones, vector<pair<long double,deque<int>>> * linkedNewPheromones);
		~Colony();
		virtual void ant()=0;
		virtual void pheremoneApplyEvent(bool debug);
		virtual void pheromoneEvaporationEvent(bool debug);
		virtual void filterPheromoneTrailEvent(int lastBest, bool debug);
		virtual void antColonyFinishEvent(bool debug);

};
