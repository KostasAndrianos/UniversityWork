class PersonalTaxDeclaration:public TaxDeclaration{
private:
	double no_tax; // no_tax
	int receipt_count;
public:
	PersonalTaxDeclaration(const char*,const char*,const char*,int,double,double, int);
	PersonalTaxDeclaration();
	int tax_more_than_x(double);
	int receipts_costs(double);
	double calculate_taxes();
	void short_display(int, int);
	void edit(const char*,int);
	void file_input(const char*);//does nothing, syntax error if not included
};
