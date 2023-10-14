#pragma once
#include <bits/stdc++.h>

using namespace std;

typedef struct vertice{
	string label;
	vector<int> edges;
	vertice() : label(""){}
	vertice(string new_label) : label(new_label){}

}vertice;
