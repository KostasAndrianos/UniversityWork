#include <iostream>
#include <curses.h>
#include <string>
#include <cstring>
#include <list>
#include <stdlib.h>
#include <typeinfo>
#include <ostream>
#include <fstream>
using std::string;
using std::list;
using std::ifstream;

//Menu stuff
WINDOW *wt, *wm, *ws;
//Menu stuff

#include "TaxDeclaration.h"
#include "TaxDeclaration.cpp"
#include "PersonalTaxDeclaration.h"
#include "PersonalTaxDeclaration.cpp"
#include "CorporateTaxDeclaration.h"
#include "CorporateTaxDeclaration.cpp"

void top_ten_each_kind(list<TaxDeclaration*> **);
void print_prompt(int , const char *);
void yo_wait(int, const char*);
void cancel_tax_declaration(list<TaxDeclaration*> li);
void general_edit(list<TaxDeclaration*> li, int choice);
void print_list_short(list<TaxDeclaration*>, int, int);
void add_new_corporate(list<TaxDeclaration*>**, char **, int);
void add_new_personal(list<TaxDeclaration*>**);
void init_screen();
void menu();
void submenu_4(list<TaxDeclaration*>*);
void submenu_2(list<TaxDeclaration*>*);
void submenu_1(list<TaxDeclaration*>*);

int main(){
	init_screen();
	menu();
	endwin();
	return 0;
}

//############################################################################### GENERAL PURPOSE STUFF ###############################################################################

//Print some critical details of each Tax Declaration for choosing one of them for choice 2, and choice 3

void print_list_short(list<TaxDeclaration*> li, int print_what, int flag = 0){ //"print_what" is a choice among 0,1,2 where 0 prints everything, 1 prints Corporate and 2 prints Personal
	werase(wm);
	wrefresh(wm);
	PersonalTaxDeclaration ptd_reference;
	CorporateTaxDeclaration ctd_reference;
	list<TaxDeclaration*>::iterator it;
	int counter=0;
	for(it = li.begin(); li.end() != it; it++){
		switch (print_what) {
			case 0:(*it)->short_display(counter, flag);counter = counter + 4;; break;
			case 1:if(typeid(ctd_reference) == typeid(*(*it))){ (*it)->short_display(counter, flag); counter = counter + 4;} break;
			case 2:if(typeid(ptd_reference) == typeid(*(*it))){ (*it)->short_display(counter, flag); counter = counter + 4;} break;
		}
		if (counter == 16){
			counter = 0;
			yo_wait(18, "Press anything for more.");
			werase(wm);
		}
	}
}

//Print a list of the top 10 declarations (needed in choice 4.c)

void print_for_top_ten(int size, string *afm, list<TaxDeclaration*> ***li, int flag){
  werase(wm);
  wrefresh(wm);
  list<TaxDeclaration*>::iterator it;
  int counter = 0;
  for (int i = 0; i < 10 && i < size; i++){
    for(it = (***li).begin(); (***li).end() != it; it++){
      if(afm[i] == (*it)->get_afm()){
        (*it)->short_display(counter, flag);
        counter = counter + 5;
        break;
      }
    }
    if (counter == 15){
		counter = 0;
		yo_wait(18, "Press anything for more.");
		werase(wm);
	}
  }
}

//Get user input for choice 2 and choice 3, choosing a Tax Declaration by the IRS Number

void get_user_choice(char *choice, int size){
	curs_set(1);
	echo();
	mvwgetnstr(wm, 18, 23, choice, size);
	curs_set(0);
	noecho();
}

//Gets user input for double numbers

void get_user_choice_double(double *num, int size){
	char *temp = new char[size];
	curs_set(1);
	echo();
	mvwgetnstr(wm, 15, 30, temp, size);
	curs_set(0);
	noecho();
	*num = atof(temp);
	delete[] temp;
}

//This function tells you how many TaxDeclarations have a Tax more than the value given.

