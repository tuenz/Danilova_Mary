﻿//исправления: vector->unordered_map
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <unordered_map>
#include <vector>
#include "Pipeline.h"
#include "Ks.h"
#include "Utils.h"
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
		<< "0. Exit.\n";
}

string AskingForName()
{
	string filename;
	cout << "\nPlease, enter the name: ";
	cin.ignore(1, '\n');
	getline(cin, filename);
	return filename;
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

template<class C, typename T>
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
	return percent <= (double)(100*(k.ceh-k.cehwork)/k.ceh);
}
template<class C, typename T>
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

void BatchEditPipes(unordered_map<int, Pipeline>& Pipeline_s)
{
	// информация взята с сайта
	//https://www.cplusplus.com/reference/unordered_map/unordered_map/find/
	int choice;
	while ((choice = GetCorrectNumber(-1, INT_MAX,
		"\nPlease, select correct id of the pipeline you want to edit; enter -1 to stop and go to menu: ")) != -1)
	{
		unordered_map<int, Pipeline>::iterator got = Pipeline_s.find(choice);
		if (got == Pipeline_s.end())
			cout << "Object not found.\n";
		else
		{
			got->second.EditPipeline();
			cout << "Done.\n";
		}
	}
}
void EditAllPipes(unordered_map <int, Pipeline>& Pipeline_s)
{
	for (auto& p : Pipeline_s)
	{
		p.second.EditPipeline();
	}
	cout << "Done.\n";
}
template <class C>
void DelPipeKs(unordered_map<int, C>& PK_s)
{
	typename unordered_map<int, C>::iterator got = PK_s.find(GetCorrectNumber(0, INT_MAX,
		"\nPlease, select correct id of the object you want to delete: "));
	//https://www.cyberforum.ru/cpp-beginners/thread2482067.html
	if (got == PK_s.end())
		cout << "Object not found.\n";
	else
	{
		PK_s.erase(got->first);
		cout << "Done.\n";
	}
}

int main()
{
	unordered_map <int, Pipeline> Pipeline_s;
	unordered_map <int, Ks> Ks_s;
	for ( ; ; ) 
	{
		PrintMenu();
		switch (GetCorrectNumber(0, 12, "Please, select a number from 0 to 12.\n"))
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
				DelPipeKs(Pipeline_s);
			}
			else cout << "Pipe information has not been received yet. Input the data by selecting 1 or 5 points.\n ";
			break;
		}
		case 4:
		{
			if (Ks_s.size())
			{
				DelPipeKs(Ks_s);
			}
			else cout << "Ks information has not been received yet. Input the data by selecting 2 or 5 points.\n ";
			break;
		}
		case 5:
		{
			LoadAll(Pipeline_s, Ks_s);
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
					BatchEditPipes(Pipeline_s);
				}
				else
				{
					EditAllPipes(Pipeline_s);
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
		case 0:
		{
			return 0;
			break;
		}
		}
    }
}