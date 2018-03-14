#ifndef ELM_H
#define ELM_H

class Elm
{
	private:
		int array[ 256 ];
		int endPos;
		
	public:
		Elm();
		void add( int element );
		void showElements();
		bool isInner( int index );
		bool getElment( int &index );
		bool getFirst( int &index );
		bool get( int index , int& dest );
		bool remove( int index );
		int size();
		void clear();
};

#endif


