/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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
    advectionFoam

Description
    Solves a transport equation for a passive scalar with explicit or implicit
    treatment of the advectin term

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    Foam::argList::addBoolOption("explicit");
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createFields.H"

    // Open control dictionary
    IOdictionary controlDict
    (
        IOobject
        (
            args.executable() + "Dict", runTime.system(), runTime,
            IOobject::MUST_READ
        )
    );
    
    // Read in the decay rate of the tracer
    const dimensionedScalar decayRate(controlDict.lookup("decayRate"));

    while (runTime.loop())
    {
        #include "CourantNo.H"
        Info<< "Time = " << runTime.timeName() << nl << endl;

        for (label iter = 0; iter < 2; iter++)
        {
            fvScalarMatrix TEqn
            (
                fvm::ddt(T)
              + fvc::Sp(decayRate, T)
            );
            if (args.options().found("explicit"))
            {
                TEqn += fvc::div(phi, T);
            }
            else
            {
                TEqn += fvm::div(phi, T);
            }
            TEqn.solve();
        }
        Info << "T goes from " << min(T.internalField()).value() << " to "
             << max(T.internalField()).value() << endl;
        
        runTime.write();
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
