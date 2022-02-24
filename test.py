import math
from decimal import Decimal, ROUND_HALF_UP
# print(int("99")+100 )
# print("aA".upper())
# str="123_3"
# if '_' in str:
#     print("yes")

# test=["123:456A"]
# for s in test.split(':'):
#     print(s)
# print(len(test.split(':')))
# if len(test.split(':'))==2:
#     print("right")
# print(''.join(test).lower())


# str_decimal = str(10.025)
# case_d_1 = Decimal(str_decimal).quantize(Decimal(".00"), ROUND_HALF_UP)
# print(f"case decimal 1: {case_d_1}")	# case decimal 1: 0.06
# print(math.floor((case_d_1%1)*100))
# print(math.floor(((72/59)%1)*100))

# test=[("g","a"),("b","z"),("a","c"),("a","a"),("b","a"),("z","a")]
# print(test)
# test.sort(key=lambda x:(x[0]))
# print(test)
# test.sort(key=lambda x:(x[1]))
# print(test)
# test.sort(key=lambda x:(x[0],x[1]))
# print(test)
# test.sort(key=lambda x:(x[1],x[0]))
# print(test)

# def size_of_block(id,totalNum,p):
#     print("first",(((id+1)*totalNum)/p))
#     print("second",((id*totalNum)/p))
#     return (((id+1)*totalNum)//p)-((id*totalNum)//p)

# def size_of_block_yao(id,totalNum,p):
#     task_num=totalNum//p
#     task_remainder_num=totalNum%p
#     start,end=0,0
#     if id<task_remainder_num:
#         start=id*(task_num+1)
#         end=start+task_num
#     else:
#         start=id*task_num+task_remainder_num
#         end=start+task_num-1
#     return end-start+1


# p=3
# for i in range(p):
#     print("id=",i)
#     print(size_of_block(i,11,p))
#     print("///////////////////////////")
#     print("id=",i)
#     print(size_of_block_yao(i,39,p))

# test=[(4,9),(2,12),(3,1)]
# print(min(test)[1])

# test=["cbz","caa","dz","Z"]
# test=sorted(test, key=lambda x:(len(x),x[1]))
# print(test)
# test=sorted(test, key=lambda x:(len(x),x[-1]))
# print(test)

# a=1
# b=3
# print(a//b)

import collections
def twoSum(nums, target):
    """
    :type nums: List[int]
    :type target: int
    :rtype: List[int]
    """
    hash_t=collections.defaultdict(list)
    for i,num in enumerate(nums):
        hash_t[num].append(i)
    output=[]
    for i in range(len(nums)):
        complement=target-nums[i]
        if complement in hash_t:
            for pos in hash_t[complement]:
                if pos!=i:
                    output.append([i,pos])
    print(output)
nums=[2,7,11,15,7]
target=9
twoSum(nums,target)