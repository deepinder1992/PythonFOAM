
#include "mat.H"
npy_intp dim_2[] = {5,num_cells,num_cells};
array_2d_2 = PyArray_SimpleNewFromData(3, dim_2, NPY_DOUBLE, &input_mat[0]);
PyTuple_SetItem(velocity_args, 0, array_2d_2);


npy_intp dim_3[] = {5,num_cells};
array_2d_3 = PyArray_SimpleNewFromData(2, dim_3, NPY_DOUBLE, &input_b[0]);
PyTuple_SetItem(velocity_args, 1, array_2d_3);


pValue = (PyObject*)PyObject_CallObject(velocity_inp, velocity_args);

PyArg_ParseTuple(pValue, "O|O:ref", &ob1, &ob2);
  for( int i = 0; i < 5; ++i)
{
        volScalarField nuT_temp_dash = *NUT[i];
        forAll(nuT_temp_dash.internalField(), id) 
        {
        
        nuT_temp_dash[id] = *((double*)PyArray_GETPTR2(ob1, id,i));
        }
        *NUT[i] = nuT_temp_dash;
    //    NUT[i]->correctBoundaryConditions();
       // NUT[i]->relax();
  }
        
for( int i = 0; i < 5; ++i)
{       volScalarField nuTilda_temp_dash = *NUTIL[i];
        forAll(nuTilda_temp_dash.internalField(), id) 
        {
        
        nuTilda_temp_dash[id] = *((double*)PyArray_GETPTR2(ob2, id,i));
        }
        *NUTIL[i] = nuTilda_temp_dash;
        NUTIL[i]->correctBoundaryConditions();
        bound(*NUTIL[i], dimensionedScalar(NUTIL[i]->dimensions(), 0));
        NUTIL[i]->relax();
       // *NUT[i] =turbulence->nutil(*NUTIL[i]); 
  }

volVectorField uu = *UArray[0];
surfaceScalarField phph = *phiArray[0];
volScalarField ppp = *pArray[0];
NUT[0]->correctBoundaryConditions();
NUT[0]->relax();
 for( int i = 1; i < 5; ++i)
{         
         U = *UArray[i];
         NUT[i]->correctBoundaryConditions();
         NUT[i]->relax();
        
  }
*UArray[0] = uu;
*phiArray[0] =phph;
*pArray[0] = ppp;
for( int i = 0; i < 5; ++i)
{       volScalarField nut_temp_dash = *NUT[i];
        volScalarField nuef_temp_dash = *NUEF[i];
        volScalarField nue = turbulence->nu();
//        forAll(nuef_temp_dash.internalField(), id) 
  //      {
        
        nuef_temp_dash =nut_temp_dash+nue;
 //       }
        *NUEF[i] = nuef_temp_dash;
        NUEF[i]->correctBoundaryConditions();
      //  bound(*NUEF[i], dimensionedScalar(NUEF[i]->dimensions(), 0));
  }
  
 
    

