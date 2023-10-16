#pragma once 
#include <bits/stdc++.h>
#include "vertice.h"

class Sequence{
	private:
		int graphSize;
		bool shreddedSeq;
	public:
		string seq;
		unique_ptr<vertice[]> graph;
		Sequence(int size);
		void shredSequence(map<string,int> args);
		void createGraphWithFixedCover(int minCover);
		void createDefaultGraph();
		void genNewSeq(int size);
		void genNewShreddedSeq(int size,map<string,int> args);
};
