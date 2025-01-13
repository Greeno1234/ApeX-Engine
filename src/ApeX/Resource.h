#pragma once
#include <string>
#include <rend/rend.h>
#include <memory>


namespace apex {

	struct Resources;
	//struct uses stack allocation, // classes use heap allocation
	struct Resource
	{
		virtual void onLoad() = 0; //make shared to create the texture??

		std::string getPath() const // returns m_path
		{
			return m_path;
		}
	
	private:
		friend struct apex::Resources;
		std::string m_path;

		void load() // calls onLoad for resources
		{
			onLoad();
		}

			
	};

}