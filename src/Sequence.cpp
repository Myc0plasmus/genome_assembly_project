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
	this->hasSeq = true;
	string bases[] = {"A","C","T","G"};
	this->seq = "";
	this->seqLen = size;
	this->shreddedSeq = false;	
	for(int i =0;i<size;i++){
		this->seq.append(bases[rand() % 4]);
	}
	postGenerationRoutine();	
}

void Sequence::clearGraph(){
	assert(this->shreddedSeq);
	for(int i=0;i<this->graphSize;i++){
		this->graph[i].edges.clear();
	}
}

void Sequence::getNewSeq(string newSeq)
{
	this->hasSeq = true;
	this->seq = newSeq;
	this->seqLen = newSeq.length();
	this->shreddedSeq = false;	
	postGenerationRoutine();	
}

void Sequence::postGenerationRoutine(){
	this->graph.reset(new vertice[this->seqLen]());
	if((int)adjacencyMatrix.size() != 0) adjacencyMatrix.clear();
	// if(this->adjacencyMatrix != NULL){
	// 	for(int i =0;i<seqLen;i++) delete [] adjacencyMatrix[i];
	// 	delete [] this->adjacencyMatrix;
	// }
	this->graphSize = 0;

	//assign default val
	this->falsePositiveThreshold = 0.01;
	this->falseNegativeThreshold = 0.01;
	this->oligo_size = 10;
	this->cover = 3;

}


Sequence::Sequence(int size) {
	// this->adjacencyMatrix = NULL;
	genNewSeq(size);
}

Sequence::Sequence(string newSeq){
	// this->adjacencyMatrix = NULL;
	getNewSeq(newSeq);
}

Sequence::~Sequence(){
	// if(adjacencyMatrix != NULL){
	// 	for(int i =0;i<this->seqLen;i++) delete [] adjacencyMatrix[i];
	// 	delete [] adjacencyMatrix;
	// }
}

void Sequence::shredSequence(){
	assert(this->hasSeq);
	bool debug = false;
	int oligoLen;
	
	oligoLen = this->oligo_size;
	assert(falseNegativeThreshold < 1 && falseNegativeThreshold >=0);
	assert(falsePositiveThreshold < 1 && falsePositiveThreshold >=0);

	this->graph.reset(new vertice[this->seqLen +  (int)(this->seqLen * falsePositiveThreshold )]);
	// cout<<"made graph with "<<this->seqLen +  (int)(this->seqLen * falsePositiveThreshold )<<" vertices"<<endl;
	int falsePositives = 0;
	int falseNegatives = 0;
	for(int i =0;i<(int)this->seq.length();i++){
		// cout<<"curr vertice num: "<<i+falsePositives-falseNegatives<<endl;
		if(i != 0 && (float)(rand()) / (float)(RAND_MAX) <= falsePositiveThreshold){
			string bases[] = {"A","C","T","G"};
			string falseOligo = "";
			for(int j=0;j<oligoLen;j++) falseOligo.append(bases[rand() % 4]);
			this->graph[i+falsePositives-falseNegatives].label = falseOligo;
			falsePositives++;
			if(debug) cout<<"false positive: "<<falseOligo<<endl;
		}
		if( i != 0 && (float)(rand()) / (float)(RAND_MAX) <= falseNegativeThreshold ){
			if(debug) cout<<"false negative: "<<i<<endl;
			falseNegatives++;
			continue;
		}
		this->graph[i+falsePositives-falseNegatives].label = this->seq.substr(i,oligoLen);
		// cout<<"i:"<<i<<" oligoLen:"<<oligoLen<<" seqLen:"<<this->seqLen<<" condition: "<<(i+oligoLen >= (int)this->seqLen)<<endl;
		if(i+oligoLen >= (int)this->seqLen) { 
			//cout<<"last i:"<<i<<endl;
			this->graphSize = this->seqLen-oligoLen+1+falsePositives-falseNegatives; break; }
		// cout<<"Iter "<<i<<" completed"<<endl;
		
	}
	// cout<<"falsePositives"<<falsePositives<<endl;
	// cout<<"falseNegatives"<<falseNegatives<<endl;
	// cout<<"graphSize: "<<graphSize<<endl;
	string firstLabel = graph[0].label;
	this->adjacencyMatrix.reserve(this->graphSize+10);
	this->adjacencyMatrix.assign(this->graphSize+10, vector<int>(this->graphSize+10,0));
	// this->adjacencyMatrix = new int* [this->seqLen+10]();
	// for(int i =0;i<this->seqLen;i++) this->adjacencyMatrix[i] = new int[this->seqLen+10]();
	sort(this->graph.get(), this->graph.get() + this->graphSize);
	// cout<<"sort finished"<<endl;
	for(int i=0;i<this->seqLen;i++){
		// cout<<"checking i:"<<i<<endl;
		if(graph[i].label == firstLabel){
			this->firstElemIdx = i;
			break;
		}
	}
	this->shreddedSeq = true;
	if(debug) cout<<"false negatives: "<<falseNegatives<<endl;
	if(debug) cout<<"false positives: "<<falsePositives<<endl;

	this->falseNegatives = falseNegatives;
	this->falsePositives = falsePositives;
	// cout<<"graphSize: "<<this->graphSize<<endl;
	
		

}

void Sequence::genNewShreddedSeq(int size){
	this->genNewSeq(size);
	this->shredSequence();
}

void Sequence::createGraphWithFixedCover()
{
	assert(this->shreddedSeq);
	int minCover = this->cover;
	vector<map<string,vector<edge>>> etiquetes; 
	for(int i =0;i<=minCover;i++) etiquetes.push_back({});
	for(int seq_cover = 1;seq_cover <= minCover;seq_cover++){
		for(int i = 0;i<this->graphSize;i++) etiquetes[seq_cover][this->graph[i].label.substr(0,this->oligo_size - seq_cover)].push_back(edge(seq_cover,i));
	}
	// int another_cover=0;
	// for(auto it = etiquetes.begin();it!=etiquetes.end();it++){
	// 	cout<<"seq_cover: "<<another_cover<<endl;
	// 	for(auto mapElem = it->begin();mapElem!=it->end();mapElem++){
	// 	cout<<"key: "<<mapElem->first<<endl<<"values: "<<endl;
	// 	for(auto v : mapElem->second) cout<<v.neighbour<<" "<<v.val<<endl;
	// 	}
	// 	another_cover++;
	// }
	for(int i = 0;i<this->graphSize;i++){
		// cout<<"This is "<<i<<"th iteration"<<endl;
		// cout<<"Lenght ith label: "<<this->graph[i].label.length()<<endl;
		for(int seq_cover = 1;seq_cover <= minCover;seq_cover++){
			string cutoff = this->graph[i].label.substr(seq_cover,this->oligo_size);
			// cout<<"cutoff: "<<cutoff<<endl;
			if(etiquetes[seq_cover].find(cutoff) != etiquetes[seq_cover].end()){
				this->graph[i].edges.reserve(graph[i].edges.size() + distance(etiquetes[seq_cover][cutoff].begin(),etiquetes[seq_cover][cutoff].end()));
				this->graph[i].edges.insert(graph[i].edges.end(),etiquetes[seq_cover][cutoff].begin(),etiquetes[seq_cover][cutoff].end());
			}
		}
	}
	for(int i = 0;i<this->graphSize;i++){
		for(auto v : graph[i].edges){
			this->adjacencyMatrix[i][v.neighbour] = v.val;
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

Sequence::Sequence(){
	this->shreddedSeq = false;	
	this->hasSeq = false;
}
