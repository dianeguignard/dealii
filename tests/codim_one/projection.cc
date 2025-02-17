// ---------------------------------------------------------------------
//
// Copyright (C) 2005 - 2018 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------



// continuous projection of a function on the surface of a hypersphere

#include "../tests.h"

// all include files needed for the program

#include <deal.II/base/function.h>
#include <deal.II/base/function_lib.h>
#include <deal.II/base/quadrature_lib.h>

#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/dofs/dof_handler.h>

#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/fe/mapping.h>
#include <deal.II/fe/mapping_q1.h>

#include <deal.II/grid/grid_in.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/grid/tria.h>

#include <deal.II/lac/affine_constraints.h>

#include <deal.II/numerics/data_out.h>
#include <deal.II/numerics/vector_tools.h>

#include <string>


template <int dim, int spacedim>
void
test(std::string filename, unsigned int n)
{
  Triangulation<dim, spacedim> triangulation;
  GridIn<dim, spacedim>        gi;

  gi.attach_triangulation(triangulation);
  std::ifstream in(filename.c_str());
  gi.read_ucd(in);

  FE_Q<dim, spacedim>       fe(n);
  DoFHandler<dim, spacedim> dof_handler(triangulation);

  dof_handler.distribute_dofs(fe);

  // Now we project the constant function on the mesh, and check
  // that this is consistent with what we expect.
  Vector<double> projected_one(dof_handler.n_dofs());
  Vector<double> error(dof_handler.n_dofs());

  Functions::CosineFunction<spacedim> cosine;
  QGauss<dim>                         quad(5);
  AffineConstraints<double>           constraints;
  constraints.close();
  VectorTools::project(dof_handler, constraints, quad, cosine, projected_one);

  DataOut<dim, spacedim> dataout;
  dataout.add_data_vector(dof_handler, projected_one, "projection");
  dataout.build_patches();
  dataout.write_vtk(deallog.get_file_stream());
}



int
main()
{
  initlog();

  for (unsigned int n = 1; n < 5; ++n)
    {
      deallog << "Test<1,2>, continuous finite element q_" << n << std::endl;
      test<1, 2>(SOURCE_DIR "/grids/circle_2.inp", n);

      deallog << "Test<2,3>, continuous finite element q_" << n << std::endl;
      test<2, 3>(SOURCE_DIR "/grids/sphere_2.inp", n);
    }
  return 0;
}