void decl_more_than_x(list<TaxDeclaration*> **li){
	double x;
	int counter = 0;

	print_prompt(15, "Give any amount of money:");
	get_user_choice_double(&x, 12);
	list<TaxDeclaration*>::iterator it;
	for(it = (**li).begin(); (**li).end() != it; it++){
		counter = counter + (*it)->tax_more_than_x(x);
	}
	mvwprintw(wm, 16, 5, "Declarations with Tax over %f : %d", x, counter);
	wrefresh(wm);
	yo_wait(17, "Press anything to return");
	werase(wm);
}

//Function that tells you how many TaxDeclarations have costs more than X or have at least one receipt

void have_receipts_or_costs(list<TaxDeclaration*> **li){
	double x;
	int counter = 0;
	print_prompt(15, "Give any amount of money:");
	get_user_choice_double(&x, 12);
	list<TaxDeclaration*>::iterator it;
	for(it = (**li).begin(); (**li).end() != it; it++){
		counter = counter + (*it)->receipts_costs(x);
	}
	mvwprintw(wm, 16, 5, "Declarations with costs over %f or with receipts: %d", x, counter);
	wrefresh(wm);
	yo_wait(17, "Press anything to return");
	werase(wm);
}

//Function that takes the list as input and marks a TaxDeclaration as invalid.

void cancel_tax_declaration(list<TaxDeclaration*> li){
	char AFM_choice[9];
	print_list_short(li, 0);
	print_prompt(18, "Choose by AFM:");
	get_user_choice(AFM_choice, 8);
	list<TaxDeclaration*>::iterator it;
	for(it = li.begin(); li.end() != it; it++){
		if ((*it)->get_afm() == AFM_choice){
			(*it)->cancel_it();
			yo_wait(19, "Cancellation Successful! Press anything to return.");
			break;
		}
	}
}

//Function that takes the list as input and depending on the "choice" we pass in, changes something.
//each choice is described in the comments inside the function.

void general_edit(list<TaxDeclaration*> li, int choice){
	char AFM_choice[8];
	//Choice 0:Income(Applies to everyone)
	//Choice 1:Costs(Applies to CorporateTaxDeclaration)
	//Choice 2:Receipt count(Applies to PersonalTaxDeclaration)
	//Choice 3:Address(Applies to everyone)
	if(choice == 0 || choice == 3) print_list_short(li, 0);
	else if(choice == 1) print_list_short(li, 1);
	else if(choice == 2) print_list_short(li, 2);
	print_prompt(18, "Choose by AFM:");
	get_user_choice(AFM_choice, 8);
	list<TaxDeclaration*>::iterator it;
	for(it = li.begin(); li.end() != it; it++){
		if ((*it)->get_afm() == AFM_choice){
			curs_set(1);
			echo();
			if(choice == 0){
				char income[13];
				print_prompt(19, "Income....:");
				mvwgetnstr(wm, 19, 17, income, 12);
				(*it)->edit(income, 1);
			}
			else if(choice == 1){
				char costs[13];
				print_prompt(19, "Costs......");
				mvwgetnstr(wm, 19, 17, costs, 12);
				(*it)->edit(costs, 2);
			}
			else if(choice == 2){
				char rec_count[6];
				print_prompt(19, "Receipt Count..:");
				mvwgetnstr(wm, 19, 21, rec_count, 5);
				(*it)->edit(rec_count, 2);
			}
			else if(choice == 3){
				char address[51];
				print_prompt(19, "Address........:");
				mvwgetnstr(wm, 19, 21, address, 50);
				(*it)->edit(address, 3);
			}
			curs_set(0);
			noecho();
			yo_wait(19, "Action Successful! Press anything to return.");
			break;
		}
	}
}

//That is for CorporateTaxDeclaration

void add_new_corporate(list<TaxDeclaration*>** li, char** afm, int flag = 0){
	curs_set(1);
	echo();
	char name[51], address[51], supervisor[51], year[5], income[13] = "", costs[13] = "";
	int year_;
	double income_,costs_;
	// I/O
	werase(wm);
	print_prompt(4, "Name...........:");
	mvwgetnstr(wm, 4, 21, name, 50);
	print_prompt(5, "Address........:");
	mvwgetnstr(wm, 5, 21, address, 50);
	print_prompt(6, "AFM............:");
	mvwgetnstr(wm, 6, 21, *afm, 7);
	print_prompt(7, "Year...........:");
	mvwgetnstr(wm, 7, 21, year, 4);
	print_prompt(8, "Supervisor.....:");
	mvwgetnstr(wm, 8, 21, supervisor, 50);
	if (flag == 0){
		print_prompt(9, "Income.........:");
		mvwgetnstr(wm, 9, 21, income, 12);
		print_prompt(10, "Costs...........");
		mvwgetnstr(wm, 10, 21, costs, 12);
	}
	curs_set(0);
	noecho();

	year_ = atoi(year);
	income_ = atof(income);
	costs_ = atof(costs);
	CorporateTaxDeclaration* ptr = new CorporateTaxDeclaration(name, address, *afm, year_, income_, costs_, supervisor);
	(**li).push_back(ptr);
}

