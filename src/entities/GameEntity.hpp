#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <set>
#include <vector>

class GameEntity
{
public:
	GameEntity(int uid);
	int getUID() const;
	bool hasAllComponents(std::vector<int>& components) const;
	void registerComponent(int componentTypeUID);
private:
	int m_uid = 0;
	std::set<int> m_componentTypeUIDs;
};

#endif