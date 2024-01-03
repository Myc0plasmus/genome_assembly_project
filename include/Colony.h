#pragma once
#include <bits/stdc++.h>
#include "Sequence.h"

class Colony{
	protected:
		Sequence & seq;
		double ** pheromones;
		bool loneInstance;
		vector<pair<double,deque<int>>> * newPheromones;
		int findInRoulette(float val, vector<float> & roulette);
		int findInWeightedRoulette(float val, vector<float> roulette, vector<int> * weights);	
	public:
		Colony(Sequence & newSeq);
		Colony(Sequence & newSeq, double ** linkedPheromones, vector<pair<double,deque<int>>> * linkedNewPheromones);
		~Colony();
		virtual void ant()=0;
		virtual void pheremoneApplyEvent();
		virtual void pheromoneEvaporationEvent();
		virtual void filterPheromoneTrailEvent(int lastBest);
		virtual void antColonyFinishEvent();

};
