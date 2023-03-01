//O(n) time and O(n) space
def sortedSquaredArray(array):
    l=0
    r=len(array)-1
    res = list()
    while l<=r:
        p_l = array[l]*array[l]
        p_r = array[r]*array[r]
        if p_l < p_r:
            res.append(p_l)
            l+=1
        else:
            res.append(p_r)
            r-=1
    return res
