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

#include "fixedDischargeProperties.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(fixedDischargeTrialProperties, 0);
addToRunTimeSelectionTable
(
    setWaveProperties,
    fixedDischargeTrialProperties,
    setWaveProperties
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


fixedDischargeTrialProperties::fixedDischargeTrialProperties
(
    const Time& rT,
    dictionary& dict,
    vector g,
    bool write
)
:
    setWaveProperties(rT, dict, g, write)
{
    Info << "\nConstructing: " << this->type() << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void fixedDischargeTrialProperties::set(Ostream& os)
{
    // Write the beginning of the sub-dictionary
    writeBeginning( os );

    // Write the already given parameters
    writeGiven( os, "waveType" );
    writeGiven( os, "Tsoft");
    writeGiven( os, "Qm3s");
    writeGiven( os, "filterTime");

    writeGiven( os, "bottomLevel");
    writeGiven( os, "domainWidth");

    // This is where type specific data can be written
    // Nothing to be done for fixedDischargeTrial

    // Write the relaxation zone
    writeRelaxationZone(os);

    // Write the closing bracket
    writeEnding(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
