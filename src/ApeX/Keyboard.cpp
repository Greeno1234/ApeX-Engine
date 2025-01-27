#include "Keyboard.h"

namespace apex
{
	bool Keyboard::isKeyDown(int key) /// returns boolean true if key stored within keys
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

	int Keyboard::whichKey() ///< returns the integer of the stored key
	{

		for (int i = 0; i < m_keys.size(); i++)
		{
			return m_keys[0];
		}
		
	}
}