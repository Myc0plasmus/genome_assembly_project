#pragma once
#include<bits/stdc++.h>
#include "Colony.h"

class pickyAnt : public Colony{
	public: 
		using Colony::Colony;
		virtual void ant() override;
		void fillRoulette(int v, int pathNum, vector<float> & roulette, map<int,int> & beginMap, vector<bool> & active, vector<deque<int>> & paths, bool debug);
		float getRouletteScore(vector<float> & roulette, bool debug);
		void pickNextVertice(int & v, int & pathNum, vector<float> & roulette, map<int,int> & beginMap, vector<bool> & active, vector<deque<int>> & paths, bool debug);
		void debugLogPathsSize(bool debug, vector<deque<int>> & paths, string comment="");
		void debugLogPathsConnections(bool debug, vector<deque<int>> & paths);
		void fillFragmentWeights(vector<vector<int>> & weights,vector<deque<int>> & paths, bool debug);
		void fillWeightRoulette(vector<float> & roulette, vector<vector<int>> & weights, vector<int> & weightOrder, vector<deque<int>> & paths, bool debug);
		
		// this is just for testing, but don't delete
		// virtual void antColonyFinishEvent() override;
};
