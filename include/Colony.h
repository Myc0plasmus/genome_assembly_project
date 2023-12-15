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
	
	public:
		Colony(Sequence & newSeq);
		Colony(Sequence & newSeq, double ** pheromones, vector<pair<double,deque<int>>> * newPheromones);
		~Colony();
		virtual void ant()=0;

};
