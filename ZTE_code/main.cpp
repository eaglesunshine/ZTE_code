#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <set>
#include <tuple>
#include <queue>
#include <stack>
#include <list>

#define MAX 99999999

using namespace std;

/*###############Dijkstra###################*/
const double INF_MAX = 9999999.9;

double distTo[1000000];
list<int> edgeTo[1000000];
int V, E, coutSP = -1;
map<int, vector<tuple<int, int, double>>> EWD;

struct GreaterThanByDist
{
	bool operator()(const int i, const int j) const
	{
		return distTo[i] > distTo[j];
	}
};
priority_queue<int, vector<int>, GreaterThanByDist> Minpq;

/*###############Dijkstra###################*/

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
	vector<int> trains;		//传输列车路径
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

	//车站字符串id映射成数字id
	map<string, int> Indexs_to_int;

	//车站数字id映射成字符串id
	map<int, string> Indexs_to_string;
};

//func
inline void init(Config &config, set<string> &Priority, set<string> &DisPriority);		//读取输入
inline void output(Config &config);		//输出路径
inline void process(Config &config, set<string> &Priority, set<string> &DisPriority);	//规划路径
inline void processPriority(set<string> &Priority, Config &config);	//规划有必经之路的货物任务
inline void processDisPriority(set<string> &DisPriority, Config &config);	//规划没有有必经之路的货物任务

/*###############Dijkstra###################*/
inline void generateEWD(Config &config);
inline int  GetNodeNum(string key, map<string, int> &mapNodes);
inline void transferPath(Config &config, vector<int> &new_path, vector<string> &path);
inline void transferTrackPath(Config &config, vector<string> &new_path, vector<string> &path);

inline void relax(tuple<int, int, double> edge);
inline void DijkstraSP(int s, int V);
inline void dfs(int source, int vertex, stack<int> &path);
inline void computeSP(int source, int vertex, vector<vector<int>> &path_all);
inline void showEWD();
/*###############Dijkstra###################*/

int main()
{
	//computeSP(source, vertex);

	//data
	Config config;
	set<string> Priority;
	set<string> DisPriority;

	//input
	init(config, Priority, DisPriority);

	//process
	generateEWD(config);

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
	//do nothing
}

