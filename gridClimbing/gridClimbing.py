def gridClimbing(twoDarr):
    nodeCount=[]
    res=0
    for row in twoDarr:
        # print(row)
        count=0
        for col in row:
            if col==1:
                count+=1
        # print(count)
        if count!=0:
            nodeCount.append(count)
    prev=0
    for i in nodeCount:
        if prev!=0:
            res+=prev*i
        prev=i
    return res
# twoDarr=[[1,0,1,1],[0,1,1,0],[0,0,0,0],[1,0,0,0]]
# twoDarr=[[0,0,0],[1,1,1],[0,1,0],[0,0,0],[1,1,0]]
twoDarr=[[1],[0],[1]]
print(gridClimbing(twoDarr))

