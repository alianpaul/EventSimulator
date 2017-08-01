#ifndef SINGLETON_H
#define SINGLETON_H

namespace eventsim
{

template<typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		static T instance;
		return &instance;
	}

protected:
	Singleton();
	
private:
	Singleton(const Singleton&);
	Singleton& operator==(const Singleton&);
};

}


#endif