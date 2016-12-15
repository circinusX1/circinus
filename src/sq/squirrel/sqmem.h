#ifndef _DYNAMICSIZEPOOL_HPP
#define _DYNAMICSIZEPOOL_HPP

#include <cstddef>
#include <cassert>
#include <cstddef>
#include <memory>
#include <new>
#include <utility>

template <size_t SZ> struct minipool
{
private:
	// Represents an element in the arena. This is a union so this element either
	// stores a T or just is a free item which points to the next free item (if
	// any).
	union minipool_item {
	private:
		using StorageType = char[SZ];

		// Points to the next freely available item.
		minipool_item *next;
		// Storage of the item. Note that this is a union
		// so it is shared with the pointer "next" above.
		StorageType datum alignas(sizeof (size_t) );

	public:
		// Methods for the list of free items.
		minipool_item *get_next_item() const { return next; }
		void set_next_item(minipool_item *n) { next = n; }

		// Methods for the storage of the item.
		void *get_storage() { return (datum); }

		// Given a T* cast it to a minipool_item*
		static minipool_item *storage_to_item(void *t) {
			minipool_item *current_item = reinterpret_cast<minipool_item *>(t);
			return current_item;
		}
	};

	// Arena of items. This is just an array of items and a pointer
	// to another arena. All arenas are singly linked between them.
	struct minipool_arena {
	private:
		// Storage of this arena.
		std::unique_ptr<minipool_item[]> storage;
		// Pointer to the next arena.
		std::unique_ptr<minipool_arena> next;

	public:
		// Creates an arena with arena_size items.
		minipool_arena(size_t arena_size) : storage(new minipool_item[arena_size]) {
			for (size_t i = 1; i < arena_size; i++) {
				storage[i - 1].set_next_item(&storage[i]);
			}
			storage[arena_size - 1].set_next_item(nullptr);
		}

		// Returns a pointer to the array of items. This is used by the arena
		// itself. This is only used to update free_list during initialization
		// or when creating a new arena when the current one is full.
		minipool_item *get_storage() const { return storage.get(); }

		// Sets the next arena. Used when the current arena is full and
		// we have created this one to get more storage.
		void set_next_arena(std::unique_ptr<minipool_arena> &&n) {
			assert(!next);

			next.reset(n.release());
		}
	};

	// Size of the arenas created by the pool.
	size_t arena_size;
	// Current arena. Changes when it becomes full and we want to allocate one
	// more object.
	std::unique_ptr<minipool_arena> arena;
	// List of free elements. The list can be threaded between different arenas
	// depending on the deallocation pattern.
	minipool_item *free_list;

public:
	// Creates a new pool that will use arenas of arena_size.
	minipool(size_t arena_size)
	    : arena_size(arena_size), arena(new minipool_arena(arena_size)),
	      free_list(arena->get_storage()) {}

	// Allocates an object in the current arena.
	template <typename... Args> void *alloc(Args &&... args) {
		if (free_list == nullptr) {
			// If the current arena is full, create a new one.
			std::unique_ptr<minipool_arena> new_arena(new minipool_arena(arena_size));
			// Link the new arena to the current one.
			new_arena->set_next_arena(std::move(arena));
			// Make the new arena the current one.
			arena.reset(new_arena.release());
			// Update the free_list with the storage of the just created arena.
			free_list = arena->get_storage();
		}

		// Get the first free item.
		minipool_item *current_item = free_list;
		// Update the free list to the next free item.
		free_list = current_item->get_next_item();

		// Get the storage for T.
		void *result = current_item->get_storage();
		// Construct the object in the obtained storage.
		return result;
	}

	void free(void *t) {
		// Destroy the object.

		// Convert this pointer to T to its enclosing pointer of an item of the
		// arena.
		minipool_item *current_item = minipool_item::storage_to_item(t);

		// Add the item at the beginning of the free list.
		current_item->set_next_item(free_list);
		free_list = current_item;
	}
};


#endif
