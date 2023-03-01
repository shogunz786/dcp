//O(n) time and O(k) space - k num of teams
def tournamentWinner(competitions, results):
    i = 0
    mp = dict()
    winner=0
    max_score=0
    for tm1, tm2 in competitions:
       result = results[i]
       i+=1
       win=tm1
       loss=tm2
       if result == 0:
           win=tm2
           loss=tm1
       if win not in mp:
           mp[win]=0
       mp[win]+=1
       if mp[win] > max_score:
          max_score = mp[win]
          winner = win

    return winner
