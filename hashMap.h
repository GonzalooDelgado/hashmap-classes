#ifndef GONZ_HASHMAP_DYNAMIC
#define GONZ_HASHMAP_DYNAMIC

#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

// Custom Hashmap class using linear probing and an array of linked lists

/** class_identifier: Class includes all functions needed to create a simple hashmap* 
 * constructors: 	
 *                  hashMap(int = 10);
	                hashMap(const hashMap<type1, type2>&);
					~hashMap(); - Deconstructor* 
 * public functions: 
                    const hashMap<type1, type2>& operator=(const hashMap<type1, type2>&);
					type2& operator[](std::string);* 
 * static members: N/A*/
template <class type1, class type2>
class hashMap
{
	public:
		const int MAX_ELEMENTS;
		hashMap(int = 10);
		hashMap(const hashMap<type1, type2>&);
		const hashMap<type1, type2>& operator=(const hashMap<type1, type2>&);
		~hashMap();
		type2& operator[](std::string);

	private:
		int hashFunction(std::string) const;
		void resize(int);

		struct node
		{
			type1 key;
			type2 value;
			node * next;
			node * prev;
		};

		struct list
		{
			node * head;
			node * middleElement;
			int amount;
		};
		
		int filledEntries;
		int tableSize;

		list * table;
};

/** function_identifier: Basic constructor that will create the values
 * * parameters: Size of the table
 * * return value: None*/
template <class type1, class type2>
hashMap<type1, type2>::hashMap(int init) : MAX_ELEMENTS(10)
{
	//Set values
	tableSize=init;
	filledEntries=0;

	//create table
	table=new list[init];

	//set default values to the table
	for(int i=0; i<init; i++){
		table[i].head=nullptr;
		table[i].middleElement=nullptr;
		table[i].amount=0;
	}
}

/** function_identifier: It will copy the passed hashMap into a new one
 * * parameters: hashmap from which to copy the values from
 * * return value: None*/
template <class type1, class type2>
hashMap<type1, type2>::hashMap(const hashMap<type1, type2>& copy) : MAX_ELEMENTS(10)
{
	table=nullptr;
	//Set new variables
	tableSize=copy.tableSize;
	filledEntries=copy.filledEntries;
	table=new list[tableSize];
	//Create new table
	for (int i=0; i < tableSize; i++)
	{
		if(copy.table[i].head==nullptr){
			table[i].head=nullptr;
		}
		else{
			//Copy the head of the linked list of the indice
			node* it, *copyIt;
			table[i].head=new node;
			table[i].head->key=copy.table[i].head->key;
			table[i].head->value=copy.table[i].head->value;
			table[i].head->prev=nullptr;
			table[i].head->next=nullptr;
			table[i].middleElement=copy.table[i].middleElement;
			table[i].amount=copy.table[i].amount;

			//sets it to the head (start) and copyIt to the start of the copy
			//Done to not lose track of nodes when copying following members of indice
			it=table[i].head;
			copyIt=copy.table[i].head;
			//If there is more copy them until it hits nullptr
			while(copyIt->next!=nullptr){
				//Updates copyIt to go to the next one and right one
				copyIt=copyIt->next;
				node* newNode = new node;
				//Sets the link of next of the prev node to this new one
				it->next=newNode;
				newNode->prev=it;
				newNode->key=copyIt->key;
				newNode->value=copyIt->value;
				newNode->next=nullptr;
				//Sets iterator to current node
				it=newNode;
			}
		}
	}
}

/** function_identifier: Overloads the = operator to copy all values into the hashmap
 * * parameters: Rhs of the = operator from which to copy the values from
 * * return value: A hashmap with new values*/
template <class type1, class type2>
const hashMap<type1, type2>& hashMap<type1, type2>::operator=(const hashMap<type1, type2>& rhs)
{
	//check for self assignment
	if (this != &rhs)
	{
		//Deallocate old table
		for (int i = 0; i < this->tableSize; i++)
		{
			node * del1, * del2;
			del1 = this->table[i].head;

			while (del1 != NULL)
			{
				del2 = del1->next;
				delete del1;
				del1 = del2;
			}
		}
		delete [] this->table;
		this->table=nullptr;

		//Set new variables
		this->tableSize=rhs.tableSize;
		this->filledEntries=rhs.filledEntries;
		this->table=new list[tableSize];
		//Create new table
		for (int i=0; i < tableSize; i++)
		{
			if(rhs.table[i].head==nullptr){
				this->table[i].head=nullptr;
			}
			else{
				//Copy the head of the linked list of the indice
				node* it, *copyIt;
				this->table[i].head=new node;
				this->table[i].head->key=rhs.table[i].head->key;
				this->table[i].head->value=rhs.table[i].head->value;
				this->table[i].head->prev=nullptr;
				this->table[i].head->next=nullptr;
				this->table[i].middleElement=rhs.table[i].middleElement;
				this->table[i].amount=rhs.table[i].amount;

				//sets it to the head (start) and copyIt to the start of the copy
				//Done to not lose track of nodes when copying following members of indice
				it=this->table[i].head;
				copyIt=rhs.table[i].head;
				//If there is more copy them until it hits nullptr
				while(copyIt->next!=nullptr){
					//Updates copyIt to go to the next one and right one
					copyIt=copyIt->next;
					node* newNode = new node;
					//Sets the link of next of the prev node to this new one
					it->next=newNode;
					newNode->prev=it;
					newNode->key=copyIt->key;
					newNode->value=copyIt->value;
					newNode->next=nullptr;
					//Sets iterator to current node
					it=newNode;
				}
			}
		}
	}
	return *this;
}

