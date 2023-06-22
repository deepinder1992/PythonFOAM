/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    simpleFoam

Description
    Steady-state solver for incompressible, turbulent flow, using the SIMPLE
    algorithm.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include "simpleControl.H"
#include "fvOptions.H"
#include "bound.H"
#include "orthogonalSnGrad.H"
#include "simpleMatrix.H"
#include <list>
#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{    // Some time related variables
    PyObject *pValue,*pValue1,*array_2d,*array_2d_2,*array_2d_3,*array_2d_U;
    PyArrayObject *ob1, *ob2;
    Py_Initialize();
    
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\"\")");

    printf("initialize numpy array library");
    import_array1(-1); 
    printf("initialize");
    
    PyObject *pName = PyUnicode_DecodeFSDefault("python_module"); // Python filename
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    PyObject *velocity_inp = PyObject_GetAttrString(pModule, "velocity_inp");
    PyObject *velocity_args = PyTuple_New(2);
    PyObject *velocity_save = PyObject_GetAttrString(pModule, "velocity_save");
    PyObject *velocity_save_args = PyTuple_New(1);


   
    
    Py_DECREF(pModule);  
 
    #include "postProcess.H"

    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    #include "createFields.H"
     #include "createFields1.H"
      #include "createFields2.H"
       #include "createFields3.H"
        #include "createFields4.H"
    #include "initContinuityErrs.H"
   int num_cells = mesh.cells().size();
        static double input_b [5][900]={};
        static double input_vels [5][900][2]={};
        static double input_mat[5][900][900]={};

volVectorField* UArray[] = {&U, &U1, &U2, &U3, &U4};
volScalarField* pArray[] = {&p, &p1, &p2, &p3, &p4};
surfaceScalarField* phiArray[] = {&phi, &phi1, &phi2, &phi3, &phi4};
label* refCell[] = {&pRefCell,&p1RefCell,&p2RefCell,&p3RefCell,&p4RefCell};
scalar* refVal[] = {&pRefValue,&p1RefValue,&p2RefValue,&p3RefValue,&p4RefValue};
int size = sizeof(UArray) / sizeof(volVectorField*);
   
turbulence->validate();
turbulence1->validate();
turbulence2->validate();
turbulence3->validate();
turbulence4->validate();
#include "matrix1.H"
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (simple.loop(runTime))
    {   

        Info<< "Time = " << runTime.timeName() << nl << endl;
        volScalarField* NUTIL[] = {&nuTilda, &nuTilda1, &nuTilda2, &nuTilda3, &nuTilda4};
        volScalarField* NUT[] = {&nut, &nut1, &nut2, &nut3, &nut4};
        volScalarField* NUEF[] = {&nuef, &nuef1, &nuef2, &nuef3, &nuef4};
        // --- Pressure-velocity SIMPLE corrector
        {
            #include "UEqn.H"
            #include "pEqn.H"
        }
        laminarTransport.correct();
        laminarTransport1.correct();
        laminarTransport2.correct();
        laminarTransport3.correct();
        laminarTransport4.correct();

         fvScalarMatrix nuTildsEqn_lef = turbulence->DnuTildaEff1(nuTilda,phi);
        
         fvScalarMatrix nuTildsEqn_lef1 = turbulence1->DnuTildaEff1(nuTilda1,phi1);
       
         fvScalarMatrix nuTildsEqn_lef2 = turbulence2->DnuTildaEff1(nuTilda2,phi2);
       
         fvScalarMatrix nuTildsEqn_lef3 = turbulence3->DnuTildaEff1(nuTilda3,phi3);
       
         fvScalarMatrix nuTildsEqn_lef4 = turbulence4->DnuTildaEff1(nuTilda4,phi4);
         
        //fvScalarMatrix nuTildsEqn_rig = turbulence->DnuTildaEff2();
        std::list<fvScalarMatrix> turbEqn = {nuTildsEqn_lef,nuTildsEqn_lef1,nuTildsEqn_lef2,nuTildsEqn_lef3,nuTildsEqn_lef4};
        
        #include"matrix.H"
        
        
      
        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
