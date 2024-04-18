#ifndef GAMEENTITYCONFIG_HPP
#define GAMEENTITYCONFIG_HPP

class Scene;

struct GameEntityConfig
{
<<<<<<< HEAD


	/// @brief Specify a setup function that will run on the scene when this entity is built.
	/// @param fn 
	/// @return 
<<<<<<< HEAD
	GameEntityConfig whenInit(const std::function<void(int, Scene&)>& fn)
=======
	GameEntityConfig whenInit(std::function<void(int entityUID, Scene&)> fn)
>>>>>>> 3607be4 (shipit)
	{
		m_initFn = fn;
		return *this;
	}

	GameEntityConfig startsActive(bool state)
	{
		m_active = state;
		return *this;
	}
	
	/// @brief Run the setup function on the scene.
	/// @param entity 
	/// @param scene 
	void init(int entityUID, Scene& scene) const
	{
		m_initFn(entityUID, scene);
<<<<<<< HEAD
=======
	}

	bool getDefaultActiveState() const
	{
		return m_active;
>>>>>>> 3607be4 (shipit)
	}

=======
	GameEntityConfig whenInit(const std::function<void(int, Scene&)>& fn);
	GameEntityConfig startsActive(bool state);
	void init(int entityUID, Scene& scene) const;
	bool getDefaultActiveState() const;
>>>>>>> 94c3281 (get ready)
private:
	std::function<void(int,Scene&)> m_initFn = [](int entityUID, auto& scene) {};
<<<<<<< HEAD
=======
	bool m_active = true; //Whether the entity is active by default
>>>>>>> 3607be4 (shipit)
};

#endif
