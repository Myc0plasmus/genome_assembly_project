#pragma once
#include<bits/stdc++.h>
#include "Sequence.h"


class AntColonyOptimization{
	private: 
		Sequence * seq;
		double ** pheromones;
		vector<pair<double,deque<int>>> newPheromones;
		int findInRoulette(float val, vector<float> & roulette);
		
	public:
		double evaporationRate;
		double smoothingLowest;
		int smoothingLogBase;
		int numOfAnts;
		int stopTime;
		AntColonyOptimization(Sequence * newSeq);	
		AntColonyOptimization(Sequence * newSeq,double newEvaporationRate);	
		AntColonyOptimization(Sequence * newSeq,double newEvaporationRate, double newSmoothingLowest, int newSmoothingLogBase);	
		~AntColonyOptimization();
		void ant();
		void pheromoneEvaporation();
		void applyPheromones();
		void pheremoneSmoothing();
		void simplePath();
		vector<pair<double,string>> commenceACO(function<void()> antAlgo);
};
