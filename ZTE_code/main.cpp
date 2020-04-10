#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#define MAX 99999999

using namespace std;

//全局信息
struct Global_info {
	int station_num;	//车站数量
	int track_num;		//轨道数量
	int train_num;		//列车数量
	int train_capacity;	//单个列车容量
	int goods_num;		//货物数量
};

//货物信息
struct goods_info {
	string station_1;		//站点1
	string station_2;		//站点2
	double weight;		//重量
	vector<string> Through_stations;	//必经站点序列
};

//config
struct Config {
	//全局信息
	Global_info Global;

	//车站信息
	map<string, int> Station_info;		//key为站点,value拣货员数量

	//轨道信息
	map<string, pair<string, string>> Track_info;	//轨道，站点1，站点2

	//货物信息
	map<string, goods_info> Goods_info;
};

//func
inline void init(Config &config);		//读取输入

int main()
{
	//data
	Config config;

	//input
	init(config);

	//process
	//cout << config.Goods_info.size() << endl;

	//output
	//全部无效，计算总重量
	double sum = 0;
	for (auto it = config.Goods_info.begin(); it != config.Goods_info.end(); it++)
	{
		sum += it->second.weight;
	}
	cout << int(config.Goods_info.size()) << "," << fixed << setprecision(6) << sum << endl;
	for (auto it = config.Goods_info.begin(); it != config.Goods_info.end(); it++)
	{
		cout << it->first << endl;
		cout << "null" << endl;
		cout << "null" << endl;
	}

	return 0;
}

inline void init(Config &config)
{

	string lineStr, str;
	config.Global.goods_num = MAX;

	//读取第一行
	getline(cin, lineStr);		//每次读入一整行，知道遇到换行符结束读入
	stringstream tmp_ss(lineStr);
	int count = 0;
	while (getline(tmp_ss, str, ','))
	{
		++count;
		if (count == 1) config.Global.station_num = atoi(str.c_str());
		if (count == 2) config.Global.track_num = atoi(str.c_str());
		if (count == 3)	config.Global.train_num = atoi(str.c_str());
		if (count == 4)	config.Global.train_capacity = atoi(str.c_str());
	}

	count = 0;
	int sub_sum = config.Global.station_num + config.Global.track_num;
	int sum = sub_sum + config.Global.goods_num;
	string num_1, num_2, num_3;
	double num_4;
	while (getline(cin, lineStr))
	{
		++count;

		stringstream ss(lineStr);
		int sub_count = 0;
		while (getline(ss, str, ','))
		{
			//车站信息
			if (count <= config.Global.station_num)
			{
				++sub_count;
				if (sub_count == 1) num_1 = str;
				if (sub_count == 2) config.Station_info[num_1] = atoi(str.c_str());
			}
			else
			{
				//轨道信息
				if (count > config.Global.station_num && count <= sub_sum)
				{
					++sub_count;
					if (sub_count == 1) num_1 = str;
					if (sub_count == 2) num_2 = str;
					if (sub_count == 3)
					{
						num_3 = atoi(str.c_str());
						config.Track_info[num_1] = pair<string, string>(num_2, str);
					}
				}
				else
				{
					//货物信息
					if (count == sub_sum + 1)
					{
						config.Global.goods_num = atoi(str.c_str());
						sum = sub_sum + config.Global.goods_num + 1;
					}
					else
					{
						++sub_count;
						if (sub_count == 1) num_1 = str;
						if (sub_count == 2) num_2 = str;
						if (sub_count == 3) num_3 = str;
						if (sub_count == 4) num_4 = atof(str.c_str());
						if (sub_count > 4)
						{
							config.Goods_info[num_1].station_1 = num_2;
							config.Goods_info[num_1].station_2 = num_3;
							config.Goods_info[num_1].weight = num_4;
							if (str == "null");
							else
							{
								config.Goods_info[num_1].Through_stations.push_back(str);
							}
						}
					}
				}
			}
		}

		if (count == sum) break;
	}
}