#include <math.h>
#include <string>
#include "cocos2d.h"

namespace compare {
	inline bool is_equal(float val1, float val2)
	{
		return abs(val1 - val2) < 0.00001;
	}

	inline bool is_equal(double val1, double val2)
	{
		return abs(val1 - val2) < 0.00001;
	}

	inline bool is_equal(int val1, int val2)
	{
		return val1 == val2;
	}

	inline bool is_equal(time_t val1, time_t val2)
	{
		return val1 == val2;
	}

	inline bool is_equal(const std::string& val1, const std::string& val2)
	{
		return val1 == val2;
	}

	inline bool is_equal(const cocos2d::Vec2& val1, const cocos2d::Vec2& val2)
	{
		return is_equal(val1.x, val2.x) && is_equal(val1.y, val2.y);
	}
	
}
