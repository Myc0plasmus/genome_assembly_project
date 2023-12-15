#pragma once
#include <bits/stdc++.h>
#include "Sequence.h"

class colony{
	protected:
		Sequence & seq;
		double ** pheromones;
		vector<pair<double,deque<int>>> newPheromones;
		int findInRoulette(float val, vector<float> & roulette);
	
	public:
		colony(Sequence & newSeq);
		colony(Sequence & newSeq, double ** newPheromones);

};
