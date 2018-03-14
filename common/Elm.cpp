#include <iostream>
#include "Elm.hpp"

using namespace std;

Elm::Elm()
{
	endPos = 0;
}
		
void Elm::add( int element )
{
	if( endPos > 256 )
		return;
	
	for( int pos = 0 ; pos < endPos ; pos++ )
		if( element == array[ pos ] )
			return;
		
	array[ endPos++ ] = element;
}

void Elm::showElements()
{
	for( int pos = 0 ; pos < endPos ; pos++ )
	{
		if( pos % 16 == 0 )
			cout << endl;
		cout << hex << array[ pos ] << " ";
	}
	
	cout << endl;	
}
bool Elm::isInner( int index )
{
	for( int pos = 0 ; pos < endPos ; pos++ )
		if( index == ( 0xFF & array[ pos ] ) )
			return true;
		
	return false;
}

bool Elm::getElment( int &index )
{
	if( endPos > 0 )
	{
		index = array[ --endPos ];
		return true;
	}
	else
		return false;
}

bool Elm::getFirst( int &index )
{
	if( endPos == 0 )
		return false;
	
	index = array[ 0 ];
	return true;
}

bool Elm::get( int index , int& dest )
{
	if( endPos < index )
		return false;

	dest = array[ index ];
	return true;
}

bool Elm::remove( int index )
{
	int posFind = endPos;
	for( int pos = 0 ; pos < endPos ; pos++ )
		if( index == ( 0xFF & array[ pos ] ) )
		{
			posFind = pos;
		}

	if( posFind < endPos )
	{
		endPos--;
		for( int pos = posFind ; pos < endPos ; pos++ )
		{
			array[ pos ] = array[ pos + 1 ];
		}	
	}
	else
	{
		return false;
	}
}

int Elm::size()
{
	return endPos;
}

void Elm::clear()
{
	endPos = 0;
}





