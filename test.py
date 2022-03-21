from asyncio.windows_events import NULL
from collections import deque
from collections import defaultdict
from collections import Counter
from enum import auto
import math
from decimal import Decimal, ROUND_HALF_UP

from unicodedata import name

from prometheus_client import Counter

from enum import Enum
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


# import collections
# def twoSum(nums, target):
#     """
#     :type nums: List[int]
#     :type target: int
#     :rtype: List[int]
#     """
#     hash_t=collections.defaultdict(list)
#     for i,num in enumerate(nums):
#         hash_t[num].append(i)
#     output=[]
#     for i in range(len(nums)):
#         complement=target-nums[i]
#         if complement in hash_t:
#             for pos in hash_t[complement]:
#                 if pos!=i:
#                     output.append([i,pos])
#     print(output)
# nums=[2,7,11,15,7]
# target=9
# twoSum(nums,target)

## Sort a stack using another stack
# input=[34, 3, 31, 98, 92, 23]
# tmpStack=[]

# while input:
#     tmp=input.pop()
#     while tmpStack and tmp<tmpStack[-1]:
#         input.append(tmpStack.pop())
#     tmpStack.append(tmp)
# print(tmpStack)

# def replaceLargest(arr):
#     n=len(arr)
#     if n<1:
#         return None
#     max=arr[-1]
#     arr[-1]=-1
#     for i in range(n-2,-1,-1):
#         tmp=arr[i]
#         arr[i]=max
#         if tmp>max:
#             max=tmp
#     return arr
# input=[5]
# print(replaceLargest(input))

# def replaceSmallestLargest(arr):
#     n=len(arr)
#     if n<1:
#         return None
#     output=[]
    
#     for i in range(n):
#         smallLargerNum=float('inf')
#         curNum=arr[i]
#         for j in range(i+1,n):
#             if arr[j]>curNum and smallLargerNum>arr[j]:
#                 if  smallLargerNum==float('inf'):
#                     smallLargerNum=arr[j]
#                 else:
#                     smallLargerNum=arr[j]
#         if smallLargerNum==float('inf'):
#             output.append(-1)
#         else:
#             output.append(smallLargerNum)
#     return output
# input=[1, 3, 5, 6, 3]
# print(replaceSmallestLargest(input))

# import heapq
# def findKsmallest(arr,k):
#     maxHeap=[]
#     for num in arr:
#         heapq.heappush(maxHeap,(-num[0],num[1]))
#         if len(maxHeap)>k:
#             heapq.heappop(maxHeap)
#     output=deque()
#     while maxHeap:
#         num,name=heapq.heappop(maxHeap)
#         output.appendleft(name)
#     return list(output)

# input=[(1,'a'), (3,'b'), (5,'c'), (6,'d'), (3,'e'),(0,'f'),(-2,'g'),(10,'h')]
# print(findKsmallest(input,4))
    

# def test(arr):
#         num=arr[0]
#         for i in arr:
#             if i!=num:
#                 return True
#         return False
# def findDifferent(arr):
#     if len(arr)==2:
#         return arr[0] if arr[0]>arr[1] else arr[1]
#     n=len(arr)
#     left=arr[:n//2]
#     print(left)
#     right=arr[n//2:]
#     print(right)
#     if test(right):
#         return findDifferent(right)
#     else:
#         return findDifferent(left)
# input=[1,1,1,1,6,1,1,1,1]
# print(findDifferent(input))

# cache=deque()

# def LRU(num,k):
#     if num in cache:
#         cache.remove(num)
#         cache.appendleft(num)
#     else:
#         cache.appendleft(num)
#         if len(cache)>k:
#             cache.pop()
        
        
# LRU(1,4)
# LRU(2,4)
# LRU(3,4)
# LRU(1,4)
# LRU(4,4)
# LRU(5,4)
# print(cache)

# def findFirstUniqueString(arr):
#     print(arr)
#     dict_string={}
#     for s in arr:
#         dict_string[s]=dict_string.get(s,0)+1
#     print(dict_string)
#     for i,s in enumerate(arr):
#         if dict_string[s]==1:
#             return s
#     return None
# input=['aaa','bbbb','a','aaa','aa','bbbb']

