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

#include "error.H"
#include "numericalBeach.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace numericalBeaches
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

autoPtr<numericalBeach> numericalBeach::New
(
    const word& subDictName,
    const fvMesh& mesh_
)
{
    word beachTypeName;

    // Enclose the creation of the waveProp to ensure it is
    // deleted before the relaxationShape is created otherwise the dictionary
    // is entered in the database twice
    {
        const dictionary coeffDict_
        (
            (mesh_.thisDb().lookupObject<IOdictionary>("waveProperties"))
             .subDict(subDictName + "Coeffs")
             .subDict("relaxationZone")
        );

        beachTypeName = coeffDict_.lookupOrDefault<word>("beachType","Empty");
    }

#if OFVERSION < 2206
    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find("numericalBeach"+beachTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "numericalBeach::New(const word&, const fvMesh&)"
        )   << "Unknown beach type 'numericalBeach" << beachTypeName << "'"
            << endl << endl
            << "Valid beach types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<numericalBeach>(cstrIter()(subDictName,mesh_));
#else
    auto* cstrIter = dictionaryConstructorTable("numericalBeach"+beachTypeName);

    if (!cstrIter)
    {
        FatalErrorIn
        (
            "numericalBeach::New(const word&, const fvMesh&)"
        )   << "Unknown beach type 'numericalBeach" << beachTypeName << "'"
            << endl << endl
            << "Valid beach types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<numericalBeach>(cstrIter(subDictName,mesh_));
#endif
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace numericalBeaches
} // End namespace Foam

// ************************************************************************* //
