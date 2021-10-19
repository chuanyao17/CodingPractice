# times_arrival=[1,3,3,5,7]
# duration=[2,2,1,2,1]
def count_event(times_arrival,duration):
    event=list(zip(times_arrival,duration))
    # print(event)
    # print(times_arrival)
    event.sort()
    # print(event)
    endding=float('-inf')
    count=0
    for start,dur in event:
        # print("start=",start,"end= ",endding)
        if start >= endding:
            count+=1
            endding=start+dur
            
    print("ans=",count)
testcase=[([1,3,3,5,7],[2, 2, 1, 2, 1]),([1, 3, 3, 4, 7],[2, 2, 2, 3, 1]),([1],[2]),([1, 3, 4, 6],[4, 3, 3, 2]),([1, 5, 5],[10,2,1])]
for times_arrival, duration in testcase:
    count_event(times_arrival,duration)
