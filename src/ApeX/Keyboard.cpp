#include "Keyboard.h"

namespace apex
{


	bool Keyboard::isKeyDown(int key)
	{
		for (int i = 0; i < m_keys.size(); ++i)
		{
			if (m_keys[i] == key)
			{
				return true;
			}
		}		

		return false;

	}


}