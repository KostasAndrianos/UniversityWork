class CorporateTaxDeclaration:public TaxDeclaration{
private:
	double company_costs;
	char supervisorName[51];
public:
	CorporateTaxDeclaration(const char*,const char*,const char*,int,double,double, const char*);
	CorporateTaxDeclaration();
	int tax_more_than_x(double);
	int receipts_costs(double);
	double calculate_taxes();
	void short_display(int, int);
	void edit(const char*, int);
	void file_input(const char*);
};
