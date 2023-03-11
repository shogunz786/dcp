
/*
This is just a simple shared plaintext pad, with no execution capabilities.

When you know what language you'd like to use for your interview,
simply choose it from the dots menu on the tab, or add a new language
tab using the Languages button on the left.

You can also change the default language your pads are created with
in your account settings: https://app.coderpad.io/settings


Assume there is a piece of hardware (say microcontroller) feeding in 12 bit values (represented as 4 byte integers with LSB 12 bits valid). Return an array of 8 bit unsigned values.
0x123, 0x456 -> 0x12, 0x34, 0x56
*/
#include<iostream>
#include<vector>

using namespace std;

#define VALID_BITS 0xfff
vector<uint8_t> find_valid_bits(uint32_t *array, int len){
	vector<uint8_t> res;
	int i=0;
	uint32_t val=0;
	uint32_t first =0;
	uint32_t second = 0;
	int rem = 0;
	while(i<len){
		val = array[i];
		if(rem){
			first = (second << 4)|((val & 0xf00) >> 8);
			second = (val & 0xff);
			rem = 0;
		}else{
			first = (val & VALID_BITS)>>4; 
			second = val & 0xf;
			rem = 1;
		}
		res.push_back(first);
		if(rem==0){
			res.push_back(second);
		}
		i++;
	}
	if(rem){
		res.push_back(second);
	}
	return res;
}

int main(int argc, char *argv[]){
    uint32_t array[] = {0x123, 0x456, 0x789};
    vector<uint8_t> res = find_valid_bits(array, 3);
    for(auto a: res){
      cout<<"0x"<<hex<<(uint32_t)a<<endl;
    } 
}
