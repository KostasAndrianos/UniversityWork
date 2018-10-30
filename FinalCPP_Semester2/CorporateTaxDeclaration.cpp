CorporateTaxDeclaration::CorporateTaxDeclaration(){
	//Just for the sake of comparing something's class to this one's using an empy object as an example
}

CorporateTaxDeclaration::CorporateTaxDeclaration(const char* n, const char* a, const char* afm, int y, double t, double costs,const char* s_n):TaxDeclaration(n, a, afm,  y,  t){
	// Using Father's Constructor for all but car_age, that one is on the kid class.
	company_costs=costs;
	strncpy(supervisorName,s_n,51);              // again, strncpy for safety
}

double CorporateTaxDeclaration::calculate_taxes(){
	double taxes;
	if (company_costs>get_total_income())
		taxes=0; // if costs > income
	else
		taxes=( get_total_income() - company_costs )*0.26;
	return taxes;
}

void CorporateTaxDeclaration::edit(const char* changer, int choice_){
	switch(choice_){ //takes choice (input from interface
		case 1 :	//total_income
			set_total_income(atof(changer));	 // gives double value on setter.
			break;
		case 2 : //costs
			company_costs=(atof(changer)); // gives double value on class variable through atof
			break;
		case 3 : //Address
			set_address(changer); // gives direct value on setter
			break;
		default :  // default case, can use this for error handling
			break;
	}
}

void CorporateTaxDeclaration::short_display(int counter, int flag_for_top_ten){
	mvwprintw(wm, 0+counter, 5,  "Type: Corporate.");
	if (!cancelled) mvwprintw(wm, 0+counter, 22,  "Valid!");
	else mvwprintw(wm, 0+counter, 22,  "Cancelled!");
	mvwprintw(wm, 1+counter, 5,  "Name:");
	mvwprintw(wm, 1+counter, 10, Name);
	wrefresh(wm);
	mvwprintw(wm, 2+counter, 5,  "AFM:");
	mvwprintw(wm, 2+counter, 9,  AFM);
	wrefresh(wm);
	if(flag_for_top_ten){
		mvwprintw(wm, 3+counter, 5, "Tax:");
		mvwprintw(wm, 3+counter, 9,  "%f", calculate_taxes());
		wrefresh(wm);
	}
}

int CorporateTaxDeclaration::tax_more_than_x(double x){
	// let's say X is 3000
	double check=0;
	check=calculate_taxes();
		if (check>x)
			return 1;
		else
			return 0;
}

int CorporateTaxDeclaration::receipts_costs(double x){
	if (company_costs>x) // if company costs more than X then
		return 1; //
	else
		return 0;
}

// damn dis is big
void CorporateTaxDeclaration::file_input(const char *filename){
	string text; // every line
	string segment=""; // segment between ,'s
	string date=""; // date string
	string money=""; // money , income or costs string
	double total_Costs=0;
	double total_Income=0;
	char ic; // character that checks what type of money is it, cost or income
	char ch; // character that adds to our segment
	char chd; // character that adds to date segment
	int seg_count=0; // segment count, money type/how much money/date
	int i,j; // for loops
	bool cost_or_income=true; // flag for type cost/income
	bool current_year=false; // flag to check if year is good, starts as false;

	/*****************************************************************************************************************************************/

	ifstream ifile(filename); // this we get from input file_input

	/*****************************************************************************************************************************************/

	if(!ifile.good()) throw 404; // exception if input file is not found 404

	// custom algorithm without the use of sscanf. Custom algorithm was created as a challenge.
	// this is basically a custom sscanf, we can use this for as many segments  we like , differentiated by the ',' character
	// format of text file is   type1,money1,date1,
	// type2,money2,date2,    etc. At the end of each line there's a ','  character
	// Skip this algorith (end of while) to see where the costs & incomes are added to.
	while (!ifile.eof()){
		getline(ifile,text); // getting 1 line at a time
		ic = text[0]; // checking if it's Income or Cost and saving to char ic. Type  is always the first character in a line afterall.
		//for loop for each line, going character by character
		for(i=0; i<=text.length(); i++){
			ch = text[i]; // character by character is saved on ch , added to segment string later.

			if(ch==','){
				seg_count++; // using this variable to have a basis on which segment we are at. Segment 1 = type of i/c, Segment 2 = value of money , Segment 3 = date. Segment_count starts at 0 and resets after every line
				// once we find a , we get add a seg_count
				switch(seg_count){
					case 1:      // Is it income or costs case
						if(ic=='C') cost_or_income=true;// INCOME , flags true for income
					  else if(ic=='I') cost_or_income=false;// COST , flags false for cost
					  else throw 99;	//error in file, corrupted I/C type
						break;
					case 2: // We cannot add without checking date first, just copying the value of money here.
						money=segment; //copying for later , money string saved
						break;
					case 3:
						for(j=6;j<segment.length();j++){
							chd=segment[j];
							date+=chd; // date string saved
						}
						if(atoi(date.c_str())==get_declaration_year()){ // converting date string to int // IS THIS THE CURRENT YEAR?
							current_year=true; // flag for current_year true, we might need it.
							//Below - adding total costs and total incomes
							if(cost_or_income)  total_Costs+=atof(money.c_str()); // atof for money conversion // cost_or_income (income flag)
							else if(!cost_or_income) total_Income+=atof(money.c_str()); // cost_or_income (costs flag)
						}
						else{ // if we aren't at the current year we don't do anything.
							current_year=false; // flagging current_year false, we might need it.
						}
						break;
					default:
						throw 150;// how did u end up here? ... I don't think you can. Throwing exception JUST IN CASE
						break;
				} // end of switch

				date =""; // resetting date
				segment=""; // reseting segment since we re  going into the next one because we found a ',' character
			} // end of IF ( new segment )


    		if(ch!=',' && ch!=32) segment += ch;	// makes sure for above code to work to not add any ' , ' characters or space bars in the segment.
		} // for loop that uses this line closes

		segment=""; // resetting segment , going to next line
		seg_count=0; // resetting segment number , going to next line

	} // while end of file closes

	// starting to add from file HERE

	//company_costs is old Costs
	// total_Costs is costs from file
	//setting new company_costs

	company_costs+=total_Costs; /*<----------INCOME CHANGE*/

	//get_total_income() is old income
	//total_Income is income from file
	//setting new totalIncome
	double newincome=0;	// declaring a new double just to make it simpler for the eye
	newincome=get_total_income()+total_Income;	//calculating value

	set_total_income(newincome); /*<----------COSTS CHANGE*/
	ifile.close();
}	//end of file_input() function
