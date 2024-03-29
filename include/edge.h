#pragma once
#include <bits/stdc++.h>

using namespace std;

typedef struct edge{
	int val; 
	int neighbour;	
	edge() : val(0), neighbour(0){}
	edge(float new_val, int new_neighbour) : val(new_val), neighbour(new_neighbour){}
	edge(int new_val, int new_neighbour) : val(new_val), neighbour(new_neighbour){}
	inline bool operator < (const edge& otherOne) const {
		return this->val < otherOne.val;
	}
}edge;
