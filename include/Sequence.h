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
		float falsePositiveThreshold;
		float falseNegativeThreshold;
		int cover;
		string seq;
		shared_ptr<vertice[]> graph;
		int ** adjacencyMatrix;
		Sequence(int size);
		Sequence(string newSeq);
		~Sequence();
		void shredSequence();
		void createGraphWithFixedCover();
		void genNewSeq(int size);
		void getNewSeq(string newSeq);
		void genNewShreddedSeq(int size);
		void postGenerationRoutine();
};
