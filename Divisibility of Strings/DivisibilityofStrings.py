def DivisibilityofStrings(s1,s2):
    def finSmallestString(s1): # find the smallest string  
        lenS=len(s1)
        for i in range(lenS):
            if lenS%(i+1)==0 and s1==s1[:i+1]*(lenS//(i+1)):
                return len(s1[:i+1])
    if len(s2)<len(s1): # make s1 always the smallest string
        s1,s2=s2,s1
    if len(s2)%len(s1)!=0: # quick check if s2 made of s1
        return -1
    else:
        if s2!=s1*(len(s2)//len(s1)): # check if s2 = s1* times
            return -1
        else:
            return finSmallestString(s1)


s1="lrbb"
s2="lrbblrbb"
print(DivisibilityofStrings(s1,s2))
    