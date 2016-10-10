#include "Utility.h"

void removeUserFromVector(class TwitterUser*& user, std::vector<class TwitterUser*>& list)
{
	std::vector<class TwitterUser*>::iterator it = std::find(list.begin(), list.end(), user);

	if (it != list.end())
	{
		list.erase(it);
	}
}
