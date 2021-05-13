/* A, B, C are polls, A is start point and dest is C
 * for three disks
 * Move disk 3 from rod F to rod T aux A count 3
        Move disk 2 from rod F to rod A  aux T count 2
            return Move disk 1 from rod F to rod T aux A count 1
        Move disk 2 from rod F to rod A  aux T count 2
            return Move disk 1 from rod T to rod A aux F count 1
    Move disk 3 from rod F to rod T  aux A count 3
        Move disk 2 from rod A to rod T  aux F count 2
            return Move disk 1 from rod A to rod F aux T count 1
        Move disk 2 from rod A to rod T  aux F count 2
            return Move disk 1 from rod F to rod T aux A count 1
 */
void towersofhanoi(int disks, char from, char to, char aux)
{
  if(n==1)
    return;
  cout<<"move disk from "<<from<<" to "<< aux<<endl;
  towersofhanoi(n-1, from, aux, to);
  towersofhanoi(n-1, aux , from, to);
}
