// Queue.h
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
	template <typename TYPE, int BLOCK_SIZE = 8>
	class Queue
	{
		private:

			struct QueueBlock
			{
				QueueBlock* next;

				ALIGNED TYPE data[BLOCK_SIZE];
			};

			QueueBlock* first_block;

			QueueBlock* last_block;

			int first_index;

			int last_index;


		public:

			int size;


			Queue() throw()
			:	first_block((QueueBlock*)0),
				last_block((QueueBlock*)0),
				first_index(0),
				last_index(BLOCK_SIZE - 1),
				size(0)
			{
			}


			Queue(const Queue<TYPE, BLOCK_SIZE>& queue) throw(MemoryException)
			:	first_block((QueueBlock*)0),
				last_block((QueueBlock*)0),
				first_index(0),
				last_index(BLOCK_SIZE - 1),
				size(0)
			{
				const QueueBlock* __restrict other_block = queue.first_block;
				while (other_block)
				{
					QueueBlock* __restrict new_block = new QueueBlock;
					if (!new_block)
					{
						Throw(Memory::exception);
					}
					int block_size;
					if (other_block->next)
					{
						block_size = BLOCK_SIZE;
					}
					else
					{
						last_index = queue.last_index;
						block_size = last_index + 1;
						new_block->next = (QueueBlock*)0;
					}
					register const TYPE* __restrict source = other_block->data;
					register TYPE* __restrict destiny = new_block->data;
					if (last_block)
					{
						last_block->next = new_block;
					}
					else
					{
						first_block = new_block;
						first_index = queue.first_index;
						block_size -= first_index;
						source += first_index;
						destiny += first_index;
					}
					for (register int i = block_size; i; --i)
					{
						*(destiny++) = *(source++);
					}
					last_block = new_block;
					other_block = other_block->next;
					size += block_size;
				}
			}


			~Queue() throw()
			{
				while (first_block)
				{
					register QueueBlock* __restrict next_block = first_block->next;
					delete first_block;
					first_block = next_block;
				}
			}


			Queue<TYPE, BLOCK_SIZE>& operator = (const Queue<TYPE, BLOCK_SIZE>& queue) throw(MemoryException)
			{
				if (this != &queue)
				{
					Clear();

					const QueueBlock* __restrict other_block = queue.first_block;
					while (other_block)
					{
						QueueBlock* __restrict new_block = new QueueBlock;
						if (!new_block)
						{
							Throw(Memory::exception);
						}
						int block_size;
						if (other_block->next)
						{
							block_size = BLOCK_SIZE;
						}
						else
						{
							last_index = queue.last_index;
							block_size = last_index + 1;
							new_block->next = (QueueBlock*)0;
						}
						register const TYPE* __restrict source = other_block->data;
						register TYPE* __restrict destiny = new_block->data;
						if (last_block)
						{
							last_block->next = new_block;
						}
						else
						{
							first_block = new_block;
							first_index = queue.first_index;
							block_size -= first_index;
							source += first_index;
							destiny += first_index;
						}
						for (register int i = block_size; i; --i)
						{
							*(destiny++) = *(source++);
						}
						last_block = new_block;
						other_block = other_block->next;
						size += block_size;
					}
				}
				return *this;
			}


			void Clear() throw()
			{
				while (first_block)
				{
					register QueueBlock* __restrict next_block = first_block->next;
					delete first_block;
					first_block = next_block;
				}
				last_block = (QueueBlock*)0;
				first_index = 0;
				last_index = BLOCK_SIZE - 1;
				size = 0;
			}


			void Enqueue(const TYPE& value) throw(MemoryException)
			{
				if (last_index < BLOCK_SIZE - 1)
				{
					++last_index;
				}
				else
				{
					register QueueBlock* __restrict new_block = new QueueBlock;
					if (!new_block)
					{
						Throw(Memory::exception);
					}
					new_block->next = (QueueBlock*)0;
					if (!first_block)
					{
						first_block = new_block;
						first_index = 0;
					}
					else
					{
						last_block->next = new_block;
					}
					last_block = new_block;
					last_index = 0;
				}
				last_block->data[last_index] = value;
				++size;
			}


			void Dequeue(TYPE& value) throw()
			{
				Assert(first_block);
				Assert(size > 0);

				value = first_block->data[first_index];
				if (first_index < ((first_block == last_block) ? last_index : BLOCK_SIZE - 1))
				{
					++first_index;
				}
				else
				{
					register QueueBlock* __restrict next_block = first_block->next;
					delete first_block;
					first_block = next_block;
					first_index = 0;
					if (!first_block)
					{
						last_block = (QueueBlock*)0;
						last_index = BLOCK_SIZE - 1;
					}
				}
				--size;
			}


			TYPE Dequeue() throw()
			{
				Assert(first_block);
				Assert(size > 0);

				register TYPE value = first_block->data[first_index];
				if (first_index < ((first_block == last_block) ? last_index : BLOCK_SIZE - 1))
				{
					++first_index;
				}
				else
				{
					register QueueBlock* __restrict next_block = first_block->next;
					delete first_block;
					first_block = next_block;
					first_index = 0;
					if (!first_block)
					{
						last_block = (QueueBlock*)0;
						last_index = BLOCK_SIZE - 1;
					}
				}
				--size;
				return value;
			}
	};
}
