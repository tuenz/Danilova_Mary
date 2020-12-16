#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <unordered_map>
#include <map>
#include <set> 
#include "Pipeline.h"
#include "Ks.h"
#include "Utils.h"
#include "Network.h"
using namespace std;

void PrintMenu()
{
	cout << "\n1. Add pipeline information.\n"
		<< "2. Add ks information.\n"
		<< "3. Delete pipeline information.\n"
		<< "4. Delete ks information.\n"
		<< "5. Load pipeline and ks information from file.\n"
		<< "6. View information about all pipelines.\n"
		<< "7. View information about all ks.\n"
		<< "8. Save all information to file.\n"
		<< "9. Launch or stop ks ceh.\n"
		<< "10. Batch editing of pipelines.\n"
		<< "11. Pipelines search by filter.\n"
		<< "12. Ks search by filter.\n"
		<< "13. Choose parts for transmission network.\n"
		<< "14. Create/update transmission network.\n"
		<< "15. Save transmission network to file.\n"
		<< "16. Load transmission network from file.\n"
		<< "17. Show graph adjacency table.\n"
		<< "18. Topological sort of graph.\n"
		<< "19. Find the shortest way.\n"
		<< "20. Find the maximum flow.\n"
		<< "0. Exit.\n";
}


void LoadAll(unordered_map<int, Pipeline>& Pipeline_s, unordered_map <int, Ks>& Ks_s)
{
	ifstream fin;
	fin.open(AskingForName(), ios::in);
	if (fin.is_open())
	{
		Pipeline_s.erase(Pipeline_s.begin(), Pipeline_s.end());
		Ks_s.erase(Ks_s.begin(), Ks_s.end());
		int num_pipelines, num_ks;
		fin >> num_pipelines;
		fin >> num_ks;
		while (num_pipelines--)
		{
			Pipeline p;
			fin >> p;
			Pipeline_s.insert(pair<int, Pipeline>(p.GetId(), p));
		}
		while (num_ks--)
		{
			Ks k;
			fin >> k;
			Ks_s.insert(pair<int, Ks>(k.GetId(), k));
		}
		fin.close();
		for (auto& p : Pipeline_s)
		{
			p.second.OutputId = 0;
			p.second.InputId = 0;
		}
		cout << "Done.\n";
	}
	else cout << "File with this name does not exist.\n";
}

void SaveAll(const unordered_map<int, Pipeline>& Pipeline_s, const unordered_map <int, Ks>& Ks_s)
{
	if ((!Pipeline_s.size()) and (!Ks_s.size()))
		cout << "Pipeline and ks information has not been received yet. Input the data by selecting 1, 2 or 5 points.\n ";
	else
	{
		ofstream fout;
		fout.open(AskingForName(), ios::out);
		if (fout.is_open())
		{
			fout << Pipeline_s.size() << endl;
			fout << Ks_s.size() << endl;
			for (const auto& p : Pipeline_s)
				fout << p.second;
			for (const auto& k : Ks_s)
				fout << k.second;
			fout.close();
			cout << "Done.\n";
		}
		else cout << "An error occured while writing the file.";
	}
}

template<typename C, typename T>
using Filter = bool(*)(const C& pk, T param);
bool CheckByRepear(const Pipeline& p, bool repear)
{
	return p.repear == repear;
}
bool CheckByName(const Ks& k, string name)
{
	return k.name == name;
}
bool CheckBy_Percent_Unused_Ceh(const Ks& k, double percent)
{
	return percent <= (double)(100 * (k.ceh - k.cehwork) / k.ceh);
}
template<typename C, typename T>
vector<int> FindObjectsByFilter(const unordered_map<int, C>& PK_s, Filter<C, T> f, T param)
{
	vector <int> res;
	int i = 0;
	for (const auto& object : PK_s)
	{
		if (f(object.second, param))
			res.push_back(i);
		i++;
	}
	if (res.size() == 0) cout << "Object not found.\n";
	return res;
}
void BatchEditPipes(unordered_map<int, Pipeline>& Pipeline_s, Network& n)
{
	int choice;
	while ((choice = GetCorrectNumber(-1, Pipeline::MaxId,
		"\nPlease, select correct id of the pipeline you want to edit; enter -1 to stop and go to menu: ")) != -1)
	{
		auto got = Pipeline_s.find(choice);
		if (got == Pipeline_s.end())
			cout << "Object not found.\n";
		else
		{
			got->second.EditPipeline();
			got->second.InputId = 0;
			got->second.OutputId = 0;
			cout << "Done.\n";
			n.PipeDelChanges(choice);
			n.CreateNetwork(Pipeline_s);
		}
	}
}

void EditAllPipes(unordered_map <int, Pipeline>& Pipeline_s, Network& n)
{
	for (auto& p : Pipeline_s)
	{
		p.second.EditPipeline();
		p.second.InputId = 0;
		p.second.OutputId = 0;
	}
	cout << "Done.\n";
	n.GtsPipe.clear();
}

void DelPipe(unordered_map<int, Pipeline>& Pipeline_s, Network& n)
{
	int IdDel = GetCorrectNumber(0, Pipeline::MaxId,
		"\nPlease, select correct id of the object you want to delete: ");
	auto IdToDelete = Pipeline_s.find(IdDel);
	if (IdToDelete == Pipeline_s.end())
		cout << "Object not found.\n";
	else
	{
		n.PipeDelChanges(IdDel);
		n.CreateNetwork(Pipeline_s);
		Pipeline_s.erase(IdToDelete->first);
		cout << "Done.\n";
	}
}

