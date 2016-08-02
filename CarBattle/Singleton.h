#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
	// Gets the instance of this object if it was created, otherwise creates a new instance
	static T& Instance()
	{
		// Makes sure that this singleton exists
		if (!m_Instance)
		{
			m_Instance = CreateInstance();
		}

		// Return this singleton
		return *m_Instance;
	}

protected:
	virtual ~Singleton()
	{
		m_Instance = nullptr;
	}
	inline explicit Singleton()
	{
	}

private:
	// Instance of this singelton
	static T* m_Instance;
	// Creates a new instance of this singleton
	static T* CreateInstance()
	{
		return new T();;
	}
};

template<typename T>
T* Singleton<T>::m_Instance = 0;

#endif