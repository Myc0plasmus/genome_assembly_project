#include <bits/stdc++.h>
#include "vertice.h"
#include "Sequence.h"

using namespace std;

Sequence::Sequence(int size){
	string bases[] = {"A","C","T","G"};
	this->graph.reset(new vertice[size]);
	this->seq = "";
	srand(time(0));
	for(int i =0;i<size;i++){
		this->seq.append(bases[rand() % 4]);
	}

}

