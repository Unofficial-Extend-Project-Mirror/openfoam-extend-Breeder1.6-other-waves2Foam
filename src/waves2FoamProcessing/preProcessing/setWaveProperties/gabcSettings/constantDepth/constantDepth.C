/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "constantDepth.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(constantDepth, 0);
addToRunTimeSelectionTable
(
    gabcSettings,
    constantDepth,
    gabcSettings
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


constantDepth::constantDepth
(
    dictionary& dict,
    scalar depth
)
:
    gabcSettings(dict, depth)
{
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void constantDepth::set( Ostream& os )
{
    // Write the preProcessMethod for future reference
    this->writeGiven(os, "preProcessMethod");

    // Write the range for future reference
    this->writeDerived(os, "shapeFunction", "constantDepthCelerity");
    this->writeGiven(os, "celerityFactor");

    this->writeDepth(os);

    this->writeSteadyCurrent(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
