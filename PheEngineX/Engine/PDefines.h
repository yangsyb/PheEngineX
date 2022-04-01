#pragma once

#define ReleasePtr(Ptr) \
if (Ptr != nullptr)\
{\
	delete(Ptr);\
	Ptr = nullptr;\
}

#define CreateTask(T, Task) \
new T([=](){Task;})

#define PString std::string
#define PWString std::wstring
#define PStringView std::string_view
#define PVector std::vector
#define PMap std::map
#define PHashMap std::unordered_map
#define PFunc std::function

#define StandardBoxVertices { 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5 }
#define StandardBoxIndices { 0, 1, 2, 1, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 }
#define StandardBoxNormal { 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 }
#define StandardBoxUVs { 0.67529296875, 0.984375, 0.98974609375, 0.984375, 0.67529296875, 0.669921875, 0.98974609375, 0.669921875, 0.329833984375, 0.669921875, 0.01537322998046875, 0.669921875, 0.01537322998046875, 0.984375, 0.329833984375, 0.984375, 0.329833984375, 0.33984375, 0.01537322998046875, 0.33984375, 0.01537322998046875, 0.654296875, 0.329833984375, 0.654296875, 0.65966796875, 0.669921875, 0.34521484375, 0.669921875, 0.34521484375, 0.984375, 0.65966796875, 0.984375, 0.329833984375, 0.01001739501953125, 0.01537322998046875, 0.01001739501953125, 0.01537322998046875, 0.324462890625, 0.329833984375, 0.324462890625, 0.65966796875, 0.33984375, 0.34521484375, 0.33984375, 0.34521484375, 0.654296875, 0.65966796875, 0.654296875 }
