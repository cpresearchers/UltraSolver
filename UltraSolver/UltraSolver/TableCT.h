#pragma once
#include <memory>
#include "RSBitSet.h"
namespace cudacp {
	using namespace std;
	class TableCT
	{
	public:

		unique_ptr<RSBitSet> curr_table_;

	};
}