# # input = [1,9,9,5,0,8,0,9] 
# print(findFirstUniqueString(input))


##1. Design Amazon locker and package's data models, and design an optimized algorithm to efficiently fit the package into the right locker.
# class Root_locker:
#     def __int__(self):
#         self.locker_availability={'s':['1','2','3'],'m':['4','5'],'l':['6','7']}
#         self.locker_status={} 


# ##判斷大中小，並且取得locker ID
# def give_locker(pkg,locker_availability):
#     if pkg<=5 and len(locker['s']>0):
#         return locker['s'].pop()
#     if pkg<=10 and len(locker['m']>0):
#         return locker['m'].pop() 
#     if pkg<=15 and len(locker['l']>0):
#         return locker['l'].pop()
#     return None

# def update_locker_status(id,locker_status):
#     locker_status[id]=Locker()

# class Locker:
#     def __int__(self):
#         self.pwd=None
#     def generate_pwd(self):
#         self.pwd=rand()
#     def get_pwd(self):
#         return self.pwd
# class s_locker:
#     def s_foo():

# init=Root_locker()   

# while(1)
# {
#     pkg_size=event()
#     get_request(pkg_size)
# } 


# def get_request(pkg_size):
#     id=give_locker(pkg_size)
#     update_locker_status(id)


##1. amazon shopping cart。两个function，把商品加入购物车和计算总价。product有两种discount的方式：1. 10% off  2. buy 3 get 1 free（也可能是buy 5 get 2 free)
# shopping_cart={} ## initial an empty shopping cart for storing the upcomning products
# product_info={
#     "id1":{"name":"toy1","amount":5,"discount_type":"10%,off","price":100},
#     "id2":{"name":"fruit1","amount":50,"discount_type":"buy 3 get 1 free","price":20}
#     } ## products' database

# #for product creating in the cart
# class Product_cart:
#     def __init__(self,product_id,amount):
#         self.product_id=product_id
#         self.amount=amount

# # user selects the product with desired amount 
# # ignore the maxium amount in the database 
# def get_request(product_id,amount):
#     amount=1 ## initial amount=1
#     product_info[product_id]["amount"]-=amount
#     if product_info[product_id]["discount_type"]=="buy 3 get 1 free":
#         product_info[product_id]["amount"]-=amount//3
#         amount+=amount//3
#     add_product=Product_cart(product_id,amount)
#     return add_product


# def update_shopping_cart(product_to_cart):
#     shopping_cart[product_to_cart.product_id]=shopping_cart.get(product_to_cart.product_id,0)+product_to_cart.amount

# def cal_shopping_cart():
#     total=0
#     #adding all the products price * amount in the cart 
#     for product_id,amount in shopping_cart.items():
#         if product_info[product_id]["discount"]=="10%,off":
#             total+=product_info[product_id]["price"] * amount * 0.9
#             continue
#         total+=product_info[product_id]["price"] * amount 
#     return total

# ## processing
# product_to_cart=get_request(product_id,amount)
# update_shopping_cart(product_to_cart)
# cal_shopping_cart()

# ##第二题，ood，设计电视节目管理系统。有station，每个station有schedule。有show，show有开始结束时间。考虑add show有冲突的情况。

# class Schedule:
#     # supposed the schedule will start from 0 to 24
#     def __init__(self):
#         # self.schedule_start=0
#         # self.schedule_end=24
#         self.shows=deque()
    
#     ## add new show to the schedule
#     def add_show(self,new_show):
        
#         # add the new show to the empty schedule or the end of the schedule
#         if not self.shows or new_show.start>self.shows[-1].end:
#             self.shows.append(new_show)
#             return True
#         # add the new show to the beginning of the schedule
#         if new_show.end<self.shows[0].start:
#             self.shows.appendleft(new_show)
#             return True
#         # search and insert the new show
#         for i in range(len(self.shows)-1):
#             if new_show.start>self.shows[i].end and new_show.end<self.shows[i+1].start:
#                 self.shows.insert(i+1,new_show)
#                 return True
#         # time conflict with the schedule
#         return False
    
