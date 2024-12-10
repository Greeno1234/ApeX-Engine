

namespace apex
{
	struct Resource;

	struct Resources
	{
		template <typename T>
		std::shared_ptr<T> load(const std::string& _path)
		{
			// search for previously loaded resouce
			for (size_t i = 0; i < m_resources.size(); ++i)
			{
				//return if found
				if (m_resources.at(i)->getPath() == _path)
				{
					return std::dynamic_pointer_cast<T>(m_resources.at(i)); //make a check for the dynamic pointer cast before returning
				}
			}

			//create new instance, construct it and add to cache
			std::shared_ptr<T> rtn = std::make_shared<T>();
			rtn->m_path = "../resources/" + _path;
			rtn->load();
			m_resources.push_back(rtn);

			return rtn;
		}

	private:
		std::vector<std::shared_ptr<Resource> > m_resources;
	};

}