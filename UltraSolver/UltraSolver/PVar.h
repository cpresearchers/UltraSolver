#pragma once
#include "Var.h"
namespace cudacp
{
	class PVar :virtual Var
	{
	public:
		u64 SimpleMask();
		bool SubmitMask();
		u64 SubmitMaskAndGet();
		u64 GetAndSubmitMask();
	};
}

