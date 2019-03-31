#pragma once
#include <math.h>
#include "typedef.h"
#include <vector>

namespace cudacp
{
	using namespace std;
	class RSBitSet
	{
	public:

		RSBitSet(const int num_tuples, const int num_vars) :
			num_tuples_(num_tuples), num_vars_(num_vars)
		{
			num_level_ = num_vars + 1;
			num_bit_ = int(ceil(double(num_tuples) / double(Constants::BITSIZE)));
			//lastLimits = num_tuples % Constants.BITSIZE;

			words_.resize(num_level_, vector<u64>(num_bit_));

			for (int i = 0; i < num_bit_; ++i) {
				words_[0][i] = UINT64_MAX;
			}

			last_limits_ = num_tuples % Constants::BITSIZE;

			//lastLimit 取值为[0, 63]
			//若lastLimit = 0, lastWord不改变
			//otherwise, lastWord <<= 64 - lastLimit
			if (last_limits_ != 0) {
				words_[0][(num_bit_ - 1)] <<= (64 - last_limits_);
			}

			//初始化limit, index, mask
			limit_.resize(num_level_, -1);
			limit_[0] = num_bit_ - 1;

			index_.resize(num_bit_);
			for (int i = 0; i < num_bit_; i++) {
				index_[i] = i;
			}

			mask_.resize(num_bit_);
		}

		void Initial(const int num_tuples, const int num_vars) {
			num_tuples_ = num_tuples;
			num_vars_ = num_vars;

			num_level_ = num_vars + 1;
			num_bit_ = int(ceil(double(num_tuples) / double(Constants::BITSIZE)));
			//lastLimits = num_tuples % Constants.BITSIZE;

			words_.resize(num_level_, vector<u64>(num_bit_));

			for (int i = 0; i < num_bit_; ++i) {
				words_[0][i] = UINT64_MAX;
			}

			last_limits_ = num_tuples % Constants::BITSIZE;

			//lastLimit 取值为[0, 63]
			//若lastLimit = 0, lastWord不改变
			//otherwise, lastWord <<= 64 - lastLimit
			if (last_limits_ != 0) {
				words_[0][(num_bit_ - 1)] <<= (64 - last_limits_);
			}

			//初始化limit, index, mask
			limit_.resize(num_level_, -1);
			limit_[0] = num_bit_ - 1;

			index_.resize(num_bit_);
			for (int i = 0; i < num_bit_; i++) {
				index_[i] = i;
			}

			mask_.resize(num_bit_, 0);
		}

		void NewLevel(const int level)
		{
			if (curr_level_ != level) {
				curr_level_ = level;
				limit_[curr_level_] = limit_[prev_level_];

				for (int i = limit_[prev_level_]; i >= 0; --i) {
					const int offset = index_[i];
					words_[curr_level_][offset] = words_[prev_level_][offset];
				}
				prev_level_ = level;
			}
		}

		void DeleteLevel() {
			limit_[curr_level_--] = -1;
		}

		void BackToLevel(int level) {
			limit_[level] = -1;
			prev_level_ = --level;
			while (limit_[prev_level_] == -1) {
				--prev_level_;
			}
			curr_level_ = prev_level_;
		}

		bool IsEmpty() {
			return limit_[curr_level_] == -1;
		}

		void ClearMask() {
			for (int i = 0, cur_limit = limit_[curr_level_]; i <= cur_limit; ++i) {
				const int offset = index_[i];
				mask_[offset] = 0L;
			}
		}

		void ReverseMask() {
			for (int i = 0, cur_limit = limit_[curr_level_]; i <= cur_limit; ++i) {
				const int offset = index_[i];
				mask_[offset] = ~mask_[offset];
			}
		}

		void AddToMask(vector<u64> & m) {
			for (int i = 0, cur_limit = limit_[curr_level_]; i <= cur_limit; ++i) {
				const int offset = index_[i];
				mask_[offset] = mask_[offset] | m[offset];
			}
		}

		bool IntersectWithMask() {
			bool changed = false;
			u64 w, currentWords;
			for (int i = limit_[curr_level_]; i >= 0; --i) {
				const int offset = index_[i];
				currentWords = words_[curr_level_][offset];
				w = currentWords & mask_[offset];
				if (w != currentWords) {
					changed = true;
					words_[curr_level_][offset] = w;
					if (w == 0L) {
						index_[i] = index_[limit_[curr_level_]];
						index_[limit_[curr_level_]] = offset;
						--limit_[curr_level_];
					}
				}
			}
			return changed;
		}

		int IntersectIndex(vector<u64> & m) {
			for (int i = 0, cur_limit = limit_[curr_level_]; i <= cur_limit; ++i) {
				const int offset = index_[i];
				if ((words_[curr_level_][offset] & m[offset]) != 0L) {
					return offset;
				}
			}
			return -1;
		}

		inline bool CheckValid(const int index, const u64 mask) {
			return words_[curr_level_][index] & mask;
		}

	protected:
		int curr_level_ = 0;
		int prev_level_ = 0;
		int num_tuples_ = 0;
		int num_vars_ = 0;
		int num_level_ = 0;
		int num_bit_ = 0;
		int last_limits_;
		vector<vector<u64>> words_;
		vector<int> limit_;
		vector<int> index_;
		vector<u64> mask_;
	};

}