#     ## for testing only, it's not part of the answer 
#     def print_schedule(self):
#         print(self.shows)
#         for show in self.shows:
#             print(show.name)

# class Show:
#     #supposed the range of the show is between 0 to 24
#     def __init__(self,start,end,name):
#         self.start=start
#         self.end=end
#         self.name=name

# #create an empty schedule
# tmp_schedule=Schedule()
# #create shows
# show1=Show(0,1,'show1')
# show2=Show(3,4,'show2')
# show3=Show(1,2,'show3')
# show4=Show(4,5,'show4')
# show5=Show(6,10,'show5')

# #check schedule
# tmp_schedule.print_schedule()

# #add shows to the tmp_schedule
# input=[show2,show4,show5,show1]
# for show in input:
#     print(tmp_schedule.add_show(show)) #check True or False
#     tmp_schedule.print_schedule()

# q=deque()
# q.append(0)
# q.append(1)
# q.append(2)
# q.append(3)
# print(q)
# q.insert(1,'a')
# print(q)

# class Mammal:
#     eyeNums=2
#     num=0
#     def __init__(self):
#         self.blood="red"
#     def print(self):
#         print("I'm a mammal")
#     def add(self,x):
#         self.num+=x
# class Dog(Mammal):
#     # def __init__(self):
#     #     super().__init__()
#         # self.leg=4
#         # self.blood="blue"
#     # def print(self):
#     #     super().print()
#     num=2
#     def add(self,x):
#       Mammal.num+=x
#     #   super().add(x)
# class Cat(Mammal):
#   pass

# A=Mammal()
# print(A.blood)
# B=Mammal()
# print(B.blood)
# D=Dog()
# print(D.blood)
# C=Cat()
# print(C.blood)
# A.print()
# D.print()
# print(A.eyeNums)
# print(D.eyeNums)
# print(A.num)
# print(D.num)
# print(C.num)
# A.add(1)
# print(A.num)
# print(B.num)
# print(D.num)
# print(C.num)
# D.add(1)
# print(A.num)
# print(B.num)
# print(D.num)
# print(C.num)

# class Animal:
#   def __init__(self, Animal):
#     print(Animal, 'is an animal.')

# class Mammal(Animal):
#   def __init__(self, mammalName='test'):
#     print(mammalName, 'is a warm-blooded animal.')
#     super().__init__(mammalName)
    
# class NonWingedMammal(Mammal):
#   def __init__(self, NonWingedMammal='testNow'):
#     print(NonWingedMammal, "can't fly.")
#     super().__init__(NonWingedMammal)

# class NonMarineMammal(Mammal):
#   def __init__(self, NonMarineMammal):
#     print(NonMarineMammal, "can't swim.")
#     super().__init__()

# class Dog(NonMarineMammal, NonWingedMammal):
#   def __init__(self):
#     print('Dog has 4 legs.')
#     super().__init__('Dog')
    
# d = Dog()
# print('')
# bat = NonMarineMammal('Bat')

# class Test(Enum):
#     a=1
#     b=2
#     c=3
# print(Test.a.name)
# print(Test.b.value)

# dict={}
# dict[Test.a]='rtr'
# print(dict[Test.a])
# print(Test(3))
# tmp=Test['a']
# print(tmp)

# if a==Test.a.name:
#     print("yes")

class Node:
   
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None

def printLeafNodes(root: Node) -> None:
 
    mapp=defaultdict(list)
    result=[]
    def DFS(node,mapp):
        if not node:
            return 0
        leftlevel= DFS(node.left,mapp)
        node.left=None
        rightlevel=DFS(node.right,mapp)
        node.right=None
        maxLevel=max(leftlevel,rightlevel)
        mapp[maxLevel].append(node.val)
        return maxLevel+1
    DFS(root,mapp)
    print(mapp)
    print(root.val)
    

    
root = Node(1)
root.left = Node(2)
root.right = Node(3)
root.left.left = Node(4)
root.right.left = Node(5)
root.right.right = Node(8)
root.right.left.left = Node(6)
root.right.left.right = Node(7)
root.right.right.left = Node(9)
root.right.right.right = Node(10)

printLeafNodes(root)