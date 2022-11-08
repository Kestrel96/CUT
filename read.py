import time
import re
import os




while(1):
    stat =open("/proc/stat","r")
    for l in stat:
        if(re.search("cpu",l)):
            print(l)
    
    time.sleep(1)
    stat.close()
    os.system('clear')