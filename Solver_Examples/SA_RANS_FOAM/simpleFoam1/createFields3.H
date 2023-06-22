Info<< "Reading field p3\n" << endl;
volScalarField p3
(
    IOobject
    (
        "p3",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading field U3\n" << endl;
volVectorField U3
(
    IOobject
    (
        "U3",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading/calculating face flux field phi3\n" << endl;

surfaceScalarField phi3
(
    IOobject
    (
        "phi3",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    linearInterpolate(U3) & mesh.Sf()
);
Info<< "Reading field p\n" << endl;
volScalarField nut3
(
    IOobject
    (
        "nut3",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
volScalarField nuTilda3
(
    IOobject
    (
        "nuTilda3",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading field U\n" << endl;
volScalarField nuef3
(
    IOobject
    (
        "nuef3",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
label p3RefCell = 0;
scalar p3RefValue = 0.0;
setRefCell(p3, simple.dict(), p3RefCell, p3RefValue);
mesh.setFluxRequired(p3.name());
singlePhaseTransportModel laminarTransport3(U3, phi3);
autoPtr<incompressible::turbulenceModel> turbulence3
(
    incompressible::turbulenceModel::New(U3, phi3, laminarTransport3)
);

