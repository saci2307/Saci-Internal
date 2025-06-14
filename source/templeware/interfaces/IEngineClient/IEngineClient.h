#pragma once

// used: callvfunc
#include "..\..\utils\memory\vfunc\vfunc.h"
#include <type_traits>
#include "..\..\utils\math\vector\vector.h" // <- necessário para QAngle_t

class IEngineClient
{
public:
	int maxClients()
	{
		return M::vfunc<int, 34U>(this);
	}

	bool in_game()
	{
		return M::vfunc<bool, 35U>(this);
	}

	bool connected()
	{
		return M::vfunc<bool, 36U>(this);
	}

	int get_local_player() {
		int nIndex = -1;
		M::vfunc<void, 49U>(this, std::ref(nIndex), 0);
		return nIndex + 1;
	}

	void get_view_angles(QAngle_t& viewAngles) {
		M::vfunc<void, 19U>(this, std::ref(viewAngles));
	}

	inline bool valid() {
		return connected() && in_game();
	}
};
