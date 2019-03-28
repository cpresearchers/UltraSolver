#pragma once
#include <math.h>
#include "typedef.h"
#include <vector>

namespace cudacp
{
	using namespace std;
	class RSparseBitSet
	{
	public:
		int num_tuples;
		int num_vars;
		int num_level = 0;
		int num_bit = 0;
		int last_limits;
		vector<vector<u64>> words;
		vector<int> limit;
		vector<int> index;
		vector<u64> mask;
		int cur_level_ = 0;
		int pre_level = 0;

		RSparseBitSet(int num_tuples, int num_vars) :
			num_tuples(num_tuples), num_vars(num_vars)
		{
			num_level = num_vars + 1;
			num_bit = ceil(double(num_tuples) / double(Constants::BITSIZE));
			//lastLimits = num_tuples % Constants.BITSIZE;

			words.resize(num_level, vector<u64>(num_bit));

			for (int i = 0; i < num_bit; ++i) {
				words[0][i] = UINT64_MAX;
			}

			last_limits = num_tuples % Constants::BITSIZE;

			//lastLimit 取值为[0, 63]
			//若lastLimit = 0, lastWord不改变
			//otherwise, lastWord <<= 64 - lastLimit
			if (last_limits != 0) {
				words[0][num_bit - 1] <<= 64 - last_limits;
			}

			//初始化limit, index, mask
			limit.resize(num_level, -1);
			limit[0] = num_bit - 1;

			index.resize(num_bit);
			for (int i = 0; i < num_bit; i++) {
				index[i] = i;
			}

			mask.resize(num_bit);
		}
		void NewLevel(int level)
		{
			if (cur_level_ != level) {
				cur_level_ = level;
				limit[cur_level_] = limit[pre_level];

				for (int i = limit[pre_level]; i >= 0; --i) {
					const int offset = index[i];
					words[cur_level_][offset] = words[pre_level][offset];
				}
				pre_level = level;
			}
		}

		void DeleteLevel(int level) {
			////        System.out.println("delete: c: " + currentLevel + ", l:" + level);
			//        limit[currentLevel] = -1;
			//        preLevel = --currentLevel;
			//        while (limit[preLevel] == -1) {
			//            preLevel--;
			//        }
			////        currentLevel = level;

			limit[level] = -1;
			pre_level = --level;
			while (limit[pre_level] == -1) {
				--pre_level;
			}
			cur_level_ = pre_level;

			///tips:还有待坐有优化空间
	////        currentLevel =
	//        //回溯后currentLevel失效
	//        if (currentLevel >= level){
	//            preLevel = --level;
	//            while (limit[preLevel] == -1) {
	//                --preLevel;
	//            }
	//            currentLevel = preLevel;
	//            limit[level] = -1;
	//        }

		}
	};

}
