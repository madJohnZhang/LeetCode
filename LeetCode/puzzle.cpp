#include<iostream>
#include<vector>
#include<stack>
#include<unordered_map>
#include<algorithm>
#include<unordered_set>
#include<fstream>
#include<time.h>
#include<set>
#include<queue>

using namespace std;

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3
#define N 3

#define GREEDY 0
#define OUR 1
#define MANHATTAN 2

int changeId[9][4] = { { -1,-1,3,1 },{ -1,0,4,2 },{ -1,1,5,-1 },
{ 0,-1,6,4 },{ 1,3,7,5 },{ 2,4,8,-1 },
{ 3,-1,-1,7 },{ 4,6,-1,8 },{ 5,7,-1,-1 }
};										//up,left,down,right

struct point
{
	int x;
	int y;
};

class puzzles
{
public:
	int index;
	vector<int> nums;
	puzzles() :nums(vector<int>(9)) {}
	puzzles(int i) { index = i; }
};

class node
{
public:
	double cost;
	int gx;
	puzzles p;
	vector<int> path;

	node(double cost, puzzles p, int gx) :cost(cost + gx), p(p), gx(gx) {}
	bool operator < (const node &b) const { return cost > b.cost; }
	void addpath(node father, int flag) { path = father.path; path.push_back(flag); }
};

bool isTarget(vector<int> &nums);
bool isEqual(vector<int> &a, vector<int> &b);
bool isVisited(vector<puzzles> &visitedArrays, vector<int> &nums);
puzzles move(puzzles item,int flag);
void copy(puzzles &from, puzzles &to);
bool isValid(vector<int> &nums);
puzzles generate();
void astar();
double caldis(puzzles start, int mode);
point transform(int index);

int main()
{
	astar();
	clock_t start = clock();
	vector<puzzles> visitedArrays;
	
	int depth;
	cin >> depth;
	puzzles init = generate();
	if (!isValid(init.nums))
	{
		cout << "none solution" << endl;
		return 0;
	}
	vector<int> target = {1,2,3,4,5,6,7,8,0};
	ofstream fout("actions.txt");
	bool find = false;
	for (int i = 0; i < depth; i++)
	{
		stack<puzzles> deepen;
		stack<int> actions;
		visitedArrays.clear();
		actions.push(-1);
		deepen.push(init);
		int tmp = 1;
		puzzles top;
		puzzles newitem;
		if (find)
			break;
		//cout << i << endl;
		while (!deepen.empty())
		{
			top = deepen.top();
			
			if (isEqual(top.nums,target))
			{
				find = true;
				cout << "sucess" << endl;
				fout << actions.size()<<endl;
				fout << clock() - start << endl;
				break;
			}
			if (tmp > i)
			{
				deepen.pop();
				actions.pop();
				tmp--;
			}
			else if ((newitem = move(top, LEFT)).index != -1 && !isVisited(visitedArrays,newitem.nums))
			{
				deepen.push(newitem);
				visitedArrays.push_back(newitem);
				actions.push(LEFT);
				tmp++;
			}
			else if ((newitem = move(top, UP)).index != -1 && !isVisited(visitedArrays, newitem.nums))
			{
				deepen.push(newitem);
				visitedArrays.push_back(newitem);
				actions.push(UP);
				tmp++;
			}
			else if ((newitem = move(top, RIGHT)).index != -1 && !isVisited(visitedArrays, newitem.nums))
			{
				deepen.push(newitem);
				visitedArrays.push_back(newitem);
				actions.push(RIGHT);
				tmp++;
			}
			else if ((newitem = move(top, DOWN)).index != -1 && !isVisited(visitedArrays, newitem.nums))
			{
				deepen.push(newitem);
				visitedArrays.push_back(newitem);
				actions.push(DOWN);
				tmp++;
			}
			else
			{
				deepen.pop();
				actions.pop();
				tmp--;
			}
		}
	}
	fout.close();
}

bool isTarget(vector<int> &nums)
{
	for (int i = 0; i < 8; i++)
	{
		if (nums[i] != (i + 1))
			return 0;
	}
	return 1;
}

