#pragma once
#include "Pipeline.h"
#include "Ks.h"
#include <unordered_map>
#include <map>
#include <set>
using namespace std;

class Network
{
private:
	unordered_map <int, int> mGtsKs;
	unordered_map <int, Pipeline> mGtsPipe;
	map<pair<int, int>, int> network;
public:
	set <int> GtsPipe;
	set <int> GtsKs;
	bool NetworkExist;

	Network();
	void CreateConnection(unordered_map<int, Pipeline>& Pipeline_s, unordered_map<int, Ks>& Ks_s);
	void CreateNetwork(unordered_map<int, Pipeline>& Pipeline_s);
	void PrintNetwork();
};