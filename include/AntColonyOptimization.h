#pragma once
#include<bits/stdc++.h>
#include "Sequence.h"

const int N = 1e5;

class AntColonyOptimization{
	private: 
		Sequence * seq;
		double pheromones[N][N];
		
	public:
		AntColonyOptimization(Sequence * newSeq);	
		void ant(int v_start);
};
