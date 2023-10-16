#include <bits/stdc++.h>
#include "vertice.h"
#include "Sequence.h"

using namespace std;

void Sequence::genNewSeq(int size)
{
	string bases[] = {"A","C","T","G"};
	this->seq = "";
	this->shreddedSeq = false;	
	srand(time(0));
	for(int i =0;i<size;i++){
		this->seq.append(bases[rand() % 4]);
	}
	this->graph.reset(new vertice[this->seq.length()]);
	this->graphSize = 0;
}

Sequence::Sequence(int size){
	genNewSeq(size);
}

void Sequence::shredSequence(map<string,int> args){
	this->graph.reset(new vertice[this->seq.length()]);
	bool randomLen = false;
	int oligoLen;
	int startRange;
	int endRange;
	if(args.find("randomLen") != args.end()) randomLen = (bool)(args["randomLen"]);
	if(!randomLen){
		srand(time(0));
		oligoLen = 9;
		if(args.find("oligoLen") != args.end()) oligoLen = args["oligoLen"]; 
	}
	else
	{
		assert(args.find("startRange") != args.end());
		assert(args.find("endRange") != args.end());
		startRange = args["startRange"]; 
		endRange = args["endRange"]; 
		assert(startRange >= 1);
		assert(endRange >= 3);
	}

	for(int i =0;i<(int)this->seq.length();i++){
		if(randomLen) { oligoLen = ( rand() % endRange ) + startRange;}
		this->graph[i].label = this->seq.substr(i,oligoLen);
		if(i+oligoLen >= (int)this->seq.length()) { this->graphSize = i; break; }
	}
	sort(this->graph.get(), this->graph.get() + this->graphSize);
	this->shreddedSeq = true;
	
		

}

void Sequence::genNewShreddedSeq(int size, map<string,int> args){
	this->genNewSeq(size);
	this->shredSequence(args);
}

void Sequence::createGraphWithFixedCover(int minCover)
{
	map<string,vector<int>> etiquetes; 
	for(int i = 0;i<=this->graphSize;i++) etiquetes[this->graph[i].label.substr(0,minCover)].push_back(i);
	// for(auto it = etiquetes.begin();it!=etiquetes.end();it++){
	// 	cout<<"key: "<<it->first<<endl<<"values: "<<endl;
	// 	for(auto v : it->second) cout<<v<<endl;
	// }
	for(int i = 0;i<=this->graphSize;i++){
		string cutoff = this->graph[i].label.substr(this->graph[i].label.length() - minCover - 1,minCover);
		if(etiquetes.find(cutoff) != etiquetes.end()) this->graph[i].edges = etiquetes[cutoff];
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
	this->createGraphWithFixedCover(2);
}

