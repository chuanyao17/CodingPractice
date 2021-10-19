


# print(arr[0:1])


    # return lists
# sub_lists(arr)

def evenSubarray(arr, k):
    res=set()
    left,right,odd_num=0,0,0
    def sub_lists (l):

        for i in range(len(l) + 1):
            for j in range(i):
                res.add(''.join(map(str,l[j:i])))
    while right< len(arr):
        if arr[right]%2==1:
            odd_num+=1
        while odd_num>k and left<right:
            if arr[left]%2==1:
                odd_num-=1
            left+=1
        sub_lists(arr[left:right+1])
        right+=1
    return res

testCase=[([1,2,3,4],1),([1, 3, 9, 5],2),([3, 2, 3, 2],1),([2, 2, 5, 6, 9, 2, 11, 9, 2, 11, 12],1)]
for arr, k in testCase:
    print(evenSubarray(arr,k))