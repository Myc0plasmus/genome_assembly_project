#pragma once 
#include <bits/stdc++.h>
#include "vertice.h"

class Sequence{
	public:
		string seq;
		unique_ptr<vertice[]> graph;
		Sequence(int size);
		void createGraph(map<string, int> args);
		void createDefaultGraph();
};