//That is for PersonalTaxDeclaration

void add_new_personal(list<TaxDeclaration*>** li){
	curs_set(1);
	echo();
	char name[51], address[51], afm[8], rec_count[6], year[5], income[13], nt_lim[8];
	int year_, rec_count_;
	double income_, no_tax;
	werase(wm);
	print_prompt(4, "Name...........:");
	mvwgetnstr(wm, 4, 21, name, 50);
	print_prompt(5, "Address........:");
	mvwgetnstr(wm, 5, 21, address, 50);
	print_prompt(6, "AFM............:");
	mvwgetnstr(wm, 6, 21, afm, 7);
	print_prompt(7, "Year...........:");
	mvwgetnstr(wm, 7, 21, year, 4);
	print_prompt(8, "Income.........:");
	mvwgetnstr(wm, 8, 21, income, 12);
	print_prompt(9, "Receipt Count..:");
	mvwgetnstr(wm, 9, 21, rec_count, 5);
	print_prompt(10, "No Tax Limit...:");
	mvwgetnstr(wm, 10, 21, nt_lim, 7);
	curs_set(0);
	noecho();

	year_ = atoi(year);
	income_ = atof(income);
	rec_count_ = atoi(rec_count);
	no_tax = atof(nt_lim);
	PersonalTaxDeclaration* ptr = new PersonalTaxDeclaration(name, address, afm, year_, income_, no_tax, rec_count_);
	(**li).push_back(ptr);
}

//This adds a new CorporateTaxDeclaration but extracts the cost and income information from a file.

void add_new_corporate_wfile(list<TaxDeclaration*> ***li){
	char *filename = new char[50];
	print_prompt(18, "Give the filename:");
	get_user_choice(filename, 50);
	char *afm = new char[8];
	add_new_corporate(*li, &afm, 1);
	wrefresh(wm);
	try{
		list<TaxDeclaration*>::iterator it;
		for(it = (***li).begin(); (***li).end() != it; it++){
			if((*it)->get_afm() == afm){
				(*it)->file_input(filename);
				break;
			}
		}
	}
	catch(int ex){
		if(ex==404){
			print_prompt(14, "404 ERROR - CANNOT FIND INPUT FILE");
			(***li).pop_back(); //Hadn't I deleted the last element, it'd have added a TaxDeclaration without any costs and/or income
			delete[] afm; //It was allocated using "new"
			delete[] filename; //Same.
			getch();
		}
		if(ex==99){
			print_prompt(14, "\nERROR IN FILE , TYPE OF MONEY (I/C) IS CORRUPTED");
			(***li).pop_back();
			delete[] afm;
			delete[] filename;
			getch();
		}
		if(ex==150){
			print_prompt(14, "\nUNKNONN ERROR - check throw declaration in code");
			(***li).pop_back();
			delete[] afm;
			delete[] filename;
			getch();
		}
	}
}

//The shell is needed to pass compilation because the submenu_1 function can only give up to double pointers and a triple pointer is required for add_new_corporate_wfile

void add_new_corporate_wfile_shell(list<TaxDeclaration*> **li){
	add_new_corporate_wfile(&li);
}

//Change the Cost and Income attributes with the ones extracted from the specified file.

