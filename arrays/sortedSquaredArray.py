#O(nlg(n)) time and O(n) space
def sortedSquaredArray(array):
    res = [0 for _ in array]
    i = 0
    for i in range(len(array)):
        res[i]=array[i]*array[i]
        i+=1
    res.sort()
    return res

#O(n) time and O(n) space
def sortedSquaredArray(array):
    l=0
    r=len(array)-1
    res = [0 for _ in array]
    
    for idx in reversed(range(len(array))):
        l_val = array[l]
        r_val = array[r]
        if abs(l_val) > abs(r_val):
            res[idx]=l_val*l_val
            l+=1
        else:
            res[idx]=r_val*r_val
            r-=1
    return res