void DelKs(unordered_map<int, Ks>& Ks_s, unordered_map <int, Pipeline>& Pipeline_s, Network& n)
{
	int IdDel = GetCorrectNumber(0, Ks::MaxId,
		"\nPlease, select correct id of the object you want to delete: ");
	auto IdToDelete = Ks_s.find(IdDel);
	if (IdToDelete == Ks_s.end())
		cout << "Object not found.\n";
	else
	{
		n.KsDelChanges(IdDel, Pipeline_s);
		n.CreateNetwork(Pipeline_s);
		Ks_s.erase(IdToDelete->first);
		cout << "Done.\n";
	}
}

int main()
{
	unordered_map <int, Pipeline> Pipeline_s;
	unordered_map <int, Ks> Ks_s;
	Network n;
	for (; ; ) {
		PrintMenu();
		switch (GetCorrectNumber(0, 20, "Please, select a number from 0 to 20.\n"))
		{
		case 1:
		{
			Pipeline p;
			cin >> p;
			Pipeline_s.insert(pair<int, Pipeline>(p.GetId(), p));
			break;
		}
		case 2:
		{
			Ks k;
			cin >> k;
			Ks_s.insert(pair<int, Ks>(k.GetId(), k));
			break;
		}
		case 3:
		{
			if (Pipeline_s.size())
			{
				DelPipe(Pipeline_s, n);
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
			break;
		}
		case 4:
		{
			if (Ks_s.size())
			{
				DelKs(Ks_s, Pipeline_s, n);
			}
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		case 5:
		{
			LoadAll(Pipeline_s, Ks_s);
			n.NetworkExist = false;
			break;
		}
		case 6:
		{
			if (Pipeline_s.size())
			{
				for (const auto& p : Pipeline_s) cout << p.second << endl;
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
			break;
		}
		case 7:
		{
			if (Ks_s.size())
			{
				for (const auto& k : Ks_s) cout << k.second << endl;
			}
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		case 8:
		{
			SaveAll(Pipeline_s, Ks_s);
			break;
		}
		case 9:
		{
			if (Ks_s.size())
			{
				unordered_map<int, Ks>::iterator got = Ks_s.find(GetCorrectNumber(0, INT_MAX,
					"\nPlease, select correct id of the ks you want to edit: "));
				if (got == Ks_s.end())
					cout << "Object not found.\n";
				else
				{
					got->second.EditKs(GetCorrectNumber(0, 1, "Please, input 1 for launch or 0 for stop: "));
					cout << "Done.\n";
				}
			}
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		case 10:
		{
			if (Pipeline_s.size())
			{
				if (GetCorrectNumber(0, 1, "Please, input 0 to edit all pipelines or 1 to batch egiting of pipelines: "))
				{
					BatchEditPipes(Pipeline_s, n);
				}
				else {
					EditAllPipes(Pipeline_s, n);
				}
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
			break;
		}
		case 11:
		{
			if (Pipeline_s.size())
			{
				if (GetCorrectNumber(0, 1, "Please, input 0 for pipeline search by id or 1 for pipeline search by attribute 'in repear': "))
				{
					for (int i : FindObjectsByFilter(Pipeline_s, CheckByRepear, (bool)GetCorrectNumber(-1, 1,
						"\nPlease, enter 0 to find pipelines not in repear and 1 to find pipelines in repear: ")))
						cout << Pipeline_s[i];
				}
				else
				{
					unordered_map<int, Pipeline>::iterator got = Pipeline_s.find(GetCorrectNumber(0, INT_MAX, "\nPlease, select correct id of the pipeline you want to find: "));
					if (got == Pipeline_s.end())
						cout << "Object not found.\n";
					else
					{
						cout << got->second;
					}
				}
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
			break;
		}
		case 12:
		{
			if (Ks_s.size())
			{
				if (GetCorrectNumber(0, 1, "Please, input 0 for ks search by name or 1 for ks search by the percentage of unused cehworks: "))
				{
					for (int i : FindObjectsByFilter(Ks_s, CheckBy_Percent_Unused_Ceh, GetCorrectNumber(0.0, 100.0,
						"\nPlease, enter the percentage of unused cehworks(%): ")))
						cout << Ks_s[i];
				}
				else {
					for (int i : FindObjectsByFilter(Ks_s, CheckByName, AskingForName()))
						cout << Ks_s[i];
				}
			}
			else  cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		case 13:
		{
			n.CreateConnection(Pipeline_s, Ks_s);
			break;
		}
		case 14:
		{
			n.CreateNetwork(Pipeline_s);
			break;
		}
		case 15:
		{
			n.SaveNetwork(Pipeline_s, Ks_s);
			break;
		}
		case 16:
		{
			n.LoadNetwork(Pipeline_s, Ks_s);
			n.CreateNetwork(Pipeline_s);
			break;
		}
		case 17:
		{
			n.PrintNetwork();
			break;
		}
		case 18:
		{
			n.TopolSort(Pipeline_s);
			break;
		}
		case 19:
		{
			n.ShortDist();
			break;
		}		
		case 20:
		{
			n.MaxFlow();
			break;
		}
		case 0:
		{
			return 0;
			break;
		}
		}
	}
}