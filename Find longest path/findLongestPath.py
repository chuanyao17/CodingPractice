# you can write to stdout for debugging purposes, e.g.
# print("this is a debug message")
from collections import defaultdict
def solution(S, A):
    # write your code in Python 3.6
    build=defaultdict(list)
    ans=0
    nodeNum=0
    for i in range(len(S)):
        build[A[i]].append((nodeNum,S[i]))
        nodeNum+=1
    print(build)
    parentLetter='#'
    def calLength(node,parentLetter):
        nonlocal ans
        print("node=",node)
        if len(build[node])==0:
            return 0
        maxLength_1,maxLength_2=0,0
        for child, letter in build[node]:
            print("letter=",letter,"child=",child,"parent=",parentLetter)
            if letter==parentLetter:
                continue
            parentLength=calLength(child,letter)+1
            if parentLength>maxLength_1:
                maxLength_1,maxLength_2=parentLength,maxLength_2
            elif parentLength>maxLength_2:
                maxLength_2=parentLength
            print(maxLength_1,maxLength_2)
        ans=max(ans,maxLength_1+maxLength_2)
        return maxLength_1
    calLength(-1,parentLetter)
    return ans