bool isEqual(vector<int> &a, vector<int> &b)
{
	for (int i = 0; i < 8; i++)
	{
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}

bool isVisited(vector<puzzles>& visitedArrays, vector<int> &nums)
{
	int size = visitedArrays.size();
	for (int i = 0; i < size; i++)
	{
		if (isEqual(visitedArrays[i].nums, nums))
		{
			return true;
		}
	}
	return false;
}

inline puzzles move(puzzles item, int flag)
{
	int id = changeId[item.index][flag];
	if (id == (-1))
		return puzzles(-1);
	int tmp = item.nums[item.index];
	item.nums[item.index] = item.nums[id];
	item.nums[id] = tmp;
	item.index = id;
	return item;
}

void copy(puzzles & from, puzzles & to)
{
	to.index = from.index;
	for (int i = 0; i < 9; i++)
	{
		to.nums[i] = from.nums[i];
	}
}

bool isValid(vector<int> &nums)
{
	int size = nums.size();
	int sum = 0;
	for (int i = 1; i < size; i++)
	{
		int count=0;
		for (int j = 0; j < i; j++)
		{
			if (nums[j] > nums[i])
			{
				count++;
			}
		}
		sum += count;
	}
	return !(sum % 2);
}

puzzles generate()
{
	int maxMove = 50;
	puzzles res;
	vector<int> init = {1,2,3,4,5,6,7,8,0};
	srand((unsigned)time(NULL));
	while (maxMove--)
	{
		int randx = rand() % 8;
		int randy = rand() % 8;
		swap(init[randx], init[randy]);
	}
	res.nums = init;
	for (int i = 0; i < init.size(); i++)
	{
		if (init[i] == 0)
		{
			res.index = i;
			break;
		}
	}
	return res;
}

void astar()
{
	priority_queue<node> toVisit;
	puzzles root = generate();
	vector<puzzles> visitedArrays;
	int choice = MANHATTAN;
	node tmp(caldis(root,choice), root, 0);
	
	toVisit.push(tmp);

	if (!isValid(root.nums))
	{
		cout << "gen wrong, no answer" << endl;
		return;
	}

	while (1)
	{
		node min = toVisit.top();
		toVisit.pop();
		if (isTarget(min.p.nums))
		{
			for (auto i : min.path)
			{
				cout << i << endl;
			}
			break;
		}
		visitedArrays.push_back(min.p);

		puzzles newitem = move(min.p,UP);
		
		if (newitem.index != -1 && !isVisited(visitedArrays, newitem.nums))
		{
			node tmp = node(caldis(newitem, choice), newitem, min.gx + 1);
			tmp.addpath(min, UP);
			toVisit.push(tmp);
		}
		newitem = move(min.p, LEFT);
		if (newitem.index != -1 && !isVisited(visitedArrays, newitem.nums))
		{
			node tmp = node(caldis(newitem, choice), newitem, min.gx + 1);
			tmp.addpath(min, LEFT);
			toVisit.push(tmp);
		}
		newitem = move(min.p, DOWN); 
		if (newitem.index != -1 && !isVisited(visitedArrays, newitem.nums))
		{
			node tmp = node(caldis(newitem, choice), newitem, min.gx + 1);
			tmp.addpath(min, DOWN);
			toVisit.push(tmp);
		}
		newitem = move(min.p, RIGHT);
		if (newitem.index != -1 && !isVisited(visitedArrays, newitem.nums))
		{
			node tmp = node(caldis(newitem, choice), newitem, min.gx + 1);
			tmp.addpath(min, RIGHT);
			toVisit.push(tmp);
		}

	}
}

double caldis(puzzles start, int mode)
{
	vector<int> s = start.nums;
	double res = 0;
	switch (mode)
	{
	case OUR:		//≈∑ Ωæ‡¿Î
		for (int i = 0; i < s.size(); i++)
		{
			point a = transform(i);
			point b = transform(s[i] - 1);
			res += sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2));
		}
		break;
		
	case MANHATTAN:	//¬¸π˛∂Ÿæ‡¿Î
		for (int i = 0; i < s.size(); i++)
		{
			point a = transform(i);
			point b = transform(s[i] - 1);
			res += abs(a.x - b.x) + abs(a.y - b.y);
		}
		break;
	}
	return res;
}

point transform(int index)
{
	point res;
	if (index != 0)
	{
		res.x = index / 3;
		res.y = index % 3;
	}
	else
	{
		res.x = 2;
		res.y = 2;
	}
	return res;
}