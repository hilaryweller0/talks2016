Info<< "Reading field h\n" << endl;
volScalarField h
(
    IOobject
    (
        "h",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

// h interpolated on to faces
surfaceScalarField hf
(
    IOobject("hf", runTime.timeName(), mesh),
    fvc::interpolate(h)
);

// h0 is defined to be the orography
Info<< "Reading field h0 if present (the orography)\n" << endl;
volScalarField h0
(
    IOobject
    (
        "h0",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT
    ),
    mesh,
    dimensionedScalar("h0", dimLength, 0.0)
);

// The gradient of the orography is assumed not to change
const surfaceScalarField gsnGradh0 = mesh.magSf()*g*fvc::snGrad(h0);


// The cell centre velocity
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "calculating field Uf - the velocity on the face\n" << endl;
surfaceVectorField Uf
(
    IOobject
    (
        "Uf",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fvc::interpolate(U)
);


Info<< "Reading/calculating face flux field\n" << endl;

surfaceScalarField flux
(
    IOobject
    (
        "flux",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    hf*(Uf & mesh.Sf())
);

Info<< "Creating Coriolis Force on the face, f (Sf x k)" << endl;
const surfaceVectorField fSfxk
(
    "fSfxk",
    beta*mesh.Cf().component(vector::Y)*(mesh.Sf() ^ vector(0.,0.,1.))
);

// Rate of changes
surfaceScalarField dFluxdt = -(fvc::interpolate(fvc::div(flux,U)) & mesh.Sf())
                           - hf*(fSfxk & Uf)
                           - hf*g*fvc::snGrad(h+h0)*mesh.magSf();
dFluxdt.oldTime();
volScalarField dhdt = -fvc::div(flux);
dhdt.oldTime();

// Courant numbers

surfaceScalarField CoNum = mesh.surfaceInterpolation::deltaCoeffs()*mag(flux)
                         / (fvc::interpolate(h)*mesh.magSf())*runTime.deltaT();

surfaceScalarField waveCoNum = scalar(0.5)
                               *mesh.surfaceInterpolation::deltaCoeffs()
                               *sqrt(mag(fvc::interpolate(h)))
                               *sqrt(g)*runTime.deltaT();

