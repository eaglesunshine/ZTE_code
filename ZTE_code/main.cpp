#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <set>

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
	vector<string> stations;	//传输站点路径
	vector<string> trains;		//传输列车路径
};

//config
struct Config {
	//全局信息
	Global_info Global;

	//车站信息
	map<string, int> Station_people;		//key为站点,value拣货员数量

	//轨道所连接的车站
	map<string, set<string>> Track_to_stations;	//轨道，站点1，站点2

	//车站所连接的轨道
	map<set<string>, string> Station_to_tracks;

	//车站的邻接车站
	map<string, set<string>> Station_adj_stations;

	//货物信息
	map<string, goods_info> Goods_info;
};

//func
inline void init(Config &config, set<string> &Priority, set<string> &DisPriority);		//读取输入
inline void output(Config &config);		//输出路径
inline void process(Config &config, set<string> &Priority, set<string> &DisPriority);	//规划路径
inline void processPriority(set<string> &Priority, Config &config);	//规划有必经之路的货物任务
inline void processDisPriority(set<string> &DisPriority, Config &config);	//规划没有有必经之路的货物任务

int main()
{
	//data
	Config config;
	set<string> Priority;
	set<string> DisPriority;

	//input
	init(config, Priority, DisPriority);

	//process
	process(config, Priority, DisPriority);

	//output
	output(config);

	return 0;
}

void process(Config &config, set<string> &Priority, set<string> &DisPriority)
{
	//优先规划有必经之路的货物任务
	processPriority(Priority, config);

	//其次规划没有必经之路的货物任务
	processDisPriority(DisPriority, config);
}

//规划有必经之路的货物任务
inline void processPriority(set<string> &Priority, Config &config)
{
	
}

//规划没有有必经之路的货物任务
inline void processDisPriority(set<string> &DisPriority, Config &config)
{
	//do nothing
}

inline void output(Config &config)
{
	//计算规划失败的货物任务
	set<string>	success_goods;
	set<string> failed_goods;	
	double sum = 0;
	for (auto it = config.Goods_info.begin(); it != config.Goods_info.end(); it++)
	{
		if (it->second.trains.size() > 0) success_goods.insert(it->first);
		else
		{
			sum += it->second.weight;
			failed_goods.insert(it->first);
		}
	}

	//输出第一行：规划失败的货物数量，规划失败的货物总重量
	cout << int(failed_goods.size()) << "," << fixed << setprecision(6) << sum << endl;

	//输出货物任务规划路径
	for (auto it = success_goods.begin(); it != success_goods.end(); it++)	//规划成功的路径
	{
		cout << *it << endl;
		int isfirst = true;
		for (auto iter = config.Goods_info[*it].stations.begin(); iter != config.Goods_info[*it].stations.end(); iter++)
		{
			if (isfirst){
				cout << *iter;
				isfirst = false;
			}
			else cout << "," << *iter;
		}
		cout << endl;
		isfirst = true;
		for (auto iter = config.Goods_info[*it].trains.begin(); iter != config.Goods_info[*it].trains.end(); iter++)
		{
			if (isfirst){
				cout << *iter;
				isfirst = false;
			}
			else cout << "," << *iter;
		}
		cout << endl;
	}
	for (auto it = failed_goods.begin(); it != failed_goods.end(); it++)	//规划失败的路径
	{
		cout << *it << endl;
		cout << "null" << endl;
		cout << "null" << endl;
	}
}

inline void init(Config &config, set<string> &Priority, set<string> &DisPriority)
{

	string lineStr, str;
	config.Global.goods_num = MAX;

	set<string> tmp_set;

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
				if (sub_count == 2) config.Station_people[num_1] = atoi(str.c_str());
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
						tmp_set.clear();
						tmp_set.insert(num_2);
						tmp_set.insert(str);

						//轨道连接的车站
						config.Track_to_stations[num_1].insert(tmp_set.begin(), tmp_set.end());
						
						//车站连接的轨道
						config.Station_to_tracks[tmp_set] = num_1;

						//车站的邻接车站
						config.Station_adj_stations[num_2].insert(str);
						config.Station_adj_stations[str].insert(num_2);
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
							if (str == "null") DisPriority.insert(num_1);
							else
							{
								Priority.insert(num_1);
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