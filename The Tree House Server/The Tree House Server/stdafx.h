#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <vector>
#include <mutex>
#include <array>
#include <map>
#include <algorithm>
#include "Global.h"
#include "ExpOver.h"

#include <concurrent_priority_queue.h>
#include "timer.h"
using namespace std;



using UserRef = shared_ptr<class cUser>;
using RoomRef = shared_ptr<class cRoom>;

static concurrency::concurrent_priority_queue<cTimerEvent> g_timer_queue;