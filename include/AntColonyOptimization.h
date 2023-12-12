#pragma once
#include<bits/stdc++.h>
#include "Sequence.h"


class AntColonyOptimization{
	private: 
		Sequence * seq;
		double ** pheromones;
		double ** newPheromones;
		
	public:
		AntColonyOptimization(Sequence * newSeq);	
		~AntColonyOptimization();
		void ant();
		void pheromoneEvaporation();
		void pheremoneSmoothing();
		void simplePath();
};
