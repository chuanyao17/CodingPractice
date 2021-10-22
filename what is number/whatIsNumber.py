import re
def checkSMS(str):
    pattern="\+?[1-9]\d{1,14}"
    return True if re.fullmatch(pattern,str) else False

def whatIsNumber(str):
    if " " in str:
        print("contain space")
        return "INVALID_ADDRESS"
    if checkSMS(str):
        return "SMS"
    channel=str.split(":")
    print(channel)
    if len(channel)!=2:
        return "INVALID_ADDRESS"
    if channel[0].lower()=="whatsapp" and checkSMS(channel[1]):
        return "WHATSAPP"
    elif channel[0]=="messenger" and checkSMS(channel[1]):
        return "MESSENGER"
    elif channel[0].lower()=="wechat":
        print(channel[1])
        if re.fullmatch("[a-zA-Z0-9@._+-]{1,248}",channel[1]):
            return "WECHAT"
    return "INVALID_ADDRESS"
        

# testcase=["+15555","155555555555555","+1555555555555555","whatsapp:+155555555555555","whatsapp:+1555555555555552","wechaT:123:13","Wechat:fjis_@13","+0123","01230","mESSENGER:+155555555555555","messenger:123","fb","","whatsapp:"]
testcase=["Wechat:fjis_@13"]
for test in testcase:
    print(test,"= ",whatIsNumber(test))
