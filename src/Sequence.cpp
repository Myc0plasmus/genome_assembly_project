#include <bits/stdc++.h>
#include "vertice.h"
#include "Sequence.h"
#include "edge.h"

using namespace std;

int Sequence::rangeAvg(int start,int end){
	assert(start<=end);
	if(start == end) return start;
	int mysum =0;
	for(int i = start; i <= end;i++) mysum+=i; 
	return mysum / (end = start);
}

void Sequence::genNewSeq(int size)
{
	string bases[] = {"A","C","T","G"};
	this->seq = "";
	this->seqLen = size;
	this->shreddedSeq = false;	
	for(int i =0;i<size;i++){
		this->seq.append(bases[rand() % 4]);
	}
	this->graph.reset(new vertice[this->seqLen]);
	this->graphSize = 0;
}

Sequence::Sequence(int size){
	genNewSeq(size);
}

void Sequence::shredSequence(map<string,float> args){
	int oligoLen;
	float falsePositiveThreshold = 0.01;
	float falseNegativeThreshold = 0.01;
	oligoLen = 9;
	if(args.find("oligoLen") != args.end()) oligoLen = args["oligoLen"];
	if(args.find("falsePositiveThreshold") != args.end()) falsePositiveThreshold = args["falsePositiveThreshold"];
	if(args.find("falseNegativeThreshold") != args.end()) falseNegativeThreshold = args["falseNegativeThreshold"];
	assert(falseNegativeThreshold < 1 && falseNegativeThreshold >=0);
	assert(falsePositiveThreshold < 1 && falsePositiveThreshold >=0);

	this->graph.reset(new vertice[this->seqLen +  (int)(this->seqLen * falsePositiveThreshold )]);
	// cout<<"made graph with "<<this->seqLen +  (int)(this->seqLen * falsePositiveThreshold )<<" vertices"<<endl;
	int falsePositives = 0;
	int falseNegatives = 0;
	for(int i =0;i<(int)this->seq.length();i++){
		// cout<<"curr vertice num: "<<i+falsePositives-falseNegatives<<endl;
		if( (float)(rand()) / (float)(RAND_MAX) <= falsePositiveThreshold){
			string bases[] = {"A","C","T","G"};
			string falseOligo = "";
			for(int j=0;j<oligoLen;j++) falseOligo.append(bases[rand() % 4]);
			this->graph[i+falsePositives-falseNegatives].label = falseOligo;
			falsePositives++;
		}
		if((float)(rand()) / (float)(RAND_MAX) <= falseNegativeThreshold ){
			falseNegatives++;
			continue;
		}
		this->graph[i+falsePositives-falseNegatives].label = this->seq.substr(i,oligoLen);
		if(i+oligoLen >= (int)this->seq.length()) { this->graphSize = i+falsePositives-falseNegatives; break; }
		// cout<<"Iter "<<i<<" completed"<<endl;
		
	}
	sort(this->graph.get(), this->graph.get() + this->graphSize);
	this->shreddedSeq = true;
	this->oligo_size = oligoLen;
	// cout<<"false negatives: "<<falseNegatives<<endl;
	// cout<<"false positives: "<<falsePositives<<endl;
	// cout<<"graphSize: "<<this->graphSize<<endl;
	
		

}

void Sequence::genNewShreddedSeq(int size, map<string,float> args){
	this->genNewSeq(size);
	this->shredSequence(args);
}

void Sequence::createGraphWithFixedCover(int minCover)
{
	assert(this->shreddedSeq);
	vector<map<string,vector<edge>>> etiquetes; 
	for(int i =0;i<=minCover;i++) etiquetes.push_back({});
	for(int cover = 1;cover <= minCover;cover++){
		for(int i = 0;i<=this->graphSize;i++) etiquetes[cover][this->graph[i].label.substr(0,this->oligo_size - cover)].push_back(edge(cover,i));
	}
	// for(auto it = etiquetes.begin();it!=etiquetes.end();it++){
	// 	cout<<"key: "<<it->first<<endl<<"values: "<<endl;
	// 	for(auto v : it->second) cout<<v<<endl;
	// }
	for(int i = 0;i<=this->graphSize;i++){
		// cout<<"This is "<<i<<"th iteration"<<endl;
		// cout<<"Lenght ith label: "<<this->graph[i].label.length()<<endl;
		for(int cover = 1;cover <= minCover;cover++){
			string cutoff = this->graph[i].label.substr(cover,this->oligo_size - cover);
			// cout<<"cutoff: "<<cutoff<<endl;
			if(etiquetes[cover].find(cutoff) != etiquetes[cover].end()) this->graph[i].edges = etiquetes[cover][cutoff];
		}
	}
	// for(int i=0;i<=graphSize;i++){
	// 	cout<<"label: "<<this->graph[i].label<<endl;
	// 	cout<<"values"<<endl;
	// 	for(auto node : this->graph[i].edges){
	// 		cout<<node<<endl;
	// 	}	
	// }
}

void Sequence::createDefaultGraph(){
	// cout<<"generating default graph"<<endl;
	this->createGraphWithFixedCover(3);
}

