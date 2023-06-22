Info<< "Reading field p1\n" << endl;
volScalarField p1
(
    IOobject
    (
        "p1",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading field U1\n" << endl;
volVectorField U1
(
    IOobject
    (
        "U1",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading/calculating face flux field phi1\n" << endl;

surfaceScalarField phi1
(
    IOobject
    (
        "phi1",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    linearInterpolate(U1) & mesh.Sf()
);
Info<< "Reading field p\n" << endl;
volScalarField nut1
(
    IOobject
    (
        "nut1",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
volScalarField nuTilda1
(
    IOobject
    (
        "nuTilda1",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading field U\n" << endl;
volScalarField nuef1
(
    IOobject
    (
        "nuef1",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
label p1RefCell = 0;
scalar p1RefValue = 0.0;
setRefCell(p1, simple.dict(), p1RefCell, p1RefValue);
mesh.setFluxRequired(p1.name());
singlePhaseTransportModel laminarTransport1(U1, phi1);
autoPtr<incompressible::turbulenceModel> turbulence1
(
    incompressible::turbulenceModel::New(U1, phi1, laminarTransport1)
);

