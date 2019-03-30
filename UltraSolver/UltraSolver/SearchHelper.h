#pragma once
#include "typedef.h"
#include <vector>
#include "Propagator.h"
#include "HModel.h"
using namespace std;
namespace cudacp
{
	class SearchHelper
	{
	public:
		// 各时间戳
		u64 global_stamp = 0;
		vector<u64> tab_stamp;
		vector<u64> var_stamp;
		// 搜索时间
		u64 time = 0;
		u64 nodes = 0;
		bool is_consistent = true;
		// 搜索上限
		u64 time_limit = 0;
		u64 node_limit = 0;
		u64 failure_limit = 0;

		int level = 0;

		// 线程启动次数
		u64 p_sum = 0;
		// 约束传播次数
		u64 c_sum = 0;

		vector<vector<Propagator*>> subscription;

		void Initial(HModel& m)
		{
			var_stamp.resize(m.vars.size(), 0);
			tab_stamp.resize(m.tabs.size(), 0);
		}
	};

}
