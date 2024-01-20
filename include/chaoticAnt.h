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

		float getRouletteScore(vector<float> & roulette, bool debug);
		void fillWeights(int v, vector<vector<int>> & weights, vector<bool> & active, int & minScore,bool debug);
		void fillRoulette(int v, vector<float> & roulette, vector<vector<int>> & weights, bool debug);
		void pickNextVertice(int & v, vector<float> & roulette, vector<vector<int>> & weights, deque<int> & randJump, vector<bool> & active, bool debug);

		virtual void ant() override;

		void debugLogActive(bool * active, bool debug);

		virtual void pheremoneApplyEvent() override; 
		virtual void pheromoneEvaporationEvent() override; 
		virtual void filterPheromoneTrailEvent(int lastBest) override;

};
