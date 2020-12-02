#include "Network.h"
#include "Utils.h"
#include <stack>
#include <numeric> 

Network::Network()
{
	NetworkExist = false;
}

void Network::CreateConnection(unordered_map<int, Pipeline>& Pipeline_s, unordered_map<int, Ks>& Ks_s)
{
	if ((!Pipeline_s.size()) or (Ks_s.size() < 2))
		cout << "Not enough objects to create a gts.\n ";
	else
	{
		unordered_map<int, Ks>::iterator IterObj1 = Ks_s.find(GetCorrectNumber(0, INT_MAX,
			"\n1. Please enter id of the ks from which the pipe exits: "));
		unordered_map<int, Pipeline>::iterator IterObj2 = Pipeline_s.find(GetCorrectNumber(0, INT_MAX,
			"\n2. Please enter id of the pipe connecting ks: "));
		unordered_map<int, Ks>::iterator IterObj3 = Ks_s.find(GetCorrectNumber(0, INT_MAX,
			"\n3. Please enter id of the ks into which the pipe enters: "));
		if (IterObj1 == Ks_s.end())
		{
			cout << "The ks from which the pipe exits not found.\n";
			return;
		}
		if ((IterObj2->second.repear == true) or (IterObj2 == Pipeline_s.end()))
		{
			cout << "The pipe connecting ks not found or this pipe is in repear.\n";
			return;
		}
		if (IterObj2->second.InputId != 0)
		{
			cout << "This pipe has already been connected with other ks.\n";
			return;
		}
		if ((IterObj1->second.GetId() == IterObj3->second.GetId()) or (IterObj3 == Ks_s.end()))
		{
			cout << "The ks into which the pipe enters not found or you have chosen 2 idential ks.\n";
			return;
		}
		GtsKs.insert(IterObj1->second.GetId());
		GtsPipe.insert(IterObj2->second.GetId());
		GtsKs.insert(IterObj3->second.GetId());
		IterObj2->second.InputId = IterObj1->second.GetId();
		IterObj2->second.OutputId = IterObj3->second.GetId();
	}
}

void Network::CreateNetwork(unordered_map<int, Pipeline>& Pipeline_s)
{
	int numKs = 0;
	int numPipes = 0;
	for (const auto& obj : GtsKs)
	{
		mGtsKs.insert(pair<int, int>(numKs, obj));
		numKs++;
	}
	for (const auto& obj : GtsPipe)
	{
		unordered_map<int, Pipeline>::iterator Iter = Pipeline_s.find(obj);
		mGtsPipe.insert(pair<int, Pipeline>(numPipes, Iter->second));
		numPipes++;
	}
	for (const auto& line : mGtsKs)
	{
		for (const auto& column : mGtsKs)
		{
			network.emplace(make_pair(line.first, column.first), 0);
		}
	}
	for (const auto& obj : mGtsPipe)
	{
		int line = 0;
		int column = 0;
		for (const auto& matr : mGtsKs)
		{
			if (matr.second == obj.second.InputId)
				line = matr.first;
			if (matr.second == obj.second.OutputId)
				column = matr.first;
		}
		network.erase(network.find(make_pair(line, column)));
		network.emplace(make_pair(line, column), 1);
	}
	NetworkExist = true;
	cout << "Transmission network created.\n";
}

void Network::PrintNetwork()
{
	cout << "\n ";
	for (const auto& column : mGtsKs)
		cout << " " << column.second;
	for (const auto& line : mGtsKs)
	{
		cout << endl << line.second;
		for (const auto& column : mGtsKs)
		{
			cout << " " << network[make_pair(line.first, column.first)];
		}
	}
	cout << "\n\n";
}

void Network::TopolSort()
{
	map <int, int> prom;
	int line;
	int length = (int)mGtsKs.size();
	map<pair<int, int>, int>  matr;

	for (const auto& line : mGtsKs)
			for (const auto& column : mGtsKs)
			matr[make_pair(line.first, column.first)]=network[make_pair(line.first, column.first)];
	int pos=0;
	unordered_map <int, int>::iterator it = mGtsKs.end();
	for (int i = 0; i <= (int)mGtsKs.size()-1; i++)
	{
		for (line=0; line<=length; line++)
		{
			pos++;
			int summ = 0;
			for (int column = 0; column <= length; column++)
				summ += matr[make_pair(line, column)];
			if (summ == 0)
			{
				it--;
				prom.insert(pair<int, int> (pos, (it->second)));
				for (int stolb = 0; stolb <= length; stolb++)
				{
				    matr.erase(make_pair(line, stolb));
					matr.erase(make_pair(stolb, line));
				}
				length--;
				break;
			}
		}
	}
	cout << "Topological sort:\n";
	for (auto it = prom.begin(); it != prom.end(); it++)
		cout << it->second << "<-";
	cout << endl;
}