void edit_corp_wfile(list<TaxDeclaration*>** li){
	char *filename = new char[50];
	print_prompt(18, "Give the filename:");
	get_user_choice(filename, 50);
	char AFM_choice[8];
	print_list_short(**li, 1);
	print_prompt(18, "Choose by AFM:");
	get_user_choice(AFM_choice, 8);
	try{
		list<TaxDeclaration*>::iterator it;
		for(it = (**li).begin(); (**li).end() != it; it++){
			if((*it)->get_afm() == AFM_choice){
				(*it)->file_input(filename);
				break;
			}
		}
	}
	catch(int ex){
		if(ex==404){
			print_prompt(14, "404 ERROR - CANNOT FIND INPUT FILE");
			delete[] filename;
			getch();
		}
		if(ex==99){
			print_prompt(14, "\nERROR IN FILE , TYPE OF MONEY (I/C) IS CORRUPTED");
			delete[] filename;
			getch();
		}
		if(ex==150){
			print_prompt(14, "\nUNKNONN ERROR - check throw declaration in code");
			delete[] filename;
			getch();
		}
	}
}

//This function searches deals with choice 4.c and finds out what to print. Essentially it prints the top ten TaxDeclarations from each kind.

void top_ten_each_kind(list<TaxDeclaration*> **li){
	werase(wm);
	wrefresh(wm);
	double ins=0;
	int counterp=0, counterc=0, counter=0;
	int sizeC; // number of Corporate Declarations
	int sizeP; // number of Personal Declarations
	PersonalTaxDeclaration pers_ref;
	CorporateTaxDeclaration corp_ref;
	list<TaxDeclaration*>::iterator it;
	for(it = (*li)->begin(); it != (*li)->end(); it++){
    if(typeid(*(*it)) == typeid(pers_ref)) counterp++; // how many P declarations
    else if(typeid(*(*it)) == typeid(corp_ref)) counterc++; // how many C declarations
	}
	sizeC = counterc; // number of Corporate Declarations
	sizeP = counterp;// number of Personal Declarations
	// allocating memory on arrays with taxes and afms accordingly
	double *maxP = new double[sizeP];
	double *maxC = new double[sizeC];
	string *afmP = new string[sizeP];
	string *afmC = new string[sizeC];
	// initialization of arrays with nothing at first
	for(counter = 0; counter < sizeP; counter++){
		maxP[counter] = 0; // initilazing array type double
		afmP[counter] = "N/A"; //initilazing array type string
	}
	for(counter = 0;counter < sizeC; counter++){
		maxC[counter] = 0;
		afmC[counter] = "N/A";
	}
	counterp = 0;
	counterc = 0;
  counter = 0;
	for(it = (*li)->begin(); it != (*li)->end(); it++){
		ins=(*it)->calculate_taxes();
    if(typeid(*(*it)) == typeid(pers_ref)){
      maxP[counterp] = ins; // initialization with taxes from objects
      afmP[counterp] = (*it)->get_afm(); // initialization with afm's from objects
      counterp++;
    }
    else if(typeid(*(*it)) == typeid(corp_ref)){
      maxC[counterc] = ins; // initialization with taxes from objects
      afmC[counterc] = (*it)->get_afm(); // initialization with afm's from objects
      counterc++;
    }
		counter++; // forwards
	}
	//sort the arrays
	for(it = (*li)->begin(); it != (*li)->end(); it++){
		//type id code here that works to choose which arrays to give, let's suppose we got all of them in 1 for now.
		if(typeid((*it)) == typeid(pers_ref)) (*it)->sort_arrays(maxP,afmP,sizeP); // personal typeid
		if(typeid((*it)) == typeid(corp_ref)) (*it)->sort_arrays(maxC,afmC,sizeC); // corporate typeid
	}
	print_for_top_ten(sizeC, afmC, &li, 1);
	yo_wait(18, "Press anything to proceed");
	print_for_top_ten(sizeP, afmP, &li, 1);
	yo_wait(18, "Press anything to return.");
	//DELETE DYNAMICALLY ALLOCATED MEMORY
	delete[] maxP;
	delete[] maxC;
	delete[] afmP;
	delete[] afmC;
}

//#################################################################################### MENU STUFF ####################################################################################

//Prepare the UI

