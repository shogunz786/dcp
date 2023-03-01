#O(nlog(n)) time and O(1) space
def nonConstructibleChange(coins):
    coins.sort()
    currChange = 0
    for cc in coins:
        if cc > currChange+1:
            return currChange+1;
        currChange+=cc
    return currChange+1
