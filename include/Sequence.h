#pragma once 
#include <bits/stdc++.h>
#include "vertice.h"



class Sequence{
	private:
		bool shreddedSeq;
		int rangeAvg(int start,int end);
	public:
		int oligo_size;
		int graphSize;
		int seqLen;
		int firstElemIdx;
		int cover;
		string seq;
		shared_ptr<vertice[]> graph;
		int ** adjacencyMatrix;
		Sequence(int size);
		~Sequence();
		void shredSequence(map<string,float> args);
		void createGraphWithFixedCover(int minCover);
		void createDefaultGraph();
		void genNewSeq(int size);
		void genNewShreddedSeq(int size,map<string,float> args);
};
