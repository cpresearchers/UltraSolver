#pragma once

#define _EXPORTING
#ifdef _EXPORTING
#define API_DECLSPEC __declspec(dllexport)
#else
#define API_DECLSPEC __declspec(dllimport)
#endif

#include <iostream>

static void  print() {
	std::cout << "12" << std::endl;
}