This is just a simple shared plaintext pad, with no execution capabilities.

When you know what language you'd like to use for your interview,
simply choose it from the dots menu on the tab, or add a new language
tab using the Languages button on the left.

You can also change the default language your pads are created with
in your account settings: https://app.coderpad.io/settings

Hello!
Assume there is a piece of hardware (say microcontroller) feeding in 12 bit values (represented as 4 byte integers with LSB 12 bits valid). Return an array of 8 bit unsigned values.
0x123, 0x456 -> 0x12, 0x34, 0x56
0xffff
0x0fff
0x0 0xff0(0xff) 0xf

#define VALID_BITS 0xff

vector<uint8_t> find_valid_bits(uint32_t *array, int len){
  vector<uint8_t> res;
  uint8_t val = (uint8_t *)array;
  int i=0;
  while(i<len*4){
       if(i%4 == 0){
          i++;
          continue;
       }
       uint32_t val = *((uint8_t*)(array+i));
       res.push_back((val&(1<<8))>>8);//first
       res.push_back(val&(VALID_BITS);//second
       i++;
       if(i!=len && i+4>len){
        res.push_back(val&(VALID_BITS);//second
        break;
       }
  }
  return res;
}
