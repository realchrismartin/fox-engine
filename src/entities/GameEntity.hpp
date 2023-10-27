#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

class GameEntity
{
public:
	GameEntity(int uid);
	int getUID() const;
	bool hasComponent(int componentTypeId) const;
	bool hasAllComponents(std::vector<int>& components) const;

	/// @brief Don't ever call this directly. Let the scene do it.
	/// @param componentTypeUID 
	void registerComponent(int componentTypeUID);
	const std::set<int>& getComponentTypes() const;
private:
	int m_uid = 0;
	std::set<int> m_componentTypeUIDs;
};

#endif