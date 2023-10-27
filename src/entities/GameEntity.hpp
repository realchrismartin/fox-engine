#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

/// @brief A game object or "thing" in the game. Is associated with Components to create anything that we might need to represent in the game.
/// @brief Systems in Systems.hpp use Entities as a mechanism to associate Components and operate on those Components to change their state.
/// @brief In short, an entity is the "concept" of a game object.
class GameEntity
{
public:
	GameEntity(int uid);
	int getUID() const;
	~GameEntity();
private:
	int m_uid = 0;
};

#endif