#! /usr/bin/python

import sys
import os.path

inst = sys.argv[1]
resDir = "eval/"+inst
opmFilePath = sys.argv[1]+"_opm"
opmFile = open(opmFilePath,'r')
instances = 480.0
allIns=0
optimized = 0
zeroTimeSAT = 0
timesSAT = 0.0

distanceTimeFile = open("eval_res/"+inst+'_dst', 'w')
############################################
# Checks optimal solutions
############################################
for i in opmFile:
    temp = i.strip()
    items = temp.split()
    optimal = int(items[2])
    name = inst+items[0]+"_"+items[1]
    opmInstance = "eval/"+inst+"/"+name+".sm_res_opt"
    if not os.path.isfile(opmInstance):
        continue
    allIns += 1
    instanceOpmFile = open(opmInstance, 'r')
    l = instanceOpmFile.readline()
    if len(l) == 0:
        continue
    if (int(l.strip()) == optimal):
        optimized += 1
    instanceOpmFile.close()
    makespanInstance = "eval/"+inst+"/"+name+".sm_res_mks"
    makespanFile = open(makespanInstance, 'r')
    for m in makespanFile:
        timesSAT += 1
        temp = m.strip()
        itemsMakespan = temp.split()
        actMakespan = int(itemsMakespan[0])
        satTime = itemsMakespan[1]
        if int(satTime) is 0:
            zeroTimeSAT += 1
            continue
        distance = abs(optimal - actMakespan)
        distanceTimeFile.write(str(distance)+" "+satTime+'\n')
    makespanFile.close()
distanceTimeFile.close()
opmFile.close()
satTimeFile = open("eval_res/"+inst+'_zro', 'w')
satTimeFile.write(str(zeroTimeSAT/timesSAT)+'\n')
satTimeFile.close()

############################################
# Compute times
############################################
instancesReal = 0.0
timesFile = open("eval/"+inst+"/done",'r')
time0 = 0
time1 = 0
time5 = 0
time10 = 0
time50 = 0
time100 = 0
time500 = 0
for i in timesFile:
    time = int(i)
    instancesReal += 1
    if time >= 0 and time < 1:
        time0 += 1
    elif time >= 1 and time < 2:
        time1 += 1
    elif time >= 2 and time < 5:
        time5 += 1
    elif time >= 5 and time < 10:
        time10 += 1
    elif time >= 10 and time < 50:
        time50 += 1
    elif time >= 50 and time < 100:
        time100 += 1
    elif time >= 100:
        time500 += 1
timesFile.close()

opmSucFile = open("eval_res/"+inst+"_opm",'w')
opmSucFile.write(str(optimized/instances))
opmSucFile.close()

timeFile = open("eval_res/"+inst+"_time",'w')
timeFile.write("0 "+str(time0/instancesReal)+'\n')
timeFile.write("1 "+str(time1/instancesReal)+'\n')
timeFile.write("5 "+str(time5/instancesReal)+'\n')
timeFile.write("10 "+str(time10/instancesReal)+'\n')
timeFile.write("50 "+str(time50/instancesReal)+'\n')
timeFile.write("100 "+str(time100/instancesReal)+'\n')
timeFile.write("500 "+str(time500/instancesReal)+'\n')
timeFile.close()
