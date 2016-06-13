// GaussLegendreQuadrature.h
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

#include <Basic/Memory.h>
#include <Container/Vector.h>
#include <Math/Quadrature.h>


namespace Grok
{
	extern const QuadraturePoint* gauss_legendre_rule[64];


	template <typename TYPE>
	void GaussLegendreQuadrature(int number_of_points, Vector<QuadratureNode<TYPE, 1> >& quadrature_node) throw(MemoryException)
	{
		Assert((number_of_points >= 1) && (number_of_points <= 64));

		try
		{
			const QuadraturePoint* rule = gauss_legendre_rule[number_of_points - 1];

			quadrature_node.Resize(number_of_points);
			int max;
			int min;
			if (number_of_points & 1)
			{
				max = (number_of_points - 1) >> 1;
				min = 1;
			}
			else
			{
				max = (number_of_points - 2) >> 1;
				min = 0;
			}
			int n = 0;
			for (int i = max; i >= min; --i, ++n)
			{
				QuadratureNode<TYPE, 1>& node = quadrature_node.entry[n];

				node.abscissa[0] = -TYPE(rule[i].abscissa);
				node.weight = TYPE(rule[i].weight);
			}
			for (int i = 0; i <= max; ++i, ++n)
			{
				QuadratureNode<TYPE, 1>& node = quadrature_node.entry[n];

				node.abscissa[0] = TYPE(rule[i].abscissa);
				node.weight = TYPE(rule[i].weight);
			}
		}
		catch (Exception&)
		{
			ReThrow();
		}
	}


	template <typename TYPE>
	void GaussLegendreQuadrature(int number_of_points, Vector<QuadratureNode<TYPE, 2> >& quadrature_node) throw(MemoryException)
	{
		Assert((number_of_points >= 1) && (number_of_points <= 64));

		try
		{
			const QuadraturePoint* rule = gauss_legendre_rule[number_of_points - 1];

			quadrature_node.Resize(number_of_points*number_of_points);
			int max;
			int min;
			if (number_of_points & 1)
			{
				max = (number_of_points - 1) >> 1;
				min = 1;
			}
			else
			{
				max = (number_of_points - 2) >> 1;
				min = 0;
			}
			int n = 0;
			for (int i = max; i >= min; --i)
			{
				for (int j = max; j >= min; --j, ++n)
				{
					QuadratureNode<TYPE, 2>& node = quadrature_node.entry[n];

					node.abscissa[0] = -TYPE(rule[i].abscissa);
					node.abscissa[1] = -TYPE(rule[j].abscissa);
					node.weight = TYPE(rule[i].weight*rule[j].weight);
				}
				for (int j = 0; j <= max; ++j, ++n)
				{
					QuadratureNode<TYPE, 2>& node = quadrature_node.entry[n];

					node.abscissa[0] = -TYPE(rule[i].abscissa);
					node.abscissa[1] = TYPE(rule[j].abscissa);
					node.weight = TYPE(rule[i].weight*rule[j].weight);
				}
			}
			for (int i = 0; i <= max; ++i)
			{
				for (int j = max; j >= min; --j, ++n)
				{
					QuadratureNode<TYPE, 2>& node = quadrature_node.entry[n];

					node.abscissa[0] = TYPE(rule[i].abscissa);
					node.abscissa[1] = -TYPE(rule[j].abscissa);
					node.weight = TYPE(rule[i].weight*rule[j].weight);
				}
				for (int j = 0; j <= max; ++j, ++n)
				{
					QuadratureNode<TYPE, 2>& node = quadrature_node.entry[n];

					node.abscissa[0] = TYPE(rule[i].abscissa);
					node.abscissa[1] = TYPE(rule[j].abscissa);
					node.weight = TYPE(rule[i].weight*rule[j].weight);
				}
			}
		}
		catch (Exception&)
		{
			ReThrow();
		}
	}


