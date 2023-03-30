    // Function to check if Kth bit is set or not.
    bool checkKthBit(int n, int k)
    {
        //when result nonzero bit is set
        return ((n & (1<<k))!=0);
    }
