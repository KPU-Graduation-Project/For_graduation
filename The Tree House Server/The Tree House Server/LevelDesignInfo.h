#include <map>
#include "struct.h"


class cLevelDesignInfo
{
public:



private:
	// < sector, < ID, Transform >>
	std::pair<unsigned int, Transform> m_characters[2];
	std::multimap<unsigned int, std::pair<unsigned int,Transform>> m_object_map;

};