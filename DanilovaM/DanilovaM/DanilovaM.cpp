#include <iostream>
using namespace std;

struct Pipeline
{
	int diameter;
	int length;
};

Pipeline InputPipeline()
{
	Pipeline p;
	cout << "Input diameter:";
	cin >> p.diameter;
	cout << "Input length:";
	cin >> p.length;
	return p;
}
void PrintPipeline(const Pipeline& p)
{
	cout << "\nDiameter: " << p.diameter << "\tLength: " << p.length << endl;
}
void EditPipeline(Pipeline& p)
{
	p.diameter -= 1;
}
int main()
{
	Pipeline pipe = InputPipeline();
	EditPipeline(pipe);
	PrintPipeline(pipe);
	return 0;
}


