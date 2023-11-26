#pragma once
#include <bits/stdc++.h>
#include "edge.h"

using namespace std;

typedef struct vertice{
	string label;
	vector<edge> edges;
	vertice() : label(""){}
	vertice(string new_label) : label(new_label){}
	inline bool operator < (const vertice& otherOne) const {
		return this->label < otherOne.label;
	}
}vertice;
