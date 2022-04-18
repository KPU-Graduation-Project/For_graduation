#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <vector>
#include <mutex>
#include <array>
#include <map>
#include <algorithm>
#include "ExpOver.h"

using namespace std;

const unsigned int  WORLD_HEIGHT = 400;
const unsigned int  WORLD_WIDTH = 400;
const unsigned char  MAX_NAME_SIZE = 20;