/** function_identifier: It will deallocate all used memory
 * * parameters: None
 * * return value: None*/
template <class type1, class type2>
hashMap<type1, type2>::~hashMap()
{
	if(table!=nullptr){
		//Deallocate old table
		for (int i = 0; i < tableSize; i++)
		{
			node * del1, * del2;
			del1 = table[i].head;

			while (del1 != NULL)
			{
				del2 = del1->next;
				del1->prev=nullptr;
				delete del1;
				del1 = del2;
			}
		}
		delete [] table;
	}
}

/** function_identifier: Overload of Operator[] which will either update the value or create a new one in the right place
 * * parameters: value from which to look for
 * * return value: value reference to update*/
template <class type1, class type2>
type2& hashMap<type1, type2>::operator[](std::string key2)
{	
	//Variables
	bool doTask=true;
	int index=0;
	double loadFactor=0.0;
	double entries=static_cast<double>(filledEntries);
	double tableSize2=static_cast<double>(tableSize);

	//sets loadFactor
	loadFactor=entries/tableSize2;

	//checks if loadFactor has been meet
	if(loadFactor>=(1.0/2.0))
		resize(tableSize);

	//gets the index from the hashfunction
	index=hashFunction(key2);
	
	//while loop created to easily account for linear probing
	do{
		//Used to check linear probing
		bool found=true;
		//if the index given is null then create a new head
		if(table[index].head==nullptr){
			table[index].head=new node;
			table[index].amount++;
			table[index].head->prev=nullptr;
			table[index].head->next=nullptr;
			//set key to passed key
			table[index].head->key=key2;
			//set value to a default value that can be updated
			table[index].head->value=type2();
			table[index].middleElement=table[index].head;

			//return value to update it
			return table[index].head->value;
		}

		//if not updated then set the iterator to the middle element for faster findings
		node *it, *t2;
		it=table[index].middleElement;
		if(it->key==key2){
			return it->value;
		}

		//fix indenting
		//While loop to ways run if the it->key is greater than the passed key
		while((it->key)>key2){
			//set t2 to it for easy node addition
			t2=it;
			it=it->prev;
			//Head insert (first checked to prevent seg faults)
			if(it==nullptr && table[index].amount<MAX_ELEMENTS){
				//inserst a new headNode and correctly changes its prev and nexts and other connections
				node * newNode=new node;
				it=t2;
				table[index].amount++;
				newNode->prev=nullptr;
				newNode->next=t2;
				newNode->key=key2;
				newNode->value=type2();
				it->prev=newNode;
				table[index].head=newNode;

				//if the index is full then add one to filledEntries
				if(table[index].amount==MAX_ELEMENTS){
					filledEntries++;
				}

				//Set new middleElement, needs to be like this due to the way nodes get added
				node* newMid;
				newMid=table[index].head;
				for(int i=0; i<table[index].amount/2; i++){
					if(newMid->next==nullptr){
						break;
					}
					newMid=newMid->next;
				}
				table[index].middleElement=newMid;

				//return value
				return newNode->value;
			}

			//linear probing head insert
			// boolean to go back on top
			if(it==nullptr && table[index].amount==MAX_ELEMENTS){
				index=(index+1)%tableSize;
				found=false;
				break;
			}
			//If the current iterator has the key
			// then return without making any changes
			if(it->key==key2){
				return it->value;
			}
			//insert between elements
			if((it->key)<key2 && table[index].amount<MAX_ELEMENTS){
				//Inserts nodes inbetween other nodes
				node * newNode=new node;
				newNode->key=key2;
				newNode->prev=t2->prev;
				newNode->next=t2;
				newNode->value=type2();
				t2->prev->next=newNode;
				t2->prev=newNode;
				//Updates amount
				table[index].amount++;
				
				//addes one to filled Entries if the amount is reached
				if(table[index].amount==MAX_ELEMENTS){
					filledEntries++;
				}

				//Set new middleElement, needs to be like this due to the way nodes get added
				node* newMid;
				newMid=table[index].head;
				for(int i=0; i<table[index].amount/2; i++){
					if(newMid->next==nullptr){
						break;
					}
					newMid=newMid->next;
				}
				table[index].middleElement=newMid;

				//Retunrs value
				return newNode->value;
			}
			//linear probing head insert
			// boolean to go back on top
			if((it->key)<key2 && table[index].amount==MAX_ELEMENTS){
				index=(index+1)%tableSize;
				found=false;
				break;
			}
			//ensures probing is working
			if(found==false){
				cout<<"not correct"<<endl;
			}
		}
		//returns to top of loop to account for linear probing
		if(found==false){
			continue;
		}

		//fix indenting
		while((it->key)<key2){
			t2=it;
			it=it->next;

			//no linear probing tail insert
			if(it==nullptr && table[index].amount<MAX_ELEMENTS){
				node * newNode=new node;
				//head insert
				it=t2;
				table[index].amount++;
				newNode->prev=t2;
				newNode->next=nullptr;
				newNode->key=key2;
				newNode->value=type2();
				it->next=newNode;
				
				//Updates filledEntries if max amount for index is reached
				if(table[index].amount==MAX_ELEMENTS){
					filledEntries++;
				}

				//Set new middleElement, needs to be like this due to the way nodes get added
				node* newMid;
				newMid=table[index].head;
				for(int i=0; i<table[index].amount/2; i++){
					if(newMid->next==nullptr){
						break;
					}
					newMid=newMid->next;
				}
				table[index].middleElement=newMid;
				//cout<<"left tail insert"<<endl;
				return newNode->value;
			}

			//linear probing head insert
			// boolean to go back on top
			if(it==nullptr && table[index].amount==MAX_ELEMENTS){
				index=(index+1)%tableSize;
				found=false;
				break;
			}

			//If the current iterator has the key
			// then return without making any changes
			if(it->key==key2){
				return it->value;
			}
			//insert between elements
			if((it->key)>key2 && table[index].amount<MAX_ELEMENTS){
				//Inserts nodes inbetween other nodes
				node * newNode=new node;
				newNode->key=key2;
				newNode->prev=t2;
				newNode->next=t2->next;
				newNode->value=type2();
				t2->next->prev=newNode;
				t2->next=newNode;
				//Updates amount
				table[index].amount++;

				//addes one to filled Entries if the amount is reached
				if(table[index].amount==MAX_ELEMENTS){
					filledEntries++;
				}

				//Set new middleElement, needs to be like this due to the way nodes get added
				node* newMid;
				newMid=table[index].head;
				for(int i=0; i<table[index].amount/2; i++){
					if(newMid->next==nullptr){
						break;
					}
					newMid=newMid->next;
				}
				table[index].middleElement=newMid;

				//returns value
				return newNode->value;
			}

			//linear probing head insert
			// boolean to go back on top
			if((it->key)>key2 && table[index].amount==MAX_ELEMENTS){
				index=(index+1)%tableSize;
				found=false;
				break;
			}
			//Used to ensure linear probing is working
			if(found==false){
				cout<<"incorrect"<<endl;
			}

		}

		//Goes back to top of while loop to account for linear probing
		if(found==false){
			continue;
		}
	}while(doTask);

	//If it ever gets to this point (impossible) return this
	//Only here to remove warnings
	return table[index].head->value;
}

