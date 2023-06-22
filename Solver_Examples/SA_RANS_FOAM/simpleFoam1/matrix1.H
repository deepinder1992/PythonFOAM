for( int i = 0; i < size; ++i)
{      volVectorField U_dash = *UArray[i];
        
        forAll(U_dash.internalField(), id) 
        {
        input_vels[i][id][0] = U_dash[id].component(vector::X);
        input_vels[i][id][1] = U_dash[id].component(vector::Y);
        }

}

npy_intp dim_U[] = {size,num_cells,2};
array_2d_U = PyArray_SimpleNewFromData(3, dim_U, NPY_DOUBLE, &input_vels[0]);
PyTuple_SetItem(velocity_save_args, 0, array_2d_U);
pValue1 = (PyObject*)PyObject_CallObject(velocity_save, velocity_save_args);

