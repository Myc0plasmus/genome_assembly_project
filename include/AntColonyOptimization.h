#pragma once
#include<bits/stdc++.h>
#include "Sequence.h"


class AntColonyOptimization{
	private: 
		Sequence * seq;
		double ** pheromones;
		
	public:
		AntColonyOptimization(Sequence * newSeq);	
		~AntColonyOptimization();
		void ant();
};
