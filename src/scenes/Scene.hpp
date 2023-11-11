#ifndef SCENE_HPP
#define SCENE_HPP

#include "src/entities/GameEntity.hpp"
#include "src/components/ComponentPool.hpp"

class System;
struct ModelData;

/// @brief An association of Entities with their Components.
/// @brief Represents the game world and holds all entities and their components that are in the world.
class Scene
{
public:

	/// @brief Get the component of the specified type T that is associated with the entity with the UID entityUID
	/// @tparam T The type of component we are asking for
	/// @param entityUID The entity we want to get a component for
	/// @return A reference to the component of the type specified.
	template<typename T>
	T& getComponent(const int entityUID)
	{
		if (!hasComponent<T>(entityUID))
		{
			throw std::out_of_range("There is no registered component of this type for this entity. The entity might not even exist..");
		}

		int componentTypeId = GetComponentTypeId<T>();

		//Get the component data that is associated with this entity UID
		void* componentData = m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->getComponent(entityUID);

		//If the pool can't find a component for this entity (unlikely, because we checked that it was registered first), it may return nullptr for the data.
		//If this happens, choke on it.
		if (componentData == nullptr)
		{
			throw std::runtime_error("Getting the component for this entity from the pool returned nullptr");
		}

		//If we have a component in the specified pool for this entity, return a pointer to it.
		T* pointer = static_cast<T*>(componentData);

		return *pointer;
	}

	/// @brief Get the component of type T associated with this GameEntity.
	/// @tparam T The component type we want to get
	/// @param entity The entity we are getting a component for
	/// @return 
	template<typename T>
	T& getComponent(const GameEntity& entity)
	{
		return getComponent<T>(entity.getUID());
	}

	template<typename T>
	bool hasComponent(int entityUID)
	{
		//Handle case where we don't have an entity yet
		if (!m_gameEntityMap.count(entityUID))
		{
			return false;
		}
		
		//Grab the id for the type of component we are getting.
		int componentTypeId = GetComponentTypeId<T>();

		//Handle case that there is no component pool for this component type, which is definitely an error. Just choke on it.
		if (!m_componentTypeToPoolMap.count(componentTypeId))
		{
			return false;
		}

		//Handle case where we don't have a registered component of this type for this entity
		if (!m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->hasRegisteredEntity(entityUID))
		{
			return false;
		}

		return true;

	}

	template<typename T>
	ComponentPool& getComponentPool()
	{
		//Grab the id for the type of component we are getting.
		int componentTypeId = GetComponentTypeId<T>();

		//Handle case that there is no component pool for this component type, which is definitely an error. Just choke on it.
		if (!m_componentTypeToPoolMap.count(componentTypeId))
		{
			throw std::out_of_range("There is no pool for this type.");
		}

		return *m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)].get();
	}

	/// @brief Return true if the entity at this index in the entity list has registered components with the specified IDs. 
	/// @param entityIndex  The index of the entity. This is not the entity UID.
	/// @param componentTypeIds 
	/// @return 
	bool entityHasComponents(int entityIndex, std::vector<int>& componentTypeIds) const;

	GameEntity& getEntity(int entityIndex);
	int getEntityCount() const;

	template<typename ComponentType>
	void applyToSceneGraph(std::function<void(int,Scene&,ComponentType&)>& rootNodeFunctor,
		                   std::function<void(int,Scene&,ComponentType&,ComponentType&)>& childNodeFunctor)
	{
		for (auto rootNode : m_rootNodes)
		{
			applyFunctorToSceneGraph(std::nullopt,rootNode, rootNodeFunctor, childNodeFunctor);
		}
	}
	
	std::optional<int> getCameraEntity() const;
	std::optional<int> getCameraTargetEntity() const;

	void loadModel(const ModelData& modelData, int entityUID);

protected:
	void addChild(int parentEntityUID, int childEntityUID);
	std::optional<int> createEntity();
	void setCameraEntity(int uid);
	void setCameraTargetEntity(int uid);
	void removeEntity(int uid);

	//Add a component to the entity specified by the ID
	//This involves assigning an existing component from our component pools, or allocating a new one.
	template<typename T>
	void addComponent(int entityUID)
	{
		//Handle case where we don't have an entity with this ID yet
		if (!m_gameEntityMap.count(entityUID))
		{
			Logger::log("There is no entity with this UID. Skipping adding a component to it.");
			return; //Return having done nothing
		}
		
		//Grab the id for the type of component we are adding
		int componentTypeId = GetComponentTypeId<T>();

		if (!m_componentTypeToPoolMap.count(componentTypeId))
		{
			//The pool for this component type isn't set up yet. Gotta set it up!
			m_componentPools.push_back(std::move(std::make_unique<ComponentPool>(componentTypeId,sizeof(T), m_maxEntities)));

			//Assign a pool index for this component pool we just added.
			m_componentTypeToPoolMap[componentTypeId] = (int)m_componentPools.size() - 1;
		}

		//Tell the pool that the entity with the given UID is using a given component memory chunk now.
		m_componentPools[m_componentTypeToPoolMap.at(componentTypeId)]->registerEntity<T>(entityUID);
	};

	template<typename ComponentType>
	void applyFunctorToSceneGraph(std::optional<int> parentEntityID, int entityID, std::function<void(int,Scene&,ComponentType&)>& rootNodeFunctor, std::function<void(int,Scene&,ComponentType&,ComponentType&)>& childNodeFunctor)
	{
		int componentId = GetComponentTypeId<ComponentType>();

		if (!m_componentTypeToPoolMap.count(componentId))
		{
			return; //The pool for this component type doesn't exist (yet?)
		}

		if (!m_componentPools[m_componentTypeToPoolMap.at(componentId)]->hasRegisteredEntity(entityID))
		{
			return; //This entity doesn't have a component of this type
		}

		//Run the functor on the node.
		if (parentEntityID.has_value())
		{
			//This is the child node
			//We know the parent has a component of this type, because we recursed to get here
			auto& parentComponent = getComponent<ComponentType>(parentEntityID.value());
			auto& childComponent = getComponent<ComponentType>(entityID);

			childNodeFunctor(entityID, *this, parentComponent, childComponent);
		}
		else
		{
			auto& component = getComponent<ComponentType>(entityID);
			rootNodeFunctor(entityID, *this, component);
		}

		//If the node has children, call this function to apply the functors to those children
		if (m_sceneGraph.count(entityID))
		{
			for (auto child : m_sceneGraph.at(entityID))
			{
				applyFunctorToSceneGraph(entityID,child,rootNodeFunctor,childNodeFunctor);
			}
		}
	}

private:
	size_t m_maxEntities = 10; //The max number of entities we can have, mostly dictated by the size of the component pools for now
	std::map<int, std::set<int>> m_sceneGraph;
	std::set<int> m_rootNodes;
	std::map<int, int> m_gameEntityMap; //Map of entity UIDs to the entity placement in the entity vector
	std::unordered_map<int, int> m_componentTypeToPoolMap; //Map of component types to the pool placement in the pool vector
	std::vector<GameEntity> m_gameEntities; //Worth noting: entity destructors get called a lot because we store them directly in the vector here.
	std::vector<std::unique_ptr<ComponentPool>> m_componentPools;
	int m_availableEntityUID = 0;

	std::optional<int> m_cameraEntityId;
	std::optional<int> m_cameraTargetEntityId;
};

#endif