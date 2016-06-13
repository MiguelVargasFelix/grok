// Formula.h
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
#include <Basic/String.h>
#include <Container/Sequence.h>
#include <Container/Vector.h>

#include <math.h>
#include <stdarg.h>

// Examples:
//  exp(-(t - 5)*(t - 5)/0.1); t
//  sqrt(x*x + y*y); x; y
//
// Functions:
//  abs     acos    asin    atan    atan2   ceil
//  cos     cosh    exp     eq      floor   gt
//  log     log10   log2    lt      mod     pow
//  sin     sinh    sqrt    tan     tanh


namespace Grok
{

	namespace FormulaReturn
	{
		enum ID
		{
			ok = 0,
			unmatched_parentheses = 1,
			syntax_error = 2,
			unknown_symbol = 3,
			invalid_variable_name = 4
		};
	}


	template <typename TYPE, int BLOCK_SIZE = 8>
	class Formula
	{
		public:

			Formula() throw()
			:	root_item(static_cast<FormulaItem*>(0)),
				binary_items(),
				unary_items(),
				constants(),
				variables()
			{
			}


			FormulaReturn::ID Define(const char* text) throw(MemoryException)
			{
				int error_position;
				return Define(text, error_position);
			}


			FormulaReturn::ID Define(const char* text, int& error_position) throw(MemoryException)
			{
				Assert(text);

				char* formula = static_cast<char*>(0);
				char* text_copy = Alloca(char, Length(text) + 1);

				try
				{
					Clear();

					const char* pre_formula = text;

					// Count and split variables
					int number_of_variables = 0;
					char* variable_names = static_cast<char*>(0);
					for (char* f = text_copy; ; ++pre_formula)
					{
						if (*pre_formula == ';')
						{
							*f = '\0';
							++f;
							if (number_of_variables == 0)
							{
								variable_names = f;
							}
							++number_of_variables;
						}
						else if (*pre_formula == '\0')
						{
							*f = '\0';
							break;
						}
						else
						{
							*f = *pre_formula;
							++f;
						}
					}

					// Create variables
					variables.Resize(number_of_variables);
					for (int v = 0; v < number_of_variables; ++v, ++variable_names)
					{
						char* name = ParseVariableDefinition(variable_names);
						if (!name)
						{
							return FormulaReturn::invalid_variable_name;
						}
						variables.entry[v].name.Set(name);
					}

					formula = text_copy;
					root_item = Parse(formula);

					error_position = -1;
					return FormulaReturn::ok;
				}
				catch (MemoryException&)
				{
					ReThrow();
				}
				catch (ExceptionBracket&)
				{
					error_position = static_cast<int>(formula - text_copy);
					return FormulaReturn::unmatched_parentheses;
				}
 				catch (ExceptionSyntax&)
				{
					error_position = static_cast<int>(formula - text_copy);
					return FormulaReturn::syntax_error;
				}
 				catch (ExceptionUnknownSymbol&)
				{
					error_position = static_cast<int>(formula - text_copy);
					return FormulaReturn::unknown_symbol;
				}
 			}


			void Clear() throw()
			{
				binary_items.Clear();
				unary_items.Clear();
				constants.Clear();
				variables.Resize(0);
				root_item = static_cast<FormulaItem*>(0);
			}


			TYPE operator () () throw()
			{
				Assert(root_item);
				Assert(variables.size == 0);

				return root_item->Evaluate();
			}


			TYPE operator () (TYPE v1, ...) throw()
			{
				Assert(root_item);
				Assert(variables.size > 0);

				if (variables.size > 0)
				{
					variables.entry[0].value = v1;

					va_list variadic;
					va_start(variadic, v1);
					for (int v = 1; v < variables.size; ++v)
					{
						variables.entry[v].value = va_arg(variadic, TYPE);
					}
					va_end(variadic);
				}
				return root_item->Evaluate();
			}


		protected:

