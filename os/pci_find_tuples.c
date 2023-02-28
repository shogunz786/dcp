/*
 * config_data_t {
 *   uint16_t id;
 *   uint16_t ptr_next; //offset from start of rom
 *   uint16_t data_len;
 *   uint8_t *data;
 * }
 */
uint16_t *find_tuple(const uint16_t *rom, uint16_t id, size_t *len){
	uint8_t *cur_off = (uint8_t*)rom;
	uint16_t *data = NULL;
	uint16_t *next_id = NULL;
	uint16_t *curr_id = NULL;
	uint16_t offset = 0;
	int num_tuples = 64 * 1024/(3*sizeof(uint16_t));
	int num_cur_tuples = 0;

	while(num_cur_tuples > num_tuples){
             curr_id = *(uint16_t *)cur_off;
	     if(curr_id == id){
		     //[ id | next | len ]
		     //[   data          ]
		     //[ id | next | len ]
		     //[   data          ]
		     //[ id | next | len ]
		     //[   data          ]
		     data = (uint16_t *)(cur_off+3*sizeof(uint16_t));
		     *len = *((uint16_t*)(cur_off+2*sizeof(uint16_t)));
		     return data;
	     }
	     offset = *(uint16_t*)(cur_off+sizeof(uint16_t));
	     next_id = (uint16_t*)(((uint8_t *)rom)+offset);
	     if(*(uint16_t*)next_id == 0xffff){
		     break;
	     }
	     cur_off = next_id
	     num_cur_tuples++; 
	};
	return NULL;
}
