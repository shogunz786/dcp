class Solution {
public:
    uint32_t reverseBits1(uint32_t n) {
        int res = 0;
        // scan all bits
        for(int i=0; i<sizeof(uint32_t)*8; i++){
            res=res<<1;
            if(n&1){
                res|=1;
            }
            n=n>>1;
        }
        return res;
    }
    uint32_t reverseBits(uint32_t n) {
        n = (n&0xffff0000)>>16 | (n&0x0000ffff)<<16;
        n = (n&0xff00ff00)>>8 | (n&0x00ff00ff)<<8;
        n = (n&0xf0f0f0f0)>>4 | (n&0x0f0f0f0f)<<4;
        n = (n&0xcccccccc)>>2 | (n&0x33333333)<<2;
        n = (n&0xaaaaaaaa)>>1 | (n&0x55555555)<<1;
        return n;
    }

};
