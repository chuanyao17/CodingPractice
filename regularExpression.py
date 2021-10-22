import re
from typing import Pattern

# str_ =  '_.Aa123Bb456Cc789Dd3.14'

# find_alphabet = re.findall(r'[A-Za-z]+', str_)
# print(find_alphabet)  
# # ['Aa', 'Bb', 'Cc', 'Dd']

# find_rational = re.findall(r'[0-9]+\.?[0-9]*', str_)
# print(find_rational)  
# # ['123', '456', '789', '3.14']

grammar="[2-9]{1,4}"
testcase="2"
if re.fullmatch(grammar,testcase):
    print("yes")