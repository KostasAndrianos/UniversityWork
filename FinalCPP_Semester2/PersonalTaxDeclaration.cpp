PersonalTaxDeclaration::PersonalTaxDeclaration(){
	//Just for the sake of comparing something's class to this one's using an empy object as an example
}

PersonalTaxDeclaration::PersonalTaxDeclaration(const char* n, const char* a, const char* afm, int y, double t, double nt, int r_count):TaxDeclaration(n, a, afm,  y,  t){
	// Using Father's Constructor for all but car_age, that one is on the kid class.
	no_tax=nt; // discuss this, problem with PDF , this has to be no_tax instead of no_tax_max probably
	receipt_count=r_count;
}

void PersonalTaxDeclaration::file_input(const char*){
	//does nothing, syntax error if not included because we have pure virtual on 'abstract' class TaxDeclaration
}

double PersonalTaxDeclaration::calculate_taxes(){
	double taxes;
	// we need 2 here, one for when no_tax_max is less than income with no receipts and one with receipts when no_tax_max is more than income
	if (no_tax<=9000 ) //no_tax_max is 9k
		taxes=( get_total_income() - no_tax )*0.2 - receipt_count*0.1;
	else
		taxes=( get_total_income() - no_tax )*0.2;
	return taxes;
}

void PersonalTaxDeclaration::short_display(int counter, int flag_for_top_ten){
	mvwprintw(wm, 0+counter, 5,  "Type: Personal.");
	if (!cancelled) mvwprintw(wm, 0+counter, 22,  "Valid!");
	else mvwprintw(wm, 0+counter, 22,  "Cancelled!");
	mvwprintw(wm, 1+counter, 5,  "Name:");
	mvwprintw(wm, 1+counter, 10, Name);
	wrefresh(wm);
	mvwprintw(wm, 2+counter, 5, "AFM:");
	mvwprintw(wm, 2+counter, 9,  AFM);
	wrefresh(wm);
	if(flag_for_top_ten){
		mvwprintw(wm, 3+counter, 5, "Tax:");
		mvwprintw(wm, 3+counter, 9,  "%f", calculate_taxes());
		wrefresh(wm);
	}
}

void PersonalTaxDeclaration::edit(const char* changer,int choice_){
	switch(choice_){ //takes choice (input from main
		case 1 :	//total_income
			set_total_income(atof(changer));
			break;
		case 2 : 	//receipt count
			receipt_count=(atoi(changer)); // gives int value on class variable through atoi
			break;
		case 3 : //Address
			set_address(changer);
			break;
		default : // default case, can use this for error handling
			break;
	}
}

int PersonalTaxDeclaration::tax_more_than_x(double x){
	// let's say X is 3000
	double check=0;
	check=calculate_taxes();
		if (check>x) // checks if tax is more than X
			return 1; // returns counter ++
		else
			return 0; // returns 0 , counter remains the same.
}

int PersonalTaxDeclaration::receipts_costs(double x){ // double x is not used here as commented on PersonalTaxDeclaration.h  - It is merely present for syntax reasons because it derives from pure virtual. We cannot overload this one.
	if (receipt_count>0) // if there are any receipts for this taxDeclaration
		return 1; // count ++
	else
		return 0; // else count remains same
}
