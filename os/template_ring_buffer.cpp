template<typename T>
class RingBuffer{
  private:
    T *buffer;
    int size;
    pthread_mutex_t *mtx;
    int read_loc;
    int write_loc;
    int count;
  public:
   RingBuffer(int size);
   ~RingBuffer();
   bool put(T item);
   T* get();
   void reset(); 
};

RingBuffer::RingBuffer(int sz);
{
   buffer = new T[size];
   memset(buffer, 0, sizeof(T)*size);
   size = sz;
   mtx = new pthread_mutex_t;
   pthread_mutex_init(mtx, NULL);
   count = read_loc = write_loc = 0;
}

RingBuffer::~RingBuffer(){
 delete[] buffer;
 delete mtx;  
}

void RingBuffer::reset()
{
  pthread_mutex_lock(mtx);
  count = read_loc=write_loc=0;
  memset(buffer, 0, sizeof(T)*size);
  pthread_mutex_unlock(mtx);
}

bool RingBuffer::put(T Item)
{
  pthread_mutex_lock(mtx);
  if(isFull()){
    pthread_mutex_unlock(mtx);  
    return false;
  }
  buffer[write_loc++]=Item;
  count++;
  if(write_loc==size)
   write_loc = 0;  
  pthread_mutex_unlock(mtx);
  return true;  
}

T * RingBuffer::get(T Item)
{
  T item;
  pthread_mutex_lock(mtx);
  if(isEmpty()){
    pthread_mutex_unlock(mtx);  
    return NULL;
  }
  item = buffer[read_loc++];
  if(read_loc==size)
   read_loc = 0;
  count--;
  pthread_mutex_unlock(mtx);
  return &item;  
}

bool RingBuffer:isEmpty()
{
  return (count == 0); 
}

bool RingBuffer::IsFull()
{
  return  (count==size)
}