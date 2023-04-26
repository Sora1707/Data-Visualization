#include "NumberUtil.h"
#include <cmath>
#include <iostream>
#include <cstdlib> // srand & rand
#include <time.h> // time 

using namespace std;

int NumberUtil::getRandomInt(int left, int right)
{

    if (left > right) swap(left, right);

    int interval = right - left + 1;
    int result = rand() % interval + left;
    
    return result;
}
