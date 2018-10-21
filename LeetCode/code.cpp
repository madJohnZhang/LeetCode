#include<iostream>
#include<vector>
#include<stack>
#include<unordered_map>
#include<algorithm>

using namespace std;


vector<vector<int>> fourSum(vector<int>& nums, int target);

int main2()
{
	// 1,0,-1,0,-2,2
	vector<int> input({0,0,0,0});
	fourSum(input, 0);
	return 0;
}

/*	LeetCode
		NO. 42 Trapping Rain Water
		Calculate the total water it can trap
*/
int trap(vector<int>& height) {
	//find the top
	int top = 0;
	int trap = 0;
	int size = height.size();
	if (size == 0)
		return trap;
	for (int i = 0; i < size; i++)
	{
		if (height[top] < height[i])
		{
			top = i;
		}
	}

	int init, fill;
	//the left
	init = height[0];
	for (int i = 1; i<top; i++)
	{
		fill = 0;
		if ((height[i]<init))
		{
			fill = init - height[i];
		}
		else
			init = height[i];

		trap += fill;
	}

	//the right
	init = height[size - 1];
	for (int i = size - 2; i>top; i--)
	{
		fill = 0;
		if ((height[i]<init))
		{
			fill = init - height[i];
		}
		else
			init = height[i];

		trap += fill;
	}

	return trap;
}

/*	LeetCode
		No. 18 4Sum
		Find if there are sum of 4 numbers equal to an integer 
*/
vector<vector<int>> fourSum(vector<int>& nums, int target) {
	unordered_map<int,int> table;
	unordered_map<int, int>::iterator it;
	int length = nums.size();
	vector<vector<int>> result;
	sort(nums.begin(),nums.end());
	
	for (int i = 0; i < length; i++)
	{
		table[nums[i]] = i;
	}
	for (int i = 0; i < length; i++)
	{
		vector<int> res;
		for (int j = i+1; j < length; j++)
		{
			for (int k = j+1; k < length; k++)
			{
					it = table.find(target - nums[i] - nums[j] - nums[k]);
					if (it != table.end())
					{
						int tmp = it->second;
						if (tmp>k)
						{
							res.push_back(nums[i]);
							res.push_back(nums[j]);
							res.push_back(nums[k]);
							res.push_back(nums[tmp]);
							result.push_back(res);
							res.clear();
						}
					}
			}
		}
		
	}
	return result;
}