//规划没有有必经之路的货物任务
inline void processDisPriority(set<string> &DisPriority, Config &config)
{
	int max = 100;
	int count = 0;
	string start, end;
	int v, w;
	vector<vector<int>> path_all;
	vector<string> path;
	vector<string> track_path;
	for (auto it = DisPriority.begin(); it != DisPriority.end(); it++)
	{
		++count;

		//取起点、终点
		start = config.Goods_info[*it].station_1;
		end = config.Goods_info[*it].station_2;
		v = config.Indexs_to_int[start];
		w = config.Indexs_to_int[end];

		//计算最短路径
		computeSP(v, w, path_all);

		//存储最短路径
		transferPath(config, path_all[0], path);
		transferTrackPath(config, path, track_path);
		config.Goods_info[*it].stations.insert(config.Goods_info[*it].stations.begin(), track_path.begin(), track_path.end());
		for (int i = 0; i < int(track_path.size()); i++)
		{
			config.Goods_info[*it].trains.push_back(1);
		}

		if (count == max) break;
	}
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
			if (isfirst)
			{
				cout << *iter;
				isfirst = false;
			}
			else
			{
				cout << "," << *iter;
			}
		}
		cout << endl;
		isfirst = true;
		for (auto iter = config.Goods_info[*it].trains.begin(); iter != config.Goods_info[*it].trains.end(); iter++)
		{
			if (isfirst)
			{
				cout << *iter;
				isfirst = false;
			}
			else
			{
				cout << "," << *iter;
			}
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
				if (sub_count == 2)
				{
					config.Station_people[num_1] = atoi(str.c_str());

					//建立车站字符串id到数字id的映射
					int id_int = GetNodeNum(num_1, config.Indexs_to_int);
					config.Indexs_to_string[id_int] = num_1;
				}
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

/*###############Dijkstra###################*/
inline void relax(tuple<int, int, double> edge)
{
	int v = get<0>(edge);
	int w = get<1>(edge);
	double weight = get<2>(edge);
	if (distTo[w] > distTo[v] + weight) {
		distTo[w] = distTo[v] + weight;
		edgeTo[w].clear();
		edgeTo[w].push_back(v);
		Minpq.push(w);
	}
	else if (distTo[w] == distTo[v] + weight)
	{
		edgeTo[w].push_back(v);
	}
}

inline void DijkstraSP(int s, int V)
{
	for (int v = 0; v < V; v++)
		distTo[v] = INF_MAX;
	distTo[s] = 0.0;

	Minpq.push(s);
	while (!Minpq.empty())
	{
		int v = Minpq.top();
		Minpq.pop();
		for (vector<tuple<int, int, double>>::iterator ii = EWD[v].begin();
			ii != EWD[v].end();
			ii++)
		{
			relax(*ii);
		}
	}
}

inline void dfs(int source, int vertex, stack<int> &path)
{
	coutSP++;
	if (vertex == source)
	{
		coutSP = 0;
		return;
	}
	for (list<int>::iterator ii = edgeTo[vertex].begin(); ii != edgeTo[vertex].end(); ii++)
	{
		if (coutSP == 0)  path.push(vertex);
		path.push(*ii);
		dfs(source, *ii, path);
	}
}

inline void computeSP(int source, int vertex, vector<vector<int>> &path_all)
{
	path_all.clear();

	DijkstraSP(source, V);

	stack<int> path;
	//showEWD();
	dfs(source, vertex, path);

	vector<int> new_path;
	while (!path.empty())
	{
		new_path.push_back(path.top());
		if (path.top() == source)
		{
			new_path.clear();			
		}
		if (path.top() == vertex)
		{
			path_all.push_back(new_path);
		}		
		path.pop();
	}

}

inline void showEWD()
{
	cout << "EdgeWeightedDigraph : " << endl;
	for (int v = 0; v < V; v++)
	{
		cout << v << " : ";
		for (vector<tuple<int, int, double>>::iterator ii = EWD[v].begin();
			ii != EWD[v].end();
			ii++)
		{
			cout << get<0>(*ii) << "->" << get<1>(*ii) << " " << get<2>(*ii) << "  ";
		}
		cout << endl;
	}
}

inline void generateEWD(Config &config)
{
	string start, end;
	int v, w;
	double weight;
	for (auto it = config.Track_to_stations.begin(); it != config.Track_to_stations.end(); it++)
	{
		int count = 0;
		for (auto iter = it->second.begin(); iter != it->second.end(); iter++)
		{
			++count;
			if (count == 1) start = *iter;
			else end = *iter;
		}

		//字符串id映射到数字id
		v = config.Indexs_to_int[start];
		w = config.Indexs_to_int[end];

		weight = 1.0;
		EWD[v].push_back(make_tuple(v, w, weight));
		EWD[w].push_back(make_tuple(w, v, weight));
	}

	V = int(config.Station_people.size());
	E = int(config.Track_to_stations.size());
}

inline int  GetNodeNum(string key, map<string, int> &mapNodes) {

	map<string, int>::iterator iter;

	iter = mapNodes.find(key);

	if (iter != mapNodes.end()) {
		return mapNodes[key];
	}
	else {
		int nodeNum = int(mapNodes.size());
		mapNodes.insert(std::pair<std::string, int>(key, nodeNum));
		return nodeNum;
	}

}

inline void transferPath(Config &config, vector<int> &new_path, vector<string> &path)
{
	path.clear();

	for (auto it = new_path.begin(); it != new_path.end(); it++)
	{
		path.push_back(config.Indexs_to_string[*it]);
	}
}

inline void transferTrackPath(Config &config, vector<string> &new_path, vector<string> &path)
{
	path.clear();

	set<string> tmp_set;
	for (int i = 0; i<int(new_path.size() - 1); i++)
	{
		tmp_set.clear();
		tmp_set.insert(new_path[i]);
		tmp_set.insert(new_path[i + 1]);

		path.push_back(config.Station_to_tracks[tmp_set]);
	}
}
/*###############Dijkstra###################*/