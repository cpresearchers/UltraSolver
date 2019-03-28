#pragma once
#include "Var.h"

namespace cudacp
{
	class Val
	{
	public:
		Var* v;
		int a;
	};

	class PVal
	{
	public:
		PVar* v;
		int a;
	};

}
