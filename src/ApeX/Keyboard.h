#include <SDL2/SDL.h>
#include <vector>
namespace apex
{
	struct Core;

	struct Keyboard
	{

		bool isKeyDown(int key);


	private:
		friend struct apex::Core;
		std::vector<int> m_keys;

	};


}