	template <typename TYPE>
	void GaussLegendreQuadrature(int number_of_points, Vector<QuadratureNode<TYPE, 3> >& quadrature_node) throw(MemoryException)
	{
		Assert((number_of_points >= 1) && (number_of_points <= 64));

		try
		{
			const QuadraturePoint* rule = gauss_legendre_rule[number_of_points - 1];

			quadrature_node.Resize(number_of_points*number_of_points*number_of_points);
			int max;
			int min;
			if (number_of_points & 1)
			{
				max = (number_of_points - 1) >> 1;
				min = 1;
			}
			else
			{
				max = (number_of_points - 2) >> 1;
				min = 0;
			}
			int n = 0;
			for (int i = max; i >= min; --i)
			{
				for (int j = max; j >= min; --j)
				{
					for (int k = max; k >= min; --k, ++n)
					{
						QuadratureNode<TYPE, 3>& node = quadrature_node.entry[n];

						node.abscissa[0] = -TYPE(rule[i].abscissa);
						node.abscissa[1] = -TYPE(rule[j].abscissa);
						node.abscissa[2] = -TYPE(rule[k].abscissa);
						node.weight = TYPE(rule[i].weight*rule[j].weight*rule[k].weight);
					}
					for (int k = 0; k <= max; ++k, ++n)
					{
						QuadratureNode<TYPE, 3>& node = quadrature_node.entry[n];

						node.abscissa[0] = -TYPE(rule[i].abscissa);
						node.abscissa[1] = -TYPE(rule[j].abscissa);
						node.abscissa[2] = TYPE(rule[k].abscissa);
						node.weight = TYPE(rule[i].weight*rule[j].weight*rule[k].weight);
					}
				}
				for (int j = 0; j <= max; ++j)
				{
					for (int k = max; k >= min; --k, ++n)
					{
						QuadratureNode<TYPE, 3>& node = quadrature_node.entry[n];

						node.abscissa[0] = -TYPE(rule[i].abscissa);
						node.abscissa[1] = TYPE(rule[j].abscissa);
						node.abscissa[2] = -TYPE(rule[k].abscissa);
						node.weight = TYPE(rule[i].weight*rule[j].weight*rule[k].weight);
					}
					for (int k = 0; k <= max; ++k, ++n)
					{
						QuadratureNode<TYPE, 3>& node = quadrature_node.entry[n];

						node.abscissa[0] = -TYPE(rule[i].abscissa);
						node.abscissa[1] = TYPE(rule[j].abscissa);
						node.abscissa[2] = TYPE(rule[k].abscissa);
						node.weight = TYPE(rule[i].weight*rule[j].weight*rule[k].weight);
					}
				}
			}
			for (int i = 0; i <= max; ++i)
			{
				for (int j = max; j >= min; --j)
				{
					for (int k = max; k >= min; --k, ++n)
					{
						QuadratureNode<TYPE, 3>& node = quadrature_node.entry[n];

						node.abscissa[0] = TYPE(rule[i].abscissa);
						node.abscissa[1] = -TYPE(rule[j].abscissa);
						node.abscissa[2] = -TYPE(rule[k].abscissa);
						node.weight = TYPE(rule[i].weight*rule[j].weight*rule[k].weight);
					}
					for (int k = 0; k <= max; ++k, ++n)
					{
						QuadratureNode<TYPE, 3>& node = quadrature_node.entry[n];

						node.abscissa[0] = TYPE(rule[i].abscissa);
						node.abscissa[1] = -TYPE(rule[j].abscissa);
						node.abscissa[2] = TYPE(rule[k].abscissa);
						node.weight = TYPE(rule[i].weight*rule[j].weight*rule[k].weight);
					}
				}
				for (int j = 0; j <= max; ++j)
				{
					for (int k = max; k >= min; --k, ++n)
					{
						QuadratureNode<TYPE, 3>& node = quadrature_node.entry[n];

						node.abscissa[0] = TYPE(rule[i].abscissa);
						node.abscissa[1] = TYPE(rule[j].abscissa);
						node.abscissa[2] = -TYPE(rule[k].abscissa);
						node.weight = TYPE(rule[i].weight*rule[j].weight*rule[k].weight);
					}
					for (int k = 0; k <= max; ++k, ++n)
					{
						QuadratureNode<TYPE, 3>& node = quadrature_node.entry[n];

						node.abscissa[0] = TYPE(rule[i].abscissa);
						node.abscissa[1] = TYPE(rule[j].abscissa);
						node.abscissa[2] = TYPE(rule[k].abscissa);
						node.weight = TYPE(rule[i].weight*rule[j].weight*rule[k].weight);
					}
				}
			}
		}
		catch (Exception&)
		{
			ReThrow();
		}
	}
}
