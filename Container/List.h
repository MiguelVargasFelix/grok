// List.h
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
	struct ListItem
	{
		ListItem* previous;

		ListItem* next;

		TYPE value;
	};


	template <typename TYPE, int BLOCK_SIZE = 8>
	class List
	{
		public:

			ListItem<TYPE>* first;

			ListItem<TYPE>* last;


		private:

			struct ListBlock
			{
				ListItem<TYPE> data[BLOCK_SIZE];

				ListBlock* previous;
			};


			ListItem<TYPE>* first_free;

			ListBlock* last_block;

			int index;


		public:

			int size;


			List() throw()
			:	first(static_cast<ListItem<TYPE>*>(0)),
				last(static_cast<ListItem<TYPE>*>(0)),
				first_free(static_cast<ListItem<TYPE>*>(0)),
				last_block(static_cast<ListBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
			}


			List(const List<TYPE, BLOCK_SIZE>& list) throw(MemoryException)
			:	first(static_cast<ListItem<TYPE>*>(0)),
				last(static_cast<ListItem<TYPE>*>(0)),
				first_free(static_cast<ListItem<TYPE>*>(0)),
				last_block(static_cast<ListBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
				for (register ListItem<TYPE>* __restrict item = list.first; item; item = item->next)
				{
					AppendLast(item->value);
				}
			}


			template <int BLOCK_SIZE2>
			List(const List<TYPE, BLOCK_SIZE2>& list) throw(MemoryException)
			:	first(static_cast<ListItem<TYPE>*>(0)),
				last(static_cast<ListItem<TYPE>*>(0)),
				first_free(static_cast<ListItem<TYPE>*>(0)),
				last_block(static_cast<ListBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
				for (register ListItem<TYPE>* __restrict item = list.first; item; item = item->next)
				{
					AppendLast(item->value);
				}
			}


			~List() throw()
			{
				while (last_block)
				{
					register ListBlock* __restrict previous = last_block->previous;
					delete last_block;
					last_block = previous;
				}
			}


			List<TYPE, BLOCK_SIZE>& operator = (const List<TYPE, BLOCK_SIZE>& list) throw(MemoryException)
			{
				if (this != &list)
				{
					Clear();
					for (register ListItem<TYPE>* __restrict item = list.first; item; item = item->next)
					{
						AppendLast(item->value);
					}
				}
				return *this;
			}


			template <int BLOCK_SIZE2>
			List<TYPE, BLOCK_SIZE>& operator = (const List<TYPE, BLOCK_SIZE2>& list) throw(MemoryException)
			{
				if (this != &list)
				{
					Clear();
					for (register ListItem<TYPE>* __restrict item = list.first; item; item = item->next)
					{
						AppendLast(item->value);
					}
				}
				return *this;
			}


			TYPE& AppendFirst() throw(MemoryException)
			{
				register ListItem<TYPE>* __restrict new_item;
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
						register ListBlock* __restrict new_block = new ListBlock;
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
							new_block->previous = static_cast<ListBlock*>(0);
						}
						last_block = new_block;
						index = 0;
						new_item = last_block->data;
					}
				}
				new_item->previous = static_cast<ListItem<TYPE>*>(0);
				if (first)
				{
					new_item->next = first;
					first->previous = new_item;
				}
				else
				{
					new_item->next = static_cast<ListItem<TYPE>*>(0);
					last = new_item;
				}
				first = new_item;
				++size;
				return new_item->value;
			}


			TYPE& AppendLast() throw(MemoryException)
			{
				register ListItem<TYPE>* __restrict new_item;
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
						register ListBlock* __restrict new_block = new ListBlock;
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
							new_block->previous = static_cast<ListBlock*>(0);
						}
						last_block = new_block;
						index = 0;
						new_item = last_block->data;
					}
				}
				if (last)
				{
					new_item->previous = last;
					last->next = new_item;
				}
				else
				{
					new_item->previous = static_cast<ListItem<TYPE>*>(0);
					first = new_item;
				}
				new_item->next = static_cast<ListItem<TYPE>*>(0);
				last = new_item;
				++size;
				return new_item->value;
			}


			void Clear() throw()
			{
				while (last_block)
				{
					register ListBlock* __restrict previous = last_block->previous;
					delete last_block;
					last_block = previous;
				}
				first = static_cast<ListItem<TYPE>*>(0);
				last = static_cast<ListItem<TYPE>*>(0);
				first_free = static_cast<ListItem<TYPE>*>(0);
				index = BLOCK_SIZE - 1;
				size = 0;
			}


			void Delete(ListItem<TYPE>* __restrict item) throw()
			{
				Assert(item);

				if (item->previous)
				{
					item->previous->next = item->next;
				}
				else
				{
					first = item->next;
				}
				if (item->next)
				{
					item->next->previous = item->previous;
				}
				else
				{
					last = item->previous;
				}
				item->next = first_free;
				first_free = item;
				--size;
			}


			ListItem<TYPE>* Search(const TYPE& value) throw()
			{
				register ListItem<TYPE>* __restrict item = first;
				while (item)
				{
					if (item->value == value)
					{
						return item;
					}
					item = item->next;
				}
				return static_cast<ListItem<TYPE>*>(0);
			}
	};
}
