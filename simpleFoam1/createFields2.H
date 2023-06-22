Info<< "Reading field p2\n" << endl;
volScalarField p2
(
    IOobject
    (
        "p2",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading field U2\n" << endl;
volVectorField U2
(
    IOobject
    (
        "U2",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading/calculating face flux field phi2\n" << endl;

surfaceScalarField phi2
(
    IOobject
    (
        "phi2",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    linearInterpolate(U2) & mesh.Sf()
);
Info<< "Reading field p\n" << endl;
volScalarField nut2
(
    IOobject
    (
        "nut2",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
volScalarField nuTilda2
(
    IOobject
    (
        "nuTilda2",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
Info<< "Reading field U\n" << endl;
volScalarField nuef2
(
    IOobject
    (
        "nuef2",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
label p2RefCell = 0;
scalar p2RefValue = 0.0;
setRefCell(p2, simple.dict(), p2RefCell, p2RefValue);
mesh.setFluxRequired(p2.name());
singlePhaseTransportModel laminarTransport2(U2, phi2);
autoPtr<incompressible::turbulenceModel> turbulence2
(
    incompressible::turbulenceModel::New(U2, phi2, laminarTransport2)
);

