#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

class GameEntity
{
public:
	GameEntity(int uid);
	int getUID() const;
private:
	int m_uid = 0;
};

#endif