			struct ExceptionBracket {};

			struct ExceptionSyntax {};

			struct ExceptionUnknownSymbol {};

			struct FormulaItem
			{
				virtual ~FormulaItem() throw()
				{
				}


				virtual TYPE Evaluate() const throw() = 0;
			};


			struct Variable : FormulaItem
			{
				String name;

				TYPE value;


				virtual TYPE Evaluate() const throw()
				{
					return value;
				}
			};


			struct Constant : FormulaItem
			{
				TYPE value;


				virtual TYPE Evaluate() const throw()
				{
					return value;
				}
			};


			struct UnaryItem : FormulaItem
			{
				FormulaItem* a;

				TYPE (UnaryItem::* function)(TYPE) const;


				UnaryItem() throw()
				:	FormulaItem(),
					a(static_cast<FormulaItem*>(0)),
					function(static_cast<TYPE (UnaryItem::*)(TYPE) const>(0))
				{
				}


				virtual TYPE Evaluate() const throw()
				{
					return (this->*function)(this->a->Evaluate());
				}


				TYPE Absolute(TYPE a) const
				{
					return fabs(a);
				}


				TYPE ArcCosine(TYPE a) const
				{
					return acos(a);
				}
				

				TYPE ArcSine(TYPE a) const
				{
					return asin(a);
				}


				TYPE ArcTangent(TYPE a) const
				{
					return atan(a);
				}


				TYPE Ceil(TYPE a) const
				{
					return ceil(a);
				}


				TYPE Cosine(TYPE a) const
				{
					return cos(a);
				}


				TYPE CosineHyperbolic(TYPE a) const
				{
					return cosh(a);
				}


				TYPE Exponential(TYPE a) const
				{
					return exp(a);
				}


				TYPE Floor(TYPE a) const
				{
					return floor(a);
				}


				TYPE Logarithm(TYPE a) const
				{
					return log(a);
				}


				TYPE Logarithm2(TYPE a) const
				{
					return log2(a);
				}


				TYPE Logarithm10(TYPE a) const
				{
					return log10(a);
				}


				TYPE Minus(TYPE a) const
				{
					return -a;
				}


				TYPE Sine(TYPE a) const
				{
					return sin(a);
				}


				TYPE SineHyperbolic(TYPE a) const
				{
					return sinh(a);
				}


				TYPE SquareRoot(TYPE a) const
				{
					return sqrt(a);
				}


				TYPE Tangent(TYPE a) const
				{
					return tan(a);
				}


				TYPE TangentHyperbolic(TYPE a) const
				{
					return tanh(a);
				}
			};


			struct BinaryItem : FormulaItem
			{
				FormulaItem* a;

				FormulaItem* b;

				TYPE (BinaryItem::* function)(TYPE, TYPE) const;


				BinaryItem() throw()
				:	FormulaItem(),
					a(static_cast<FormulaItem*>(0)),
					b(static_cast<FormulaItem*>(0)),
					function(static_cast<TYPE (BinaryItem::*)(TYPE, TYPE) const>(0))
				{
				}


				virtual TYPE Evaluate() const throw()
				{
					return (this->*function)(this->a->Evaluate(), this->b->Evaluate());
				}


				TYPE Addition(TYPE a, TYPE b) const
				{
					return a + b;
				}


				TYPE Subtraction(TYPE a, TYPE b) const
				{
					return a - b;
				}


				TYPE Multiplication(TYPE a, TYPE b) const
				{
					return a*b;
				}


				TYPE Division(TYPE a, TYPE b) const
				{
					return a/b;
				}


				TYPE ArcTangent2(TYPE a, TYPE b) const
				{
					return atan2(a, b);
				}


				TYPE Equal(TYPE a, TYPE b) const
				{
					#if defined(CC_Clang)
						#pragma clang diagnostic push
						#pragma clang diagnostic ignored "-Wfloat-equal"
					#endif

					return a == b ? static_cast<TYPE>(1) : static_cast<TYPE>(0);

					#if defined(CC_Clang)
						#pragma clang diagnostic pop
					#endif
				}


