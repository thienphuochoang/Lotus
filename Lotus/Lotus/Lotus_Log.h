#pragma once
#include <string>
using namespace std;
class Lotus_Log
{
public:
    static void Log(const string& message);
    static void Error(const string& message);
    static void Warning(const string& message);
};

