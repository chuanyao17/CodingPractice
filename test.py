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


str_decimal = str(10.025)
case_d_1 = Decimal(str_decimal).quantize(Decimal(".00"), ROUND_HALF_UP)
print(f"case decimal 1: {case_d_1}")	# case decimal 1: 0.06
print(math.floor((case_d_1%1)*100))
print(math.floor(((72/59)%1)*100))