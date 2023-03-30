    //Function to find position of first set bit in the given number.
//O(log(n)) time and O(1) space
    unsigned int getFirstSetBitA(int n)
    {
        int i=0;
        unsigned int k=1;
        int count=0;
        do{
            k=1<<count;
            if(k&n){
                return count+1;
            }
            count++;
        }while(k<n);
        return 0;
    }

//O(log(n)) time and O(1) space
    unsigned int getFirstSetBitB(int n)
    {
        //cout<<std::bitset<8>(n)<<endl;
        //cout<<std::bitset<8>(-n)<<endl;
        //cout<<std::bitset<8>(~(n-1))<<endl;
        //cout<<std::bitset<8>(n & ~(n-1))<<endl;
        //~(n-1) same as -n
        return log2(n & ~(n-1))+1;
    }
