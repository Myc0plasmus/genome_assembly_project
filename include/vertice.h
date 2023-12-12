#pragma once
#include <bits/stdc++.h>
#include "edge.h"

using namespace std;

typedef struct vertice{
	string label;
	vector<edge> edges;
	vertice() : label(""), edges(){}
	vertice(string new_label) : label(new_label),edges(){}
	inline bool operator < (const vertice& otherOne) const {
		return this->label < otherOne.label;
	}
}vertice;
