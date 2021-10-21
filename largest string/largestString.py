def findLargestString(str,k):
    def findNext(start):
        for i in range(start-1,-1,-1):
            if mapp[i]>0:
                return i
        return -1
    mapp=[0 for i in range(26)]
    ans=[]
    for i in str:
        char=ord(i)-ord('a')
        mapp[char]+=1
    # print(mapp)
    for i in range(25,-1,-1):
        count=0
        while mapp[i]>0:
            if count==k:
                nextI=findNext(i)
                if nextI==-1:
                    return ''.join(ans)
                
                ans.append(chr(nextI+ord('a')))
                mapp[nextI]-=1
                count=0
            ans.append(chr(i+ord('a')))
            mapp[i]-=1
            count+=1
    return ''.join(ans)
print(findLargestString('ccbbb',2))