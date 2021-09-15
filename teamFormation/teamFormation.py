#Team Formation 2 FC Codelona Is Trying To Assemble A Team From A Roster Of Available Players
from itertools import combinations, permutations
# Get all permutations of [1, 2, 3]
# perm = permutations([1, 2, 3])
# print(len(list(perm)))
# Print the obtained permutations
# for i in list(perm):
#     print (i)

def countTeam(skills, minPlayers, minLevel, maxLevel):
    matchteam=[]
    res=0
    for i in skills:
        if i>= minLevel and i<= maxLevel:
            matchteam.append(i)
    for i in range(minPlayers, len(matchteam)+1):
        combination=len(list(combinations(matchteam,i)))
        res+=combination
    
    return res

testCase=[([12,4,6,13,5,10],1,5,7),([4,8,5,6],2,5,7),([4,8,5,6],2,7,8)]
print([countTeam(skills, minPlayers, minLevel, maxLevel) for skills, minPlayers, minLevel, maxLevel in testCase])