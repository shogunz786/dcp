package main

const HOME_TEAM_WINNER = 1

//O(n) time and O(k) space - number of players is k
func TournamentWinner(competitions [][]string, results []int) string {
    mp := map[string]int{}
    player := ""
    maxplayer := ""
    maxwinner := 0
    for i, players := range competitions {
        player = players[1]     
        if results[i] == HOME_TEAM_WINNER {
           player = players[0]
        }
        if _, found := mp[player]; found {
            mp[player]+=1
        }else{
            mp[player]=1
        }
        if mp[player] > maxwinner {
            maxwinner = mp[player]
            maxplayer = player
        }
    }
	return maxplayer
}