				TYPE GreaterThan(TYPE a, TYPE b) const
				{
					return a > b ? static_cast<TYPE>(1) : static_cast<TYPE>(0);
				}


				TYPE LessThan(TYPE a, TYPE b) const
				{
					return a < b ? static_cast<TYPE>(1) : static_cast<TYPE>(0);
				}


				TYPE Modulo(TYPE a, TYPE b) const
				{
					return fmod(a, b);
				}


				TYPE Power(TYPE a, TYPE b) const
				{
					return pow(a, b);
				}
			};


			FormulaItem* Parse(char*& formula) throw(MemoryException, ExceptionBracket, ExceptionSyntax, ExceptionUnknownSymbol)
			{
				FormulaItem* base_item = ParseMonomial(formula);

				for ( ; ; )
				{
					while ((*formula == ' ') || (*formula == '\t'))
					{
						++formula;
					}

					FormulaItem* new_item = static_cast<FormulaItem*>(0);
					if (*formula == '+')
					{
						++formula;
						BinaryItem& item = binary_items.Append();
						item.a = base_item;
						item.b = ParseMonomial(formula);
						item.function = &Formula<TYPE>::BinaryItem::Addition;
						new_item = &item;
					}
					else if (*formula == '-')
					{
						++formula;
						BinaryItem& item = binary_items.Append();
						item.a = base_item;
						item.b = ParseMonomial(formula);
						item.function = &Formula<TYPE>::BinaryItem::Subtraction;
						new_item = &item;
					}
					else if (*formula == '\0')
					{
						break;
					}
					else
					{
						Throw(ExceptionSyntax());
					}
					base_item = new_item;
				}
				return base_item;
			}


			FormulaItem* ParseMonomial(char*& formula) throw(MemoryException, ExceptionBracket, ExceptionSyntax, ExceptionUnknownSymbol)
			{
				FormulaItem* base_item = ParseItem(formula);

				for ( ; ; )
				{
					while ((*formula == ' ') || (*formula == '\t'))
					{
						++formula;
					}

					FormulaItem* new_item = static_cast<FormulaItem*>(0);
					if (*formula == '*')
					{
						++formula;
						BinaryItem& item = binary_items.Append();
						item.a = base_item;
						item.b = ParseItem(formula);
						item.function = &Formula<TYPE>::BinaryItem::Multiplication;
						new_item = &item;
					}
					else if (*formula == '/')
					{
						++formula;
						BinaryItem& item = binary_items.Append();
						item.a = base_item;
						item.b = ParseItem(formula);
						item.function = &Formula<TYPE>::BinaryItem::Division;
						new_item = &item;
					}
					else if ((*formula == '+') || (*formula == '-') || (*formula == '\0'))
					{
						break;
					}
					else
					{
						Throw(ExceptionSyntax());
					}
					base_item = new_item;
				}
				return base_item;
			}


			FormulaItem* ParseItem(char*& formula) throw(MemoryException, ExceptionBracket, ExceptionSyntax, ExceptionUnknownSymbol)
			{
				while ((*formula == ' ') || (*formula == '\t'))
				{
					++formula;
				}

				if ((*formula >= '0') && (*formula <= '9'))
				{
					return ParseNumber(formula);
				}
				else if (((*formula >= 'a') && (*formula <= 'z')) || ((*formula >= 'A') && (*formula <= 'Z')))
				{
					return ParseSymbol(formula);
				}
				else if (*formula == '(')
				{
					return ParseBrackets(formula);
				}
				else if (*formula == '+')
				{
					++formula;
					return ParseItem(formula);
				}
				else if (*formula == '-')
				{
					++formula;
					UnaryItem& new_item = unary_items.Append();
					new_item.a = ParseItem(formula);
					new_item.function = &Formula<TYPE>::UnaryItem::Minus;
					return &new_item;
				}

				Throw(ExceptionSyntax());
			}


