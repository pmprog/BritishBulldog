
/****************************************
| Simple C-based List system
|
| Code by Marq Watkin
|   Polymath Programming
|   http://www.pmprog.co.uk
| 
| Entered into the public domain - Feel free to do what you like with this
|
*/

#pragma once


struct ListItem
{
	void* itemData;
	ListItem* nextItem;
	ListItem* previousItem;
};

class List
{
	public:
		ListItem* first;
		ListItem* last;
		int count;

		List();
		void AddToStart( void* newItem );
		void AddToEnd( void* newItem );

		void* ItemAt( int index );

		void RemoveFirst();
		void RemoveLast();
		void RemoveAt( int index );

};

