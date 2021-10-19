from typing import Counter


def check_anagram(s1,s2):
    count1=Counter(s1)
    count2=Counter(s2)
    return True if count1==count2 else False


def countSentence(words, sentences):
    wordSet={}
    countSet={}
    numSet=0
    res=1
    for i in range(len(words)):
        for j in range(i+1,len(words)):
            # print(check_anagram(words[i],words[j]))
            if check_anagram(words[i],words[j]):
                if words[i] in wordSet:
                    wordSet[words[j]]=wordSet[words[i]]
                    countSet[wordSet[words[j]]]+=1
                else:
                    wordSet[words[i]]=numSet
                    wordSet[words[j]]=numSet
                    countSet[wordSet[words[i]]]=2
                    numSet+=1
    for w in sentences.split(" "):
        # print(countSet[wordSet[w]])
        if w in wordSet:
            res*=countSet[wordSet[w]]
    return res
testcase=[(['listen','silent','it','is'],'listen it is silent'),(["ab","ba"],"ab ab")]
# testcase=[(['a','b'],'a')]
for words, sentences in testcase:
    # print(words)
    # print(sentences)
    print(countSentence(words,sentences))
