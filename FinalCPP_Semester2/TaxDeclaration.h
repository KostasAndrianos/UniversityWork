class TaxDeclaration{
protected:
  char Name[51], Address[51], AFM[8];
  int Dyear; // declaration year;
  int cancelled;
  double total_income;
public:
  void sort_arrays(double*,string*,int); // first is tax array then its afm array then size of list
  virtual void short_display(int, int) = 0;
  virtual void edit(const char*, int) = 0;
  virtual int tax_more_than_x(double) = 0;
  virtual int receipts_costs(double) = 0;
  virtual double calculate_taxes() = 0;
  virtual void file_input(const char*)=0;
  TaxDeclaration(); // Default Constructor
  TaxDeclaration(const char*,const char*,const char*,int,double); // General Constructor
  //getters and setters
  string get_name(){
    return Name;
  }

  string get_address(){
    return Address;
  }

  string get_afm(){
    return AFM;
  }

  int get_declaration_year(){
    return Dyear;
  }

  int get_total_income(){
    return total_income;
  }

  void set_address(const char* addz){
    strncpy(Address,addz,20);
  }

  void set_name(const char* addz){
    strncpy(Name,addz,20);
  }

  void set_afm(const char* addz){
    strncpy(AFM,addz,20);
  }

  void set_dyear(int addz){
    Dyear=addz;
  }

  void set_total_income(double addz){
    total_income=addz;
  }

  int cancel_it(){
    cancelled = 1;
  }
};
