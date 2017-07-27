#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <allocators>
#include <Windows.h>

namespace eventsim
{

class FreeList
{
public:
	FreeList() 
		: header(nullptr)
	{}

	void Push(void* block)
	{
		((Node*)block)->next = header;
		header = (Node*)block;
	}

	void* Pop()
	{
		if (header != nullptr)
		{
			void* block = header;
			header = header->next;
			return block;
		}

		return header;
	}

	bool IsEmpty()
	{
		return header == nullptr;
	}

private:
	struct Node
	{
		Node* next;
	};

	Node* header;
};

enum { WIDTH = 8 };
enum { MAX_BYTES = 128 };
enum { NFREELISTS = MAX_BYTES / WIDTH };
enum { NELES = 5 };

class FreeListCache
{
public:
	void* Allocate(size_t n)
	{
		if (n > MAX_BYTES)
		{
			return ::operator new(n);
		}
		else
		{
			FreeList& freelist = caches[IDX(n)];
			if (freelist.IsEmpty())
			{
				n = ALIGN(n);
				char* begin  = (char*)::operator new(n * NELES);
				char* end    = begin + n * NELES;
				while (begin != end)
				{
					freelist.Push(begin);
					begin += n;
				}	
			}
			return freelist.Pop();
		}
	}

	void  Deallocate(void*p, size_t n)
	{
		if (n > MAX_BYTES)
		{
			::operator delete(p);
		}
		else
		{
			caches[IDX(n)].Push(p);
		}
	}

private:
	int ALIGN(int n)
	{
		return (IDX(n) + 1)* WIDTH;
	}

	int IDX(int n)
	{
		return (n + WIDTH - 1) / WIDTH - 1;
	}

	FreeList caches[NFREELISTS];
};

template<typename Cache>
class SyncNone
{
public:
	static void* Allocate(size_t n)
	{
		return cache.Allocate(n);
	}

	static void  Deallocate(void* p, size_t n)
	{
		cache.Deallocate(p, n);
	}

private:
	static Cache cache;
};

template<typename Cache>
Cache SyncNone<Cache>::cache;

template<typename Cache>
class SyncShared
{
public:
	static void* Allocate(size_t n)
	{
		return cache.Allocate()n;
	}

	static void  Deallocate(void*p, size_t n)
	{
		cache.Deallocate(p, n);
	}

private:
	static Cache cache;
};

template<typename Cache>
Cache SyncShared<Cache>::cache;



};


#endif