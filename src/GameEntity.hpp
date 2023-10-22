#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <set>

class GameEntity
{
public:
	GameEntity(int uid);
	int getUID() const;
	bool hasComponent(int componentTypeUID) const;
	void registerComponent(int componentTypeUID);
private:
	int m_uid = 0;
	std::set<int> m_componentTypeUIDs;
};

#endif