			FormulaItem* ParseBrackets(char*& formula) throw(MemoryException, ExceptionBracket, ExceptionSyntax, ExceptionUnknownSymbol)
			{
				++formula;
				char* sub_formula = formula;
				int bracket_level = 1;
				for ( ; *formula; ++formula)
				{
					if (*formula == ')')
					{
						--bracket_level;
						if (bracket_level == 0)
						{
							*formula = '\0';
							++formula;
							return Parse(sub_formula);
						}
					}
					else if (*formula == '(')
					{
						++bracket_level;
					}
				}
				Throw(ExceptionBracket());
			}


			FormulaItem* ParseBracketsWithComma(char*& formula, FormulaItem*& first_item) throw(MemoryException, ExceptionBracket, ExceptionSyntax, ExceptionUnknownSymbol)
			{
				first_item = static_cast<FormulaItem*>(0);

				++formula;
				char* sub_formula = formula;
				int bracket_level = 1;
				for ( ; *formula; ++formula)
				{
					if (*formula == ',')
					{
						if (bracket_level == 1)
						{
							if (first_item)
							{
								Throw(ExceptionSyntax());
							}

							*formula = '\0';
							++formula;
							first_item = Parse(sub_formula);
							sub_formula = formula;
						}
					}
					if (*formula == ')')
					{
						--bracket_level;
						if (bracket_level == 0)
						{
							if (!first_item)
							{
								Throw(ExceptionSyntax());
							}

							*formula = '\0';
							++formula;
							return Parse(sub_formula);
						}
					}
					else if (*formula == '(')
					{
						++bracket_level;
					}
				}
				Throw(ExceptionBracket());
			}


			FormulaItem* ParseSymbol(char*& formula) throw(MemoryException, ExceptionBracket, ExceptionSyntax, ExceptionUnknownSymbol)
			{
				char* symbol = formula;
				int length = 0;
				do
				{
					++length;
					++formula;
				} while (((*formula >= 'a') && (*formula <= 'z')) || ((*formula >= 'A') && (*formula <= 'Z')) || ((*formula >= '0') && (*formula <= '9')) || (*formula == '_'));

				for (int v = 0; v < variables.size; ++v)
				{
					if (IsEqual(symbol, variables.entry[v].name.entry, length))
					{
						return &variables.entry[v];
					}
				}

				if (IsEqual(symbol, "pi", length))
				{
					Constant& new_constant = constants.Append();
					new_constant.value = TYPE(3.1415926535897932384626433832795029L);
					return &new_constant;
				}
				else if (IsEqual(symbol, "e", length))
				{
					Constant& new_constant = constants.Append();
					new_constant.value = TYPE(2.7182818284590452353602874713526625L);
					return &new_constant;
				}

				while ((*formula == ' ') || (*formula =='\t'))
				{
					*formula = '\0';
					++formula;
				}

				if (*formula == '(')
				{
					FormulaItem* first_item = static_cast<FormulaItem*>(0);
					FormulaItem* second_item = static_cast<FormulaItem*>(0);
					*formula = '\0';

					// For the following code see [1]

					typedef TYPE (UnaryItem::* UnaryFunction)(TYPE) const;

					typedef TYPE (BinaryItem::* BinaryFunction)(TYPE, TYPE) const;

					struct FunctionID
					{
						const char* name;

						UnaryFunction unary_function;

						BinaryFunction binary_function;
					};

					static const struct FunctionID function_id[] =
					{
						{"", static_cast<UnaryFunction>(0), static_cast<BinaryFunction>(0)},
						{"", static_cast<UnaryFunction>(0), static_cast<BinaryFunction>(0)},
						{"gt", static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::GreaterThan},
						{"", static_cast<UnaryFunction>(0), static_cast<BinaryFunction>(0)},
						{"lt", static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::LessThan},
						{"abs", &Formula<TYPE>::UnaryItem::Absolute, static_cast<BinaryFunction>(0)},
						{"atan", &Formula<TYPE>::UnaryItem::ArcTangent, static_cast<BinaryFunction>(0)},
						{"atan2", static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::ArcTangent2},
						{"cos", &Formula<TYPE>::UnaryItem::Cosine, static_cast<BinaryFunction>(0)},
						{"cosh", &Formula<TYPE>::UnaryItem::CosineHyperbolic, static_cast<BinaryFunction>(0)},
						{"log", &Formula<TYPE>::UnaryItem::Logarithm, static_cast<BinaryFunction>(0)},
						{"log2", &Formula<TYPE>::UnaryItem::Logarithm2, static_cast<BinaryFunction>(0)},
						{"log10", &Formula<TYPE>::UnaryItem::Logarithm10, static_cast<BinaryFunction>(0)},
						{"pow", static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::Power},
						{"sin", &Formula<TYPE>::UnaryItem::Sine, static_cast<BinaryFunction>(0)},
						{"sinh", &Formula<TYPE>::UnaryItem::SineHyperbolic, static_cast<BinaryFunction>(0)},
						{"mod", static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::Modulo},
						{"ceil", &Formula<TYPE>::UnaryItem::Ceil, static_cast<BinaryFunction>(0)},
						{"asin", &Formula<TYPE>::UnaryItem::ArcSine, static_cast<BinaryFunction>(0)},
						{"tan", &Formula<TYPE>::UnaryItem::Tangent, static_cast<BinaryFunction>(0)},
						{"tanh", &Formula<TYPE>::UnaryItem::TangentHyperbolic, static_cast<BinaryFunction>(0)},
						{"sqrt", &Formula<TYPE>::UnaryItem::SquareRoot, static_cast<BinaryFunction>(0)},
						{"eq", static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::Equal},
						{"exp", &Formula<TYPE>::UnaryItem::Exponential, static_cast<BinaryFunction>(0)},
						{"acos", &Formula<TYPE>::UnaryItem::ArcCosine, static_cast<BinaryFunction>(0)},
						{"", static_cast<UnaryFunction>(0), static_cast<BinaryFunction>(0)},
						{"floor", &Formula<TYPE>::UnaryItem::Floor, static_cast<BinaryFunction>(0)}
					};

					static const unsigned char asso_values[] =
					{
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
						27, 27, 27, 27, 27, 27, 27,  2,  4,  0,
						18,  8, 13,  0, 27, 27,  6, 27,  2,  8,
						27, 27,  5, 27, 12,  5, 12,  0, 27, 27,
						27, 12, 27, 27, 27, 27, 27, 27, 27
					};

					enum
					{
						MIN_WORD_LENGTH = 2,
						MAX_WORD_LENGTH = 5,
						MIN_HASH_VALUE = 2,
						MAX_HASH_VALUE = 26
					};

					if ((length < MIN_WORD_LENGTH) || (length > MAX_WORD_LENGTH))
					{
						Throw(ExceptionUnknownSymbol());
					}
					int key = length + asso_values[static_cast<unsigned char>(symbol[1]) + 1] + asso_values[static_cast<unsigned char>(symbol[0])];
					if ((key < MIN_HASH_VALUE) || (key > MAX_HASH_VALUE))
					{
						Throw(ExceptionUnknownSymbol());
					}
					if (!IsEqual(symbol, function_id[key].name))
					{
						Throw(ExceptionUnknownSymbol());
					}

					if (function_id[key].unary_function)
					{
						Assert(function_id[key].binary_function == static_cast<BinaryFunction>(0));
						UnaryItem& item = unary_items.Append();
						item.a = ParseBrackets(formula);
						item.function = function_id[key].unary_function;
						return &item;
					}
					else
					{
						Assert(function_id[key].binary_function != static_cast<BinaryFunction>(0));
						second_item = ParseBracketsWithComma(formula, first_item);
						BinaryItem& item = binary_items.Append();
						item.a = first_item;
						item.b = second_item;
						item.function = function_id[key].binary_function;
						return &item;
					}
				}
			
				Throw(ExceptionSyntax());
			}


			FormulaItem* ParseNumber(char*& formula) throw(MemoryException, ExceptionSyntax)
			{
				enum
				{
					ns_integer,
					ns_radix_point,
					ns_fractional,
					ns_exp_symbol,
					ns_exp_sign,
					ns_exponent,
					ns_end
				} state = ns_integer;
				static const bool from_to_state[ns_end][ns_end + 1] =
				{
					{true , true , false, true , false, false, true },
					{false, false, true , false, false, false, false},
					{false, false, true , true , false, false, true },
					{false, false, false, false, true , true , false},
					{false, false, false, false, false, true , false},
					{false, false, false, false, false, true , true }
				};

				TYPE integer = 0;
				TYPE fractional = 0;
				TYPE divisor = 1;
				TYPE exp_sign = 1;
				TYPE exponent = 0;
				for ( ; ; ++formula)
				{
					if ((*formula >= '0') && (*formula <= '9'))
					{
						if (from_to_state[state][ns_integer])
						{
							state = ns_integer;
							integer *= 10;
							integer += *formula - '0';
							continue;
						}
						else if (from_to_state[state][ns_fractional])
						{
							state = ns_fractional;
							fractional *= 10;
							fractional += *formula - '0';
							divisor *= 10;
							continue;
						}
						else if (from_to_state[state][ns_exponent])
						{
							state = ns_exponent;
							exponent *= 10;
							exponent += *formula - '0';
							continue;
						}
					}
					else if (*formula == '.')
					{
						if (from_to_state[state][ns_radix_point])
						{
							state = ns_radix_point;
							continue;
						}
					}
					else if ((*formula == 'e') || (*formula == 'E'))
					{
						if (from_to_state[state][ns_exp_symbol])
						{
							state = ns_exp_symbol;
							continue;
						}
					}
					else if (*formula == '+')
					{
						if (from_to_state[state][ns_exp_sign])
						{
							state = ns_exp_sign;
							continue;
						}
						else if (from_to_state[state][ns_end])
						{
							break;
						}

					}
					else if (*formula == '-')
					{
						if (from_to_state[state][ns_exp_sign])
						{
							state = ns_exp_sign;
							exp_sign = -1;
							continue;
						}
						else if (from_to_state[state][ns_end])
						{
							break;
						}
					}
					else
					{
						if (from_to_state[state][ns_end])
						{
							break;
						}
					}
					Throw(ExceptionSyntax());
				}
				TYPE number = (integer*divisor + fractional)*pow(10, exp_sign*exponent)/divisor;
				Constant& new_constant = constants.Append();
				new_constant.value = number;
				return &new_constant;
			}


			char* ParseVariableDefinition(char*& formula) throw()
			{
				enum
				{
					vs_start,
					vs_pre_space,
					vs_var_head,
					vs_var_tail,
					vs_post_space,
					vs_end
				} state = vs_start;
				static const bool from_to_state[vs_end][vs_end + 1] =
				{
					{false, true , true,  false, false, false},
					{false, true , true,  false, false, false},
					{false, false, false, true , true , true },
					{false, false, false, true , true , true },
					{false, false, false, false, true , true }
				};

				char* name = static_cast<char*>(0);
				for ( ; ; ++formula)
				{
					if ((*formula == ' ') || (*formula == '\t'))
					{
						if (from_to_state[state][vs_pre_space])
						{
							state = vs_pre_space;
							continue;
						}
						else if (from_to_state[state][vs_post_space])
						{
							state = vs_post_space;
							*formula = '\0';
							continue;
						}
					}
					else if (((*formula >= '0') && (*formula <= '9')) || (*formula == '_'))
					{
						if (from_to_state[state][vs_var_tail])
						{
							state = vs_var_tail;
							continue;
						}
					}
					else if (((*formula >= 'a') && (*formula <= 'z')) || ((*formula >= 'A') && (*formula <= 'Z')))
					{
						if (from_to_state[state][vs_var_head])
						{
							state = vs_var_head;
							name = formula;
							continue;
						}
						else if (from_to_state[state][vs_var_tail])
						{
							state = vs_var_tail;
							continue;
						}
					}
					else if (*formula == '\0')
					{
						if (from_to_state[state][vs_end])
						{
							break;
						}
					}
					return static_cast<char*>(0);
				}
				return name;
			}


