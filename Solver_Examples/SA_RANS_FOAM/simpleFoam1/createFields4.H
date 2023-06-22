Info<< "Reading field p4\n" << endl;
volScalarField p4
(
    IOobject
    (
        "p4",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading field U4\n" << endl;
volVectorField U4
(
    IOobject
    (
        "U4",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading/calculating face flux field phi4\n" << endl;

surfaceScalarField phi4
(
    IOobject
    (
        "phi4",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    linearInterpolate(U4) & mesh.Sf()
);
Info<< "Reading field p\n" << endl;
volScalarField nut4
(
    IOobject
    (
        "nut4",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
volScalarField nuTilda4
(
    IOobject
    (
        "nuTilda4",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading field U\n" << endl;
volScalarField nuef4
(
    IOobject
    (
        "nuef4",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
label p4RefCell = 0;
scalar p4RefValue = 0.0;
setRefCell(p4, simple.dict(), p4RefCell, p4RefValue);
mesh.setFluxRequired(p4.name());
singlePhaseTransportModel laminarTransport4(U4, phi4);
autoPtr<incompressible::turbulenceModel> turbulence4
(
    incompressible::turbulenceModel::New(U4, phi4, laminarTransport4)
);

#include "createMRF.H"
#include "createFvOptions.H"
