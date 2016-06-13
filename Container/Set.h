// Set.h
// Copyright (C) 2016 Miguel Vargas-Felix (miguel.vargas@gmail.com)
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#pragma once

#include <Basic/Assert.h>
#include <Basic/Memory.h>
#include <Basic/System.h>


namespace Grok
{
	template <typename TYPE>
	struct SetItem
	{
		SetItem* next;

		TYPE value;
	};


	template <typename TYPE, int BLOCK_SIZE = 8>
	class Set
	{
		public:

			SetItem<TYPE>* first;


		private:

			struct SetBlock
			{
				SetBlock* previous;

				SetItem<TYPE> data[BLOCK_SIZE];
			};

			SetItem<TYPE>* first_free;

			SetBlock* last_block;

			int index;


		public:

			int size;


			Set() throw()
			:	first(static_cast<SetItem<TYPE>*>(0)),
				first_free(static_cast<SetItem<TYPE>*>(0)),
				last_block(static_cast<SetBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
			}


			Set(const Set<TYPE, BLOCK_SIZE>& set) throw(MemoryException)
			:	first(static_cast<SetItem<TYPE>*>(0)),
				last_block(static_cast<SetBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
				for (register SetItem<TYPE>* __restrict item = set.first; item; item = item->next)
				{
					Append(item->value);
				}
			}


			template <int OTHER_BLOCK_SIZE>
			Set(const Set<TYPE, OTHER_BLOCK_SIZE>& set) throw(MemoryException)
			:	first(static_cast<SetItem<TYPE>*>(0)),
				last_block(static_cast<SetBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
				for (register SetItem<TYPE>* __restrict item = set.first; item; item = item->next)
				{
					Append(item->value);
				}
			}


			~Set() throw()
			{
				while (last_block)
				{
					register SetBlock* __restrict previous = last_block->previous;
					delete last_block;
					last_block = previous;
				}
			}


			Set<TYPE, BLOCK_SIZE>& operator = (const Set<TYPE, BLOCK_SIZE>& set) throw(MemoryException)
			{
				if (this != &set)
				{
					Clear();
					for (register SetItem<TYPE>* __restrict item = set.first; item; item = item->next)
					{
						Append(item->value);
					}
				}
				return *this;
			}


			template <int OTHER_BLOCK_SIZE>
			Set<TYPE, BLOCK_SIZE>& operator = (const Set<TYPE, OTHER_BLOCK_SIZE>& set) throw(MemoryException)
			{
				if (this != &set)
				{
					Clear();
					for (register SetItem<TYPE>* __restrict item = set.first; item; item = item->next)
					{
						Append(item->value);
					}
				}
				return *this;
			}


			void Append(const TYPE& value) throw(MemoryException)
			{
				register SetItem<TYPE>* __restrict previous_item = static_cast<SetItem<TYPE>*>(0);
				register SetItem<TYPE>* __restrict search_item = first;
				while (search_item)
				{
					if (value == search_item->value)
					{
						return;
					}
					if (value < search_item->value)
					{
						break;
					}
					previous_item = search_item;
					search_item = search_item->next;
				}

				register SetItem<TYPE>* __restrict new_item;
				if (first_free)
				{
					new_item = first_free;
					first_free = first_free->next;
				}
				else
				{
					if (index < BLOCK_SIZE - 1)
					{
						++index;
						new_item = &last_block->data[index];
					}
					else
					{
						register SetBlock* __restrict new_block = new SetBlock;
						if (!new_block)
						{
							Throw(MemoryException());
						}
						if (last_block)
						{
							new_block->previous = last_block;
						}
						else
						{
							new_block->previous = static_cast<SetBlock*>(0);
						}
						last_block = new_block;
						index = 0;
						new_item = last_block->data;
					}
				}

				if (previous_item)
				{
					previous_item->next = new_item;
				}
				else
				{
					first = new_item;
				}
				new_item->next = search_item;
				new_item->value = value;
				++size;
			}


			void Clear() throw()
			{
				while (last_block)
				{
					register SetBlock* __restrict previous = last_block->previous;
					delete last_block;
					last_block = previous;
				}
				first = static_cast<SetItem<TYPE>*>(0);
				first_free = static_cast<SetItem<TYPE>*>(0);
				index = BLOCK_SIZE - 1;
				size = 0;
			}


			bool Delete(const TYPE& value) throw()
			{
				register SetItem<TYPE>* __restrict previous_item = static_cast<SetItem<TYPE>*>(0);
				register SetItem<TYPE>* __restrict search_item = first;
				while (search_item)
				{
					if (value == search_item->value)
					{
						break;
					}
					if (value < search_item->value)
					{
						return false;
					}
					previous_item = search_item;
					search_item = search_item->next;
				}
				if (!search_item)
				{
					return false;
				}
				if (previous_item)
				{
					previous_item->next = search_item->next;
				}
				else
				{
					first = search_item->next;
				}
				search_item->next = first_free;
				first_free = search_item;
				--size;
				return true;
			}


			SetItem<TYPE>* Search(const TYPE& value) throw()
			{
				register SetItem<TYPE>* __restrict item = first;
				while (item)
				{
					if (value == item->value)
					{
						return item;
					}
					if (value < item->value)
					{
						break;
					}
					item = item->next;
				}
				return static_cast<SetItem<TYPE>*>(0);
			}
	};
}
