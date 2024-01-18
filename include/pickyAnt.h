#pragma once
#include<bits/stdc++.h>
#include "Colony.h"

class pickyAnt : public Colony{
	public: 
		using Colony::Colony;
		virtual void ant() override;
		void fillRoulette(int v, int pathNum, vector<float> & roulette, map<int,int> beginMap, bool * active, vector<deque<int>> paths, bool debug);
		void pickNextVertice(int v, int pathNum, vector<float> & roulette, map<int,int> beginMap, bool * active, vector<deque<int>> paths, bool debug);
		
		// this is just for testing, but don't delete
		// virtual void antColonyFinishEvent() override;
};
