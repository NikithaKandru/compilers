


int Sum();
int Prod();

int main(){
	int a[10];
	for(int i =0; i<10; i++){
	    a[i] = i;
	}	
        int s= Sum();
	int p = Prod();

}

int Sum(){
	int sum=0;
	for(int i=0; i<10; i++){
		for(int j = 0; j<10; j++){
			sum = i+j;
		}
	}

	return sum;

}

int Prod(){

	int a[10];
	for(int i=0; i<10; i++){
		a[i] = 10-i;
	}

	int result;
	int prod[10];

	for(int i=0; i<10; i++){
		for(int j=0; j<10; j++){
			prod[j] = i*j;	
		}
		result += prod[i];
	}

	return result;

}