			FormulaItem* root_item;

			Sequence<BinaryItem, BLOCK_SIZE> binary_items;

			Sequence<UnaryItem, BLOCK_SIZE> unary_items;

			Sequence<Constant, BLOCK_SIZE> constants;

			Vector<Variable> variables;
	};
}

// [1] Hash table generated with gperf (http://www.gnu.org/software/gperf/):
//   gperf --struct-type --seven-bit --readonly-tables --word-array-name=function_id --initializer-suffix=", static_cast<UnaryFunction>(0), static_cast<BinaryFunction>(0)" --multiple-iterations=100 --language=C --enum input
// "input" file:
/*
typedef TYPE (UnaryItem::* UnaryFunction)(TYPE) const;
typedef TYPE (BinaryItem::* BinaryFunction)(TYPE, TYPE) const;
struct FunctionID {const char* name; UnaryFunction unary_function; BinaryFunction binary_function;};
%%
abs, &Formula<TYPE>::UnaryItem::Absolute, static_cast<BinaryFunction>(0)
acos, &Formula<TYPE>::UnaryItem::ArcCosine, static_cast<BinaryFunction>(0)
asin, &Formula<TYPE>::UnaryItem::ArcSine, static_cast<BinaryFunction>(0)
atan, &Formula<TYPE>::UnaryItem::ArcTangent, static_cast<BinaryFunction>(0)
atan2, static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::ArcTangent2
ceil, &Formula<TYPE>::UnaryItem::Ceil, static_cast<BinaryFunction>(0)
cos, &Formula<TYPE>::UnaryItem::Cosine, static_cast<BinaryFunction>(0)
cosh, &Formula<TYPE>::UnaryItem::CosineHyperbolic, static_cast<BinaryFunction>(0)
exp, &Formula<TYPE>::UnaryItem::Exponential, static_cast<BinaryFunction>(0)
eq, static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::Equal
floor, &Formula<TYPE>::UnaryItem::Floor, static_cast<BinaryFunction>(0)
gt, static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::GreaterThan
log, &Formula<TYPE>::UnaryItem::Logarithm, static_cast<BinaryFunction>(0)
log10, &Formula<TYPE>::UnaryItem::Logarithm10, static_cast<BinaryFunction>(0)
log2, &Formula<TYPE>::UnaryItem::Logarithm2, static_cast<BinaryFunction>(0)
lt, static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::LessThan
mod, static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::Modulo
pow, static_cast<UnaryFunction>(0), &Formula<TYPE>::BinaryItem::Power
sin, &Formula<TYPE>::UnaryItem::Sine, static_cast<BinaryFunction>(0)
sinh, &Formula<TYPE>::UnaryItem::SineHyperbolic, static_cast<BinaryFunction>(0)
sqrt, &Formula<TYPE>::UnaryItem::SquareRoot, static_cast<BinaryFunction>(0)
tan, &Formula<TYPE>::UnaryItem::Tangent, static_cast<BinaryFunction>(0)
tanh, &Formula<TYPE>::UnaryItem::TangentHyperbolic, static_cast<BinaryFunction>(0)
*/
