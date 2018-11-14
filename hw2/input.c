


int main(){
	int i =0;
	int a[10];
	for(i =0; i<10; i++){
	    a[i] = i;
	    for(int j=0; j<i;j++){    
		a[j] = j*2;
	    }
	}	

	for(int k=0; k<10; k++){
	   a[i] = a[i]-1;
	}
	return 0;
}