void init_screen(){
	initscr();
	start_color();
	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, FALSE);
	curs_set(0);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	wt = newwin(3, 80, 0, 0);
	clearok(wt, true);
	scrollok(wt, false);
	wcolor_set(wt, 1, 0);
	werase(wt);
	wborder(wt, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(wt);
	wm = newwin(25, 81, 4, 0);
	clearok(wm, 0);
	scrollok(wm, false);
	wcolor_set(wm, 1, 0);
	werase(wm);
	keypad(wm, TRUE);
	wrefresh(wm);
	ws = newwin(3, 81, 25, 0);
	clearok(ws, 0);
	wcolor_set(ws, 1, 0);
	werase(ws);
	wborder(wt, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(ws);
	wattr_on(wt, COLOR_PAIR(2), 0);
	mvwprintw(wt, 1, 27, "Tax Declaration System v1.0");
	wattr_off(wt, COLOR_PAIR(2), 0);
	wrefresh(wt);
}

//Main menu

void menu(){
	list<TaxDeclaration*> general_purp;
	const char* Items[] = {
		"[1]...New tax declaration",
		"[2]...Modify tax declaration",
		"[3]...Cancel tax declaration",
		"[4]...Additional Information",
		"[5]...Exit"
	};
	int Cur = 0;
	int ch, i;
	int flag = 0;
	do{
		werase(wm);
		mvwprintw(wm, 2, 16, "MENU");
		for(i = 0; i < 5; i++){
			if(i == Cur)
				wattr_on(wm, A_REVERSE, 0);
			mvwprintw(wm, 4+i, 5, Items[i]);
			wattr_off(wm, A_REVERSE, 0);
		}
		mvwprintw(wm, 14, 5, "Choice: ");
		wprintw(wm, "%1d", Cur+1);
		wrefresh(wm);
		ch=wgetch(wm);
		switch(ch){
			case '1':Cur = 0;submenu_1(&general_purp);break;
			case '2':Cur = 1;submenu_2(&general_purp);break;
			case '3':Cur = 2;cancel_tax_declaration(general_purp);break;
			case '4':Cur = 3;submenu_4(&general_purp);break;
			case '5':flag = 1;break;
			case KEY_UP:
			case KEY_LEFT: Cur--; if (Cur < 0) Cur = 4; break;
			case KEY_DOWN:
			case KEY_RIGHT: Cur++; if(Cur > 4) Cur = 0; break;
			case 27: flag = 1; break;
			case 32:
			case 13:
				switch (Cur){
					case 0:submenu_1(&general_purp);break;
					case 1:submenu_2(&general_purp);break;
					case 2:cancel_tax_declaration(general_purp);break;
					case 3:submenu_4(&general_purp);break;
					case 4:flag = 1;break;
				}
		}
	}while(!flag);
}

//Submenu for choice 1

void submenu_1(list<TaxDeclaration*>* li){
	char *afm = new char[9];
	const char* Items[] = {
		"[1]...Corporate",
		"[2]...Personal",
		"[3]...Corporate (extract Income and Costs from file)",
		"[4]...Back to main menu"
	};
	int Cur = 0;
	int ch, i;
	int flag = 0;
	do{
		werase(wm);
		mvwprintw(wm, 2, 16, "Submenu 1");
		for(int i = 0; i < 4;i ++){
			if(i==Cur)
				wattr_on(wm, A_REVERSE, 0);
			mvwprintw(wm, 4+i, 5, Items[i]);
			wattr_off(wm, A_REVERSE, 0);
		}
		mvwprintw(wm, 14, 5, "Choice: ");
		wprintw(wm, "%1d", Cur+1);
		wrefresh(wm);
		ch=wgetch(wm);
		switch(ch){
			case '1':Cur = 0;add_new_corporate(&li, &afm);break;
			case '2':Cur = 1;add_new_personal(&li);break;
			case '3':Cur = 2;add_new_corporate_wfile_shell(&li);break;
			case '4':flag = 1; break;
			case KEY_UP:
			case KEY_LEFT: Cur--; if (Cur < 0) Cur = 3; break;
			case KEY_DOWN:
			case KEY_RIGHT: Cur++; if(Cur > 3) Cur = 0; break;
			case 27: flag = 1; break;
			case 32:
			case 13:
				switch (Cur){
					case 0:add_new_corporate(&li, &afm);break;
					case 1:add_new_personal(&li);break;
					case 2:add_new_corporate_wfile_shell(&li);break;
					case 3:flag = 1;break;
				}
		}
	}while(!flag);
	delete[] afm;
}

//Submenu for choice 2

void submenu_2(list<TaxDeclaration*>* li){
	const char* Items[] = {
		"[1]...Income",
		"[2]...Costs",
		"[3]...Receipt Count",
		"[4]...Address",
		"[5]...Extract Costs and Income from file (Corporate)",
		"[6]...Back to main menu"
	};
	int Cur = 0;
	int ch, i;
	int flag = 0;
	do{
		werase(wm);
		mvwprintw(wm, 2, 16, "Submenu 2");
		for(int i = 0; i < 6; i++){
			if(i == Cur)
				wattr_on(wm, A_REVERSE, 0);
			mvwprintw(wm, 4+i, 5, Items[i]);
			wattr_off(wm, A_REVERSE, 0);
		}
		mvwprintw(wm, 14, 5, "Choice: ");
		wprintw(wm, "%1d", Cur+1);
		wrefresh(wm);
		ch=wgetch(wm);
		switch(ch){
			case '1':Cur = 0;general_edit(*li, 0);break;
			case '2':Cur = 1;general_edit(*li, 1);break;
			case '3':Cur = 2;general_edit(*li, 2);break;
			case '4':Cur = 3;general_edit(*li, 3);break;
			case '5':Cur = 4;edit_corp_wfile(&li);break;
			case '6':flag = 1;break;
			case KEY_UP:
			case KEY_LEFT: Cur--; if (Cur < 0) Cur = 5; break;
			case KEY_DOWN:
			case KEY_RIGHT: Cur++; if(Cur > 5) Cur = 0; break;
			case 27: flag = 1; break;
			case 32:
			case 13:
				switch (Cur){
					case 0:general_edit(*li, 0);break;
					case 1:general_edit(*li, 1);break;
					case 2:general_edit(*li, 2);break;
					case 3:general_edit(*li, 3);break;
					case 4:edit_corp_wfile(&li);break;
					case 5:flag = 1;break;
				}
		}
	}while(!flag);
}

//Submenu for choice 4

void submenu_4(list<TaxDeclaration*> *li){
	const char* Items[] = {
		"[1]...Declarations with tax over X",
		"[2]...Declarations with costs over X or with receipts",
		"[3]...Top 10 declarations of each kind",
		"[4]...Back to main menu"
	};
	int Cur = 0;
	int ch, i;
	int flag = 0;
	do{
		werase(wm);
		mvwprintw(wm, 2, 16, "Submenu 4");
		for(i = 0; i < 4; i++){
			if(i == Cur)
				wattr_on(wm, A_REVERSE, 0);
			mvwprintw(wm, 4+i, 5, Items[i]);
			wattr_off(wm, A_REVERSE, 0);
		}
		mvwprintw(wm, 14, 5, "Choice: ");
		wprintw(wm, "%1d", Cur+1);
		wrefresh(wm);
		ch=wgetch(wm);
		switch(ch){
			case '1':decl_more_than_x(&li);Cur = 0;break;
			case '2':have_receipts_or_costs(&li);Cur = 1;break;
			case '3':top_ten_each_kind(&li);Cur = 2;break;
			case '4':flag = 1;break;
			case KEY_UP:
			case KEY_LEFT: Cur--; if (Cur < 0) Cur = 3; break;
			case KEY_DOWN:
			case KEY_RIGHT: Cur++; if(Cur > 3) Cur = 0; break;
			case 27: flag = 1; break;
			case 32:
			case 13:
				switch (Cur){
					case 0:decl_more_than_x(&li);break;
					case 1:have_receipts_or_costs(&li);break;
					case 2:top_ten_each_kind(&li);break;
					case 3:flag = 1;break;
				}
		}
	}while(!flag);
}

//after the end of a proccess, it needs to let the user see the screen printed, so it shouts "Yo man, wait up" and the programm magically stops

void yo_wait(int line, const char* ch){
	print_prompt(line, ch);
	getch();
}

//Prints messages to prompt the user to enter stuff

void print_prompt(int line, const char *ch){
	mvwprintw(wm, line, 5,  ch);
	wrefresh(wm);
}
