int k = 0;
std::list<volScalarField> rAtU;
std::list<volVectorField> HbyA;
std::list<surfaceScalarField> phiHbyA;
std::list<fvScalarMatrix> pEqn;
std::list<volScalarField> div_phiHbyA;

for(fvVectorMatrix vv:UEqn)
{
    volScalarField rAU= 1.0/vv.A();
    volVectorField tHbyA= constrainHbyA(rAU*vv.H(), *UArray[k],*pArray[k]);
    surfaceScalarField tphiHbyA =  fvc::flux(tHbyA);
    
    
    
    
    MRF.makeRelative(tphiHbyA);
    adjustPhi(tphiHbyA,*UArray[k],*pArray[k]);

    volScalarField trAtU =  rAU;
    
    if (simple.consistent())
    {
        trAtU = 1.0/(1.0/rAU - vv.H1());
        tphiHbyA +=
            fvc::interpolate(trAtU - rAU)*fvc::snGrad(*pArray[k])*mesh.magSf();
        tHbyA -= (rAU - trAtU)*fvc::grad(*pArray[k]);
    }
    // Update the pressure BCs to ensure flux consistency
    constrainPressure(*pArray[k], *UArray[k], tphiHbyA, trAtU, MRF);

    // Non-orthogonal pressure corrector loop

        fvScalarMatrix tpEqn
        (
            fvm::laplacian(trAtU, *pArray[k]) == fvc::div(tphiHbyA)
        );

    rAtU.push_back(trAtU);
    HbyA.push_back(tHbyA);
    phiHbyA.push_back(tphiHbyA);
    volScalarField temp_field = fvc::div(tphiHbyA);
    div_phiHbyA.push_back(temp_field);
    pEqn.push_back(tpEqn);
    
    
k++;    
}

k = 0;
std::list<fvScalarMatrix>::iterator pp = pEqn.begin();
std::list<surfaceScalarField>::iterator bb = phiHbyA.begin();
while(pp != pEqn.end() && bb != phiHbyA.end())
{    
   
    while (simple.correctNonOrthogonal())
    {

        pp->setReference(*refCell[k], *refVal[k]);
        pp->solve();
    }    
k++;
pp++;
bb++;

}
//#include "PythonComm.H"       
k = 0;
pp = pEqn.begin();
bb = phiHbyA.begin();
while(pp != pEqn.end() && bb != phiHbyA.end())
{       pArray[k]->correctBoundaryConditions();
        //if (simple.finalNonOrthogonalIter())
       // {
          *phiArray[k] = *bb - pp->flux();  
        //}
#include "contErrs.H"
    
    // Explicitly relax pressure for momentum corrector
    pArray[k]->relax();
k++;
pp++;
bb++;
}


k = 0;
std::list<volVectorField>::iterator hh = HbyA.begin();
std::list<volScalarField>::iterator rr = rAtU.begin();
while(hh != HbyA.end() && rr != rAtU.end())
{

    // Momentum corrector
    *UArray[k] = *hh - *rr*fvc::grad(*pArray[k]);
    UArray[k]->correctBoundaryConditions();
    fvOptions.correct(*UArray[k]);
    k++;
    hh++;  
    rr++;
}

