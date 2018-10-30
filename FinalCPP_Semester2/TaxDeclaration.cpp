TaxDeclaration::TaxDeclaration(const char* n, const char* a, const char* afm, int y, double t){
	strncpy(Name,n,51); // using strncpy instead of strcpy for safety
	strncpy(Address,a,51);
	strncpy(AFM,afm,8);
	cancelled = 0;
	Dyear=y;
	total_income=t;
}

TaxDeclaration::TaxDeclaration(){
	//Just for the sake of comparing something's class to this one's using an empy object as an example
}

void TaxDeclaration::sort_arrays(double *tax,string *afm,int size){
  int i,j,flag=1;
  double temp;
  string tempstring;
  //bubble sorting because we need to also swap around afm values
  for(i=1;i<=size &&flag;i++){ // dont forget , you get the length dynamically , the 49 is a placeholder
		flag=0;
		for(j=0;j<(size-1);j++){
			if(tax[j+1] > tax[j]){
				temp=tax[j];
				tax[j]=tax[j+1]; // tax sort
				tax[j+1]=temp;
				tempstring=afm[j];
				afm[j]=afm[j+1]; // afm sort , changing afm's around when their taxes change around in array as well
				afm[j+1]=tempstring;
				flag=1;
			}
  	}
  }
}
