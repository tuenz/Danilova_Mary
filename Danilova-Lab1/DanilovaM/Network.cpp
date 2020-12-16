#include "Network.h"
#include "Utils.h"
#include <algorithm> 

Network::Network()
{
	NetworkExist = false;
	cycle_found = false;
}

void Network::CreateConnection(unordered_map<int, Pipeline>& Pipeline_s, const unordered_map<int, Ks>& Ks_s)
{
	if ((!Pipeline_s.size()) or (Ks_s.size() < 2))
		cout << "Not enough objects to create a gts.\n ";
	else
	{
		auto IterObj1 = Ks_s.find(GetCorrectNumber(1, Ks::MaxId,
			"\n1. Please enter correct id of the ks from which the pipe exits: "));
		auto IterObj2 = Pipeline_s.find(GetCorrectNumber(1, Pipeline::MaxId,
			"\n2. Please enter correct id of the pipe connecting ks: "));
		auto IterObj3 = Ks_s.find(GetCorrectNumber(1, Ks::MaxId,
			"\n3. Please enter correct id of the ks into which the pipe enters: "));
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

void Network::CreateNetwork(const unordered_map<int, Pipeline>& Pipeline_s)
{
	if (!GtsKs.size() && !GtsPipe.size())
	{
		cout << "You have not yet selected objects to create a transmission network.\n";
		return;
	}
	int numKs = 0;
	int numPipes = 0;
	mGtsKs.clear();
	mGtsPipe.clear();
	network.clear();
	throughputs.clear();

	for (const auto& obj : GtsKs)
	{
		mGtsKs.insert(pair<int, int>(numKs, obj));
		numKs++;
	}
	for (const auto& obj : GtsPipe)
	{
		auto Iter = Pipeline_s.find(obj);
		mGtsPipe.insert(pair<int, Pipeline>(numPipes, Iter->second));
		numPipes++;
	}
	for (const auto& line : mGtsKs)
	{
		for (const auto& column : mGtsKs)
		{
			network.emplace(make_pair(line.first, column.first), 0);
			throughputs.emplace(make_pair(line.first, column.first), 0);
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
	    throughputs.erase(throughputs.find(make_pair(line, column)));
		network.emplace(make_pair(line, column), obj.second.Weight);
		throughputs.emplace(make_pair(line, column), obj.second.Throughput);
	}
	NetworkExist = true;
	cout << "Transmission network updated.\n";
}

void Network::PrintTable(map<pair<int, int>, int>& table)
{
	cout << "\n ";
	for (const auto& column : mGtsKs)
		cout << "\t" << column.second;
	for (const auto& line : mGtsKs)
	{
		cout << endl << line.second;
		for (const auto& column : mGtsKs)
		{
			cout << "\t" << table[make_pair(line.first, column.first)];
		}
	}
	cout << "\n\n";
}

void Network::PrintNetwork()
{
	if (NetworkExist)
	{
		cout << "\nAdjacent table for pipe lengths:\n";
		PrintTable(network);
		cout << "\nAdjacent table for pipe throughputs:\n";
		PrintTable(throughputs);
	}
	else cout << "The network has not yet been created.\n";
}

void Network::KsDelChanges(int id, unordered_map <int, Pipeline>& Pipeline_s)
{
	//https://stackoverflow.com/questions/20627458/how-to-remove-elements-from-an-stdset-while-iterating-over-it/20627506#20627506
	for (auto IterKs = GtsKs.begin(); IterKs != GtsKs.end();)
	{
		if (*IterKs == id)
		{
			GtsKs.erase(IterKs++);
			for (auto& p : Pipeline_s)
			{
				if ((p.second.OutputId == id) || (p.second.InputId == id))
				{
					p.second.InputId = 0;
					p.second.OutputId = 0;
					for (auto IterPipe = GtsPipe.begin(); IterPipe != GtsPipe.end();)
					{
						if (*IterPipe == p.second.GetId())
							GtsPipe.erase(IterPipe++);
						else
							IterPipe++;

					}
				}
			}
		}
		else
			IterKs++;
	}
}

void Network::PipeDelChanges(int id)
{
	for (auto IterPipe = GtsPipe.begin(); IterPipe != GtsPipe.end();)
	{
		if (*IterPipe == id)
			GtsPipe.erase(IterPipe++);
		else
			IterPipe++;
	}
}

void Network::SaveNetwork(const unordered_map<int, Pipeline>& Pipeline_s, const unordered_map<int, Ks>& Ks_s)
{
	if (NetworkExist)
	{
		ofstream fout;
		fout.open(AskingForName(), ios::out);
		if (fout.is_open())
		{
			fout << GtsPipe.size() << endl;
			fout << GtsKs.size() << endl;
			for (const auto& pipe : GtsPipe)
				fout << pipe << endl;
			for (const auto& ks : GtsKs)
				fout << ks << endl;
			for (const auto& pipe : GtsPipe)
			{
				auto IterPipe = Pipeline_s.find(pipe);
				fout << IterPipe->second;
			}
			for (const auto& ks : GtsKs)
			{
				auto IterKs = Ks_s.find(ks);
				fout << IterKs->second;
			}
			fout.close();
			cout << "Done.\n";
		}
		else cout << "An error occured while writing the file.";
	}
	else cout << "The network has not yet been created.\n";
}

void Network::LoadNetwork(unordered_map<int, Pipeline>& Pipeline_s, unordered_map<int, Ks>& Ks_s)
{
	ifstream fin;
	fin.open(AskingForName(), ios::in);
	if (fin.is_open())
	{
		Pipeline_s.erase(Pipeline_s.begin(), Pipeline_s.end());
		Ks_s.erase(Ks_s.begin(), Ks_s.end());
		int numKs, numPipes;
		fin >> numPipes;
		fin >> numKs;
		for (int i = 1; i <= numPipes; i++)
		{
			int prom;
			fin >> prom;
			GtsPipe.emplace(prom);
		}
		for (int i = 1; i <= numKs; i++)
		{
			int prom;
			fin >> prom;
			GtsKs.emplace(prom);
		}
		while (numPipes--)
		{
			Pipeline p;
			fin >> p;
			Pipeline_s.insert(pair<int, Pipeline>(p.GetId(), p));
		}
		while (numKs--)
		{
			Ks k;
			fin >> k;
			Ks_s.insert(pair<int, Ks>(k.GetId(), k));
		}
		fin.close();
		cout << "Done.\n";
	}
	else cout << "File with this name does not exist.\n";
}

void Network::DFS(int start, vector<int>& color, stack <int>& answer_stack)
{
	color[start] = 1;
	for (const auto& ver : mGtsKs)
	{
		if (network[make_pair(start, ver.first)] != 0 && (color[ver.first] == 0))
		{
			DFS(ver.first, color, answer_stack);
		}
		else if (network[make_pair(start, ver.first)] != 0 && (color[ver.first] == 1))
			cycle_found = true;
	}
	color[start] = 2;
	answer_stack.push(start);
}

void Network::TopolSort(const unordered_map<int, Pipeline>& Pipeline_s)
{
	if (NetworkExist)
	{
		stack <int> answer_stack;
		vector<int> color;
		unordered_map<int, int> answer;
		color.clear();
		answer.clear();
		color.resize(GtsKs.size());
		for (const auto& ver : mGtsKs)
			(color[ver.first] == 0);
		while (!answer_stack.empty())
			answer_stack.pop();
		int koren = -1;
		for (const auto& line : mGtsKs)
		{
			int summa = 0;
			for (const auto& column : mGtsKs)
			{
				summa += network[make_pair(column.first, line.first)];
			}
			bool not_isolated = false;
			for (const auto& p : Pipeline_s)
			{
				if ((p.second.OutputId == line.second) || (p.second.InputId == line.second))
					not_isolated = true;
			}
			if ((summa == 0) && (not_isolated == true))
			{
				koren = line.first;
				break;
			}
		}
		if (koren == -1)
		{
			cout << "\nTopological sort is impossible. Graph has a cycle. KOREN\n";
			return;
		}
		else
		{
			DFS(koren, color, answer_stack);
			if (cycle_found)
			{
				cout << "\nTopological sort is impossible. Graph has a cycle.\n";
				return;
			}
			else
			{
				int i = 1;
				while (!answer_stack.empty())
				{
					auto iter = mGtsKs.find(answer_stack.top());
					answer.insert(pair<int, int>(i, iter->second));
					answer_stack.pop();
					i++;
				}
				cout << "\nTopological sort is:\n";
				for (const auto& sort : answer)
					cout << sort.first << " - " << sort.second << endl;
			}
		}
	}
	else cout << "The network has not yet been created.\n";
}

void Network::FindWay(int vertex, stack<int>& way, const vector <int>& distance)
{
	way.push(vertex);
	if (distance[vertex] == 0)
	{
		return;
	}
	else
		for (int i = 0; i <= (int)GtsKs.size() - 1; i++)
		{
			if (distance[vertex] == network[make_pair(i, vertex)])
			{
				way.push(i);
				return;
			}
			else if ((distance[vertex] - network[make_pair(i, vertex)] == distance[i]) && i != vertex)
				FindWay(i, way, distance);
		}
}

void Network::ShortDist()
{
	if (NetworkExist)
	{
		int start = FindVertex("\nPlease, enter correct number of the vertex to calculate the path from: ");
		int end = FindVertex("\nPlease, enter correct number of the vertex to calculate the path to: ");
		if ((start == -1) || (end == -1))
		{
			cout << "The number of the vertex is incorrect.\n";
			return;
		}
		else
		{
			vector <int> distance;
			vector <int> visited;
			stack <int> way;
			while (!way.empty())
				way.pop();
			distance.reserve(GtsKs.size());
			visited.reserve(GtsKs.size());
			for (int i = 0; i <= (int)GtsKs.size() - 1; i++)
			{
				distance.push_back(INT_MAX);
				visited.push_back(0);
			}
			int indexstart = 0;
			int indexend = 0;
			for (const auto& m : mGtsKs)
			{
				if (m.second == start)
					indexstart = m.first;
				if (m.second == end)
					indexend = m.first;
			}
			distance[indexstart] = 0;
			int index = 0;
			for (int count = 0; count < (int)GtsKs.size() - 1; count++)
			{
				int min = INT_MAX;
				for (int i = 0; i < (int)GtsKs.size(); i++)
					if (!visited[i] && distance[i] <= min)
					{
						min = distance[i];
						index = i;
					}
				visited[index] = 1;
				for (int i = 0; i < (int)GtsKs.size(); i++)
					if (!visited[i] && network[make_pair(index, i)] && distance[index] != INT_MAX &&
						distance[index] + network[make_pair(index, i)] < distance[i])
						distance[i] = distance[index] + network[make_pair(index, i)];
			}
			cout << "\nThe shorterst way:\n\n";
			if (distance[indexend] != INT_MAX)
			{
				cout << start << " > " << end << " = " << distance[indexend];
				cout << endl;
				if (distance[indexend] != 0)
				{
					cout << "Way: ";
					FindWay(indexend, way, distance);
					auto iter = mGtsKs.find(way.top());
					way.pop();
					cout << iter->second;
					while (!way.empty())
					{
						iter = mGtsKs.find(way.top());
						way.pop();
						cout << "->" << iter->second;
					}
					cout << endl;
				}
				cout << endl;
			}
			else cout << start << " > " << end << " = " << "there is no way (infinity)" << endl << endl;
		}
	}
	else cout << "The network has not yet been created.\n";
}

int Network::FindVertex(string str)
{
	vector <int> vershina;
	vershina.reserve(GtsKs.size());
	for (const auto& v : GtsKs)
		vershina.push_back(v);
	int vertex = GetCorrectNumber(1, Ks::MaxId, str);
	auto got = find(vershina.begin(), vershina.end(), vertex);
	if (got == vershina.end())
	{
		return -1;
	}
	return vertex;
}

int Network::BFS(int stvertex, int endvertex, vector <int>& way, map <pair<int, int>, int>& flow)
{
	vector<int> color;
	color.clear();
	color.resize(GtsKs.size());
	for (int i = 0; i <= (int)GtsKs.size() - 1; i++)
		color[i]=0;
	queue<int> q;
	q.push(stvertex);     
	color[stvertex] = 1;
	way[stvertex]=-1;
	while (!q.empty())
	{
		int prom = q.front();
		color[prom] = 2;
		q.pop();
		for (int vershina = 0; vershina <= (int)GtsKs.size() - 1; vershina++)
		{
			if (color[vershina] == 0 && (throughputs[make_pair(prom, vershina)] - flow[make_pair(prom, vershina)]) > 0)
			{
				q.push(vershina);
				color[vershina] = 1;
				way[vershina] = prom;		
			}
		}
	}
	if (color[endvertex] == 2)
		return 0;
	else 
		return 1;
}

void Network::MaxFlow()
{ 
	//�������� �����-����������
	if (NetworkExist)
	{
		int source = FindVertex("\nPlease, enter correct number of the vertex to calculate the flow from (source): ");
		int stock = FindVertex("\nPlease, enter correct number of the vertex to calculate the flow to(stock): ");
		if ((source == -1) || (stock == -1) || (stock ==source))
			cout << "The number of the vertex is incorrect.\n";
		else
		{
			int maxflow = 0;
			vector <int> way;
			way.clear();
			way.resize(GtsKs.size());
			map <pair<int, int>, int> flow;
			for (const auto& line : mGtsKs)
			{
				for (const auto& column : mGtsKs)
				{
					flow.emplace(make_pair(line.first, column.first), 0);
				}
			}
			int indexsource = 0;
			int indexstock = 0;
			for (const auto& m : mGtsKs)
			{
				if (m.second == source)
					indexsource = m.first;
				if (m.second == stock)
					indexstock = m.first;
			}
			while (!BFS(indexsource, indexstock, way, flow))
			{
				int lyambda = INT_MAX;
				int vershina = indexstock;
				while (way[vershina] != -1)
				{
					lyambda = min(lyambda, throughputs[make_pair(way[vershina], vershina)] - flow[make_pair(way[vershina], vershina)]);
					vershina = way[vershina];
				}
				vershina = indexstock;
				while (way[vershina] != -1)
				{
					flow[make_pair(vershina, way[vershina])] -= lyambda;
					flow[make_pair(way[vershina], vershina)] += lyambda;
					vershina = way[vershina];
				}
				maxflow += lyambda;
			}
			//cout << "\nParticipating vertices:\n";
			//for (const auto& line : mGtsKs)
			//{
			//	for (const auto& column : mGtsKs)
			//	{
			//		if (throughputs[make_pair(line.first, column.first)] == flow[make_pair(line.first, column.first)] && throughputs[make_pair(line.first, column.first)] != 0)
			//		{
			//			cout << line.second << "->" << column.second << endl;
			//		}
			//	}
			//}
			cout << "\nThe maximum flow between " << source << " and " << stock << " is: " << maxflow << endl;
		}
	}
	else cout << "The network has not yet been created.\n";
}
