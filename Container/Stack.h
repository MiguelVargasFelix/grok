// Stack.h
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
	class Stack
	{
		private:

			struct StackBlock
			{
				StackBlock* previous;

				TYPE data[BLOCK_SIZE];
			};

			StackBlock* last_block;

			int index;


		public:

			int size;


			Stack() throw()
			:	last_block(static_cast<StackBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
			}


			Stack(const Stack<TYPE, BLOCK_SIZE>& stack) throw(MemoryException)
			:	last_block(static_cast<StackBlock*>(0)),
				index(BLOCK_SIZE - 1),
				size(0)
			{
				const StackBlock* __restrict other_block = stack.last_block;
				StackBlock* __restrict current_block = static_cast<StackBlock*>(0);
				while (other_block)
				{
					StackBlock* __restrict new_block = new StackBlock;
					if (!new_block)
					{
						Throw(MemoryException());
					}
					if (current_block)
					{
						current_block->previous = new_block;
						size += BLOCK_SIZE;
					}
					else
					{
						last_block = new_block;
						index = stack.index;
						size = index + 1;
					}
					current_block = new_block;
					register const TYPE* __restrict source = other_block->data;
					register TYPE* __restrict destiny = current_block->data;
					for (register int i = BLOCK_SIZE; i; --i)
					{
						*(destiny++) = *(source++);
					}
					other_block = other_block->previous;
				}
				if (current_block)
				{
					current_block->previous = static_cast<StackBlock*>(0);
				}
			}


			~Stack() throw()
			{
				while (last_block)
				{
					register StackBlock* __restrict previous = last_block->previous;
					delete last_block;
					last_block = previous;
				}
			}


			Stack<TYPE, BLOCK_SIZE>& operator = (const Stack<TYPE, BLOCK_SIZE>& stack) throw(MemoryException)
			{
				if (this != &stack)
				{
					Clear();

					const StackBlock* __restrict other_block = stack.last_block;
					StackBlock* __restrict current_block = static_cast<StackBlock*>(0);
					while (other_block)
					{
						StackBlock* __restrict new_block = new StackBlock;
						if (!new_block)
						{
							Throw(MemoryException());
						}
						if (current_block)
						{
							current_block->previous = new_block;
							size += BLOCK_SIZE;
						}
						else
						{
							last_block = new_block;
							index = stack.index;
							size = index + 1;
						}
						current_block = new_block;
						register const TYPE* __restrict source = other_block->data;
						register TYPE* __restrict destiny = current_block->data;
						for (register int i = BLOCK_SIZE; i; --i)
						{
							*(destiny++) = *(source++);
						}
						other_block = other_block->previous;
					}
					if (current_block)
					{
						current_block->previous = static_cast<StackBlock*>(0);
					}
				}
				return *this;
			}


			void Clear() throw()
			{
				while (last_block)
				{
					register StackBlock* __restrict previous_block = last_block->previous;
					delete last_block;
					last_block = previous_block;
				}
				index = BLOCK_SIZE - 1;
				size = 0;
			}


			void Push(const TYPE& value) throw(MemoryException)
			{
				if (index < BLOCK_SIZE - 1)
				{
					++index;
				}
				else
				{
					register StackBlock* __restrict new_block = new StackBlock;
					if (!new_block)
					{
						Throw(MemoryException());
					}
					new_block->previous = last_block;
					last_block = new_block;
					index = 0;
				}
				last_block->data[index] = value;
				++size;
			}


			void Pop(TYPE& value) throw()
			{
				Assert(last_block);

				value = last_block->data[index];
				if (index)
				{
					--index;
				}
				else
				{
					register StackBlock* __restrict delete_block = last_block;
					last_block = last_block->previous;
					delete delete_block;
					index = BLOCK_SIZE - 1;
				}
				--size;
			}


			TYPE Pop() throw()
			{
				Assert(last_block);

				register TYPE value = last_block->data[index];
				if (index)
				{
					--index;
				}
				else
				{
					register StackBlock* __restrict delete_block = last_block;
					last_block = last_block->previous;
					delete delete_block;
					index = BLOCK_SIZE - 1;
				}
				--size;
				return value;
			}
	};
}
