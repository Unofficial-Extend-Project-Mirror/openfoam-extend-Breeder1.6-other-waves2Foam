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

#include "setWaveProperties.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(setWaveProperties, 0);
defineRunTimeSelectionTable(setWaveProperties, setWaveProperties);

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


void setWaveProperties::lineFormatting( Ostream& os, const word& key)
{
    os << indent << key << token::SPACE;

    for (int i=key.size(); i<Nspaces_-1; i++)
    {
        os << token::SPACE;
    }
}


void setWaveProperties::addITstream
(
    Ostream& os,
    const word& key,
    const ITstream& it
)
{
    lineFormatting(os, key);

    forAll (it, ii)
    {
        os << it[ii];

        if (ii < it.size() - 1)
        {
            os << token::SPACE;
        }
    }

    os << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //


void setWaveProperties::writeBeginning( Ostream& os)
{
    wordList names( dict_.name().components('/') );

    fileName ends( names[names.size() -1]);

#if EXTBRANCH==1
    char delim(':');
#elif OFPLUSBRANCH==1
    char delim('.');
#else
    #if OFVERSION<220
        char delim(':');
    #else
        char delim('.');
    #endif
#endif

    wordList subnames( ends.components( delim ) );
    word dictName( subnames[ subnames.size() -1 ] );

    os << dictName << nl << token::BEGIN_BLOCK << incrIndent << nl;
}


void setWaveProperties::writeGiven( Ostream& os )
{
    wordList toc( dict_.toc() );

    forAll (toc, item)
    {
        if (!dict_.isDict( toc[item]))
        {
            ITstream it( dict_.lookup( toc[item] ) );
            addITstream( os, toc[item], it);
        }
    }
}


void setWaveProperties::writeGiven( Ostream& os, word name )
{
    ITstream it( dict_.lookup(name) );

    addITstream( os, name, it );
}


void setWaveProperties::writeDerived( Ostream& os, word name, scalar val)
{
    lineFormatting(os, name);

    os << val << token::END_STATEMENT << nl;
}


void setWaveProperties::writeDerived( Ostream& os, word name, vector val)
{
    lineFormatting(os, name);

    os << val << token::END_STATEMENT << nl;
}


void setWaveProperties::writeDerived( Ostream& os, word name, scalarField val)
{
    lineFormatting(os, name);

    os << "nonuniform List<scalar>" << nl;

    os << indent << val.size() << nl << indent
       << token::BEGIN_LIST << incrIndent << nl;

    forAll (val, vali)
    {
        os << indent << val[vali] << nl;
    }

    os << decrIndent << indent << token::END_LIST
       << token::END_STATEMENT << nl;
}


void setWaveProperties::writeDerived(Ostream& os, word name, vectorField val)
{
    lineFormatting(os, name);

    os << "nonuniform List<vector>" << nl;

    os << indent << val.size() << nl << indent << token::BEGIN_LIST
       << incrIndent << nl;

    forAll (val, vali)
    {
        os << indent << val[vali] << nl;
    }

    os << decrIndent << indent << token::END_LIST
       << token::END_STATEMENT << nl;
}


void setWaveProperties::writeRelaxationZone( Ostream& os )
{
    // Write the relaxation zone settings
    word rl("relaxationZone");

    if (dict_.found(rl))
    {
        os << nl << indent << rl << nl << indent << token::BEGIN_BLOCK
           << incrIndent << nl;

        dictionary sd( dict_.subDict(rl) );

        wordList toc( sd.toc() );

        forAll (toc, item)
        {
            ITstream it( sd.lookup(toc[item]) );

            addITstream( os, toc[item], it );
        }

        os << decrIndent << indent << token::END_BLOCK << endl;
    }

    // Write the GABC settings
    word gabc("GABCSettings");

    if (dict_.found(gabc))
    {
        this->writeGABCSettings(os);
    }
}


void setWaveProperties::writeGABCSettings(Ostream& os)
{
    // Get the subdictionary
    word gabc("GABCSettings");

    dictionary sd(dict_.subDict(gabc));

    // Write the start of the block
    os << nl << indent << gabc << nl << indent << token::BEGIN_BLOCK
       << incrIndent << nl;

    scalar depth(-1);
    if (dict_.found("depth"))
    {
        depth = readScalar(dict_.lookup("depth"));
    }

    // Call the GABC settings method
    autoPtr<Foam::gabcSettings> settings(Foam::gabcSettings::New(sd, depth));
    settings->set(os);

    // Write the end of the block
    os << decrIndent << indent << token::END_BLOCK << endl;

}


void setWaveProperties::writeEnding( Ostream& os )
{
    os  << decrIndent << token::END_BLOCK << nl << endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


setWaveProperties::setWaveProperties
(
    const Time& rT,
    dictionary& dict,
    vector g,
    bool write
)
:
    write_(write),
    dict_(dict),

/*#if OFPLUSBRANCH==1
    #if OFVERSION<1812
        g_(uniformDimensionedVectorField
            (
                rT.db().lookupObject<uniformDimensionedVectorField>("g")
            ).value())
    #else
        g_(Foam::meshObjects::gravity::New(rT).value())
    #endif
#else
        g_(uniformDimensionedVectorField
            (
                rT.db().lookupObject<uniformDimensionedVectorField>("g")
            ).value())
#endif*/
    g_(g)
{
    G_  = Foam::mag(g_);
    PI_ = M_PI;

    Nspaces_ = 20;
}


setWaveProperties::~setWaveProperties()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


autoPtr<setWaveProperties> setWaveProperties::New
(
    const Time& rT,
    dictionary& dict,
    vector g,
    bool write
)
{
    word waveTheoryTypeName;
    dict.lookup("waveType") >> waveTheoryTypeName;

#if OFVERSION < 2206
    setWavePropertiesConstructorTable::iterator cstrIter =
        setWavePropertiesConstructorTablePtr_->find
        (
            waveTheoryTypeName+"Properties"
        );

    if (cstrIter == setWavePropertiesConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "setWaveProperties::New(const fvMesh&, const Time&, ...)"
        )
        << "Unknown wave property type " << waveTheoryTypeName << "Properties"
        << endl << endl
        << "Valid wave property types are :" << endl
        << setWavePropertiesConstructorTablePtr_->toc()
        << exit(FatalError);
    }

    return autoPtr<setWaveProperties>(cstrIter()(rT, dict, g, write));
#else
    auto* cstrIter = setWavePropertiesConstructorTable
        (
            waveTheoryTypeName+"Properties"
        );

    if (!cstrIter)
    {
        FatalErrorIn
        (
            "setWaveProperties::New(const fvMesh&, const Time&, ...)"
        )
        << "Unknown wave property type " << waveTheoryTypeName << "Properties"
        << endl << endl
        << "Valid wave property types are :" << endl
        << setWavePropertiesConstructorTablePtr_->toc()
        << exit(FatalError);
    }

    return autoPtr<setWaveProperties>(cstrIter(rT, dict, g, write));
#endif
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
