#pragma once




template<typename T>
class IRepository
{
public:
	virtual T Get(int id) = 0;
	virtual std::vector<T> GetAll() = 0;
	
	template<typename U>
	std::vector<T> Find(U predicate)
	{
		return Model::ORM_Central::refresh_and_get_schema().get_all<T>(sqlite_orm::where(predicate));
	}

	// virtual std::vector<T> Find(std::function<bool()> predicate) = 0;

	virtual void Add(T entity) = 0;
	virtual void AddRange(std::vector<T> range) = 0;

	virtual void Remove(T entity) = 0;
	virtual void RemoveRange(std::vector<T> range) = 0;

	virtual ~IRepository() {}
};

