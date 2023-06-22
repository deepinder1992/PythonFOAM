    // Momentum predictor

    MRF.correctBoundaryVelocity(U);

std::list<fvVectorMatrix> UEqn;
for( int i = 0; i < size; ++i)
{
    MRF.correctBoundaryVelocity(*UArray[i]);
  
    fvVectorMatrix tUEqn
    (
        fvm::div(*phiArray[i], *UArray[i])
      + MRF.DDt(*UArray[i])
      + turbulence->divDevRhoReff(*NUEF[i],*UArray[i])
     ==
        fvOptions(*UArray[i])
    );
    tUEqn.relax();
    fvOptions.constrain(tUEqn);
    UEqn.push_back(tUEqn);
    
    
 }

int j = 0;

for(fvVectorMatrix vv:UEqn)
{
   if (simple.momentumPredictor())
    {
        solve(vv == -fvc::grad(*pArray[j]));

        fvOptions.correct(*UArray[j]);
    }
j++;
}


