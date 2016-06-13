// Sequence.h
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
	struct SequenceItem
	{
		SequenceItem* next;

		TYPE value;
	};


	template <typename TYPE, int BLOCK_SIZE = 8>
	class Sequence
	{
		public:

			SequenceItem<TYPE>* first;

			SequenceItem<TYPE>* last;


		private:

			struct SequenceBlock
			{
				SequenceItem<TYPE> data[BLOCK_SIZE];
				SequenceBlock* previous;
			};

			SequenceBlock* last_block;

			int index;


		public:

			int size;


			Sequence() throw()
			:	first(static_cast<SequenceItem<TYPE>*>(0)),
				last(static_cast<SequenceItem<TYPE>*>(0)),
				last_block(static_cast<SequenceBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
			}


			Sequence(const Sequence<TYPE, BLOCK_SIZE>& sequence) throw(MemoryException)
			:	first(static_cast<SequenceItem<TYPE>*>(0)),
				last(static_cast<SequenceItem<TYPE>*>(0)),
				last_block(static_cast<SequenceBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
				for (register SequenceItem<TYPE>* __restrict item = sequence.first; item; item = item->next)
				{
					AppendLast(item->value);
				}
			}


			template <int BLOCK_SIZE2>
			Sequence(const Sequence<TYPE, BLOCK_SIZE2>& sequence) throw(MemoryException)
			:	first(static_cast<SequenceItem<TYPE>*>(0)),
				last(static_cast<SequenceItem<TYPE>*>(0)),
				last_block(static_cast<SequenceBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
				for (register SequenceItem<TYPE>* __restrict item = sequence.first; item; item = item->next)
				{
					AppendLast(item->value);
				}
			}


			~Sequence() throw()
			{
				while (last_block)
				{
					register SequenceBlock* __restrict previous = last_block->previous;
					delete last_block;
					last_block = previous;
				}
			}


			Sequence<TYPE, BLOCK_SIZE>& operator = (const Sequence<TYPE, BLOCK_SIZE>& sequence) throw(MemoryException)
			{
				if (this != &sequence)
				{
					Clear();
					for (register SequenceItem<TYPE>* __restrict item = sequence.first; item; item = item->next)
					{
						AppendLast(item->value);
					}
				}
				return *this;
			}


			template <int BLOCK_SIZE2>
			Sequence<TYPE, BLOCK_SIZE>& operator = (const Sequence<TYPE, BLOCK_SIZE2>& sequence) throw(MemoryException)
			{
				if (this != &sequence)
				{
					Clear();
					for (register SequenceItem<TYPE>* __restrict item = sequence.first; item; item = item->next)
					{
						AppendLast(item->value);
					}
				}
				return *this;
			}


			TYPE& Append() throw(MemoryException)
			{
				#if defined(CC_Intel)
					#pragma warning(push)
					#pragma warning(disable: 873) // entity-kind "entity" has no corresponding operator deletexxxx (to be called if an exception is thrown during initialization of an allocated object)
				#endif

				register SequenceItem<TYPE>* __restrict new_item;
				if (index < BLOCK_SIZE - 1)
				{
					++index;
					new_item = &last_block->data[index];
				}
				else
				{
					register SequenceBlock* __restrict new_block = new(DEFAULT_ALIGNMENT) SequenceBlock;
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
						new_block->previous = static_cast<SequenceBlock*>(0);
					}
					last_block = new_block;
					index = 0;
					new_item = last_block->data;
				}
				
				if (last)
				{
					last->next = new_item;
				}
				else
				{
					first = new_item;
				}
				new_item->next = static_cast<SequenceItem<TYPE>*>(0);
				last = new_item;
				++size;
				return new_item->value;

				#if defined(CC_Intel)
					#pragma warning(pop)
				#endif
			}


			void Clear() throw()
			{
				while (last_block)
				{
					register SequenceBlock* __restrict previous = last_block->previous;
					delete last_block;
					last_block = previous;
				}
				first = static_cast<SequenceItem<TYPE>*>(0);
				last = static_cast<SequenceItem<TYPE>*>(0);
				index = BLOCK_SIZE - 1;
				size = 0;
			}


			SequenceItem<TYPE>* Search(const TYPE& value) throw()
			{
				register SequenceItem<TYPE>* __restrict item = first;
				while (item)
				{
					if (item->value == value)
					{
						return item;
					}
					item = item->next;
				}
				return static_cast<SequenceItem<TYPE>*>(0);
			}
	};
}
