#pragma once
#define ReleasePtr(Ptr) \
if (Ptr != nullptr)\
{\
	delete(Ptr);\
	Ptr = nullptr;\
}

#define CreateTask(T, Task) \
new T([=](){Task;})