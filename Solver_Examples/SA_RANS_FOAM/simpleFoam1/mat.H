int k = 0;
for(fvScalarMatrix nuTildsEqn_temp:turbEqn)
{   label NC = mesh.nCells(); //Number of cells
simpleMatrix<double> A(NC); //Coeff.matrix
  
    fvScalarMatrix nuTildsEqn_dash = nuTildsEqn_temp;
             for(label i=0; i<NC; i++)
             {
               A.source()[i] = 0.0;
               for(label j=0; j<NC; j++)
                 {
                  A[i][j] = 0.0;
                 }
             }
// Assigning diagonal coefficients
          for(label i=0; i<NC; i++)
          {
          A[i][i] = nuTildsEqn_dash.diag()[i];
          }
// Assigning off-diagonal coefficients
         for(label faceI=0;
          faceI<nuTildsEqn_temp.lduAddr().lowerAddr().size(); faceI++)
          {
          label l = nuTildsEqn_dash.lduAddr().lowerAddr()[faceI];
          label u = nuTildsEqn_dash.lduAddr().upperAddr()[faceI];
          A[l][u] = nuTildsEqn_dash.upper()[faceI];
          A[u][l] = nuTildsEqn_dash.lower()[faceI];
          }



// Assigning contribution from BC
forAll(nuTilda.boundaryField(), patchI)
{
const fvPatch &pp =
nuTilda.boundaryField()[patchI].patch();
forAll(pp, faceI)
{
label cellI = pp.faceCells()[faceI];

A[cellI][cellI]
+= nuTildsEqn_dash.internalCoeffs()[patchI][faceI];
A.source()[cellI]
+= nuTildsEqn_dash.boundaryCoeffs()[patchI][faceI];
}
}






for(label i=0; i<NC; i++)
{
for(label j=0; j<NC; j++)
{
input_mat[k][i][j] = A[i][j];
input_b[k][i] = nuTildsEqn_temp.source()[i]+A.source()[i];
}
}
k++;       
 }