/** function_identifier: It will create a hash for the passed in key
 * * parameters: string key
 * * return value: hash*/
template <class type1, class type2>
int hashMap<type1, type2>::hashFunction(std::string key) const
{
	int sum=0;
	//Adds up values of the string
	for(unsigned int i=0; i<key.length(); i++){
		sum=sum+int(key[i]);
	}

	//retunrs index for the key
	return (sum%tableSize);
}

/** function_identifier: resizes the table to new size
 * * parameters: Size to add to current size
 * * return value: None*/
template <class type1, class type2>
void hashMap<type1, type2>::resize(int amt)
{
	list * oldTable = table;
	int oldSize = tableSize;
	tableSize += amt;
	filledEntries = 0;
	table = new list[tableSize];

	for (int i = 0; i < tableSize; i++)
	{
		table[i].head = NULL;
		table[i].middleElement = NULL;
		table[i].amount = 0;
	}

	node * it;

	//goes through each table entry of the oldTable (obsolete table)
	//and maps them to an element of the new table and stores the same
	//value into the location where it's found in the new table
	for (int i = 0; i < oldSize; i++)
		for (it = oldTable[i].head; it != NULL; it = it->next){
				(*this)[it->key] = it->value;
		}


	//Deallocate old table
	for (int i = 0; i < oldSize; i++)
	{
		node * del1, * del2;
		del1 = oldTable[i].head;

		while (del1 != NULL)
		{
			del2 = del1->next;
			delete del1;
			del1 = del2;
		}
	}
	delete [] oldTable;
}

#endif