#pragma once
#include <string>
#include <rend/rend.h>
#include <memory>


namespace apex {


	//struct uses stack allocation, // classes use heap allocation
	struct Resource
	{
		virtual void onLoad() = 0; //make shared to create the texture??

		std::string getPath() const;
	private:
		std::string m_path;

		void load();
	};

}