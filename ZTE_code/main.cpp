#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <set>

#define MAX 99999999

using namespace std;

//ȫ����Ϣ
struct Global_info {
	int station_num;	//��վ����
	int track_num;		//�������
	int train_num;		//�г�����
	int train_capacity;	//�����г�����
	int goods_num;		//��������
};

//������Ϣ
struct goods_info {
	string station_1;		//վ��1
	string station_2;		//վ��2
	double weight;		//����
	vector<string> Through_stations;	//�ؾ�վ������
	vector<string> stations;	//����վ��·��
	vector<string> trains;		//�����г�·��
};

//config
struct Config {
	//ȫ����Ϣ
	Global_info Global;

	//��վ��Ϣ
	map<string, int> Station_people;		//keyΪվ��,value���Ա����

	//��������ӵĳ�վ
	map<string, set<string>> Track_to_stations;	//�����վ��1��վ��2

	//��վ�����ӵĹ��
	map<set<string>, string> Station_to_tracks;

	//��վ���ڽӳ�վ
	map<string, set<string>> Station_adj_stations;

	//������Ϣ
	map<string, goods_info> Goods_info;
};

//func
inline void init(Config &config, set<string> &Priority, set<string> &DisPriority);		//��ȡ����
inline void output(Config &config);		//���·��
inline void process(Config &config, set<string> &Priority, set<string> &DisPriority);	//�滮·��
inline void processPriority(set<string> &Priority, Config &config);	//�滮�бؾ�֮·�Ļ�������
inline void processDisPriority(set<string> &DisPriority, Config &config);	//�滮û���бؾ�֮·�Ļ�������

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
	//���ȹ滮�бؾ�֮·�Ļ�������
	processPriority(Priority, config);

	//��ι滮û�бؾ�֮·�Ļ�������
	processDisPriority(DisPriority, config);
}

//�滮�бؾ�֮·�Ļ�������
inline void processPriority(set<string> &Priority, Config &config)
{
	
}

//�滮û���бؾ�֮·�Ļ�������
inline void processDisPriority(set<string> &DisPriority, Config &config)
{
	//do nothing
}

inline void output(Config &config)
{
	//����滮ʧ�ܵĻ�������
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

	//�����һ�У��滮ʧ�ܵĻ����������滮ʧ�ܵĻ���������
	cout << int(failed_goods.size()) << "," << fixed << setprecision(6) << sum << endl;

	//�����������滮·��
	for (auto it = success_goods.begin(); it != success_goods.end(); it++)	//�滮�ɹ���·��
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
	for (auto it = failed_goods.begin(); it != failed_goods.end(); it++)	//�滮ʧ�ܵ�·��
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

	//��ȡ��һ��
	getline(cin, lineStr);		//ÿ�ζ���һ���У�֪���������з���������
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
			//��վ��Ϣ
			if (count <= config.Global.station_num)
			{
				++sub_count;
				if (sub_count == 1) num_1 = str;
				if (sub_count == 2) config.Station_people[num_1] = atoi(str.c_str());
			}
			else
			{
				//�����Ϣ
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

						//������ӵĳ�վ
						config.Track_to_stations[num_1].insert(tmp_set.begin(), tmp_set.end());
						
						//��վ���ӵĹ��
						config.Station_to_tracks[tmp_set] = num_1;

						//��վ���ڽӳ�վ
						config.Station_adj_stations[num_2].insert(str);
						config.Station_adj_stations[str].insert(num_2);
					}
				}
				else
				{
					//������Ϣ
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