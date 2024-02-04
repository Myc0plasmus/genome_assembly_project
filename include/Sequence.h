#pragma once 
#include <bits/stdc++.h>
#include "vertice.h"



class Sequence{
	private:
		bool shreddedSeq;
		int rangeAvg(int start,int end);
	public:
		bool hasSeq;
		int oligo_size;
		int graphSize;
		int seqLen;
		int firstElemIdx;
		float falsePositiveThreshold;
		float falseNegativeThreshold;
		int falseNegatives;
		int falsePositives;
		int cover;
		string seq;
		shared_ptr<vertice[]> graph;
		vector<vector<int>> adjacencyMatrix;
		Sequence(int size);
		Sequence(string newSeq);
		Sequence();
		~Sequence();
		void shredSequence();
		void createGraphWithFixedCover();
		void genNewSeq(int size);
		void getNewSeq(string newSeq);
		void genNewShreddedSeq(int size);
		void postGenerationRoutine();
		void clearGraph();
};
