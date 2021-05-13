//total bits = 4*10^9(all numbers) * 1 bit = 4*10^9 bits
//assume is 32 bit
//slots = max 32 bit numbers(0xffffffff)/8 bits in a byte
void find_missing_when_all_bits_are_stored(const char *filename){
    string line;
    int radix = 8;
    int slots = 0xffffffff/radix;
    unsigned char bitfield[slots]={0};
    ifstream myfile(filename);
    //load integers in the slots
    while(getline(myfile,line))
    {
        int n = atoi(line.c_str());
        bitfield[n/radix] |= (1 << (n%radix));
    }

    for(int i = 0; i< slots; i++){
        for(int j =0; j<radix; j++){
            if( (bitfield[i] & (1<<j)) == 0)
		  cout<<(i*radix+j)<<endl;
        }
    }
}

void find_missing_number(const char *filename, int prefix){
    string line;
    int radix = 8;
    int slots = 0xffff/radix;
    unsigned char bitfield[slots]={0};
    ifstream myfile(filename);
    //load integers in the slots
    while(getline(myfile,line))
    {
        int n = atoi(line.c_str());
	if((n>>16)==prefix)
          bitfield[(n&0xffff)/radix] |= (1 << ((n&0xfff)%radix));
    }

    for(int i = 0; i< slots; i++){
        for(int j =0; j<radix; j++){
            if( (bitfield[i] & (1<<j)) == 0)
		  cout<<((prefix<<16)|(i*radix+j))<<endl;
        }
    }
}

//total slots = max 32 bit numbers(0xffffffff)/0xffff
void find_missing_when_10MB_available(const char *filename){
    string line;
    int radix = 0xffff;
    int slots = 0xffffffff/radix;
    int counter[slots]={0};
    ifstream myfile(filename);
    //load integers in the slots
    while(getline(myfile,line))
    {
        int n = atoi(line.c_str());
        counter[n>>16]++;
    }

    for(int i=0; i<slots; i++)
    {
	if(counter[i]!=0xffff){
          find_missing_number(filename, i);
	}
    }
}
