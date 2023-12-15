#pragma once
#include <string>

#ifdef _DEBUG
#define USE_DEBUG_NAME std::string DBG_name
#else
#define DEBUGNAME
#endif

#ifdef _DEBUG
#define ASSIGN_DEBUG_NAME(object, name) object.DBG_name = name
#else
#define ASSIGNDEBUGNAME(object, name)
#endif
