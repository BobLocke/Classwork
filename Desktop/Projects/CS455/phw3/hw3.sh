#!/bin/bash

# Ryan "Bob" Dean

#2.1. What is the average packet size, across all traffic in the trace? Describe how you computed this number.

#cut -d "," -f5,6 --output-delimiter " " flow.csv | awk '{ bytes +=$2; packets+=$1}; END {print bytes/packets}'

#This line computes the average packet size to be 768.181 bytes.

#2.2. Plot the Cumulative Probability Distribution (CDF) of flow durations (i.e., the finish time minus the start time) and of flow sizes(i.e., number of bytes, and number of packets). 
#First plot each graph with a linear scale on each axis, and then a second time with a logarithmic scale on each axis. 
#What are the main features of the graphs? Why is it useful to plot on a logarithmic scale?


#tail -n +2 flow.csv | cut -d "," -f 6,7,8 --output-delimiter " " | awk '{timeframe = $3 - $2; bytes = $1; print timeframe, bytes}' | sort -nr -k 1,1 | tr " " , > number2.csv 

#cut -d "," -f 1 --output-delimiter " " number2.csv | sort | uniq -c | sort -n -k 2,2 > timecounts.csv
#sed -i 's/^ *//' timecounts.csv

#cut -d "," -f 2 --output-delimiter " " number2.csv | sort | uniq -c | sort -n -k 2,2 > bytecounts.csv
#sed -i 's/^ *//' bytecounts.csv


#2.3 

#tail -n +2 flow.csv | cut -d "," -f 6,16 --output-delimiter " " | sort | awk '{bytes += $1; sender = $2; traffic[sender] += $1} END {for (var in traffic) print var, traffic[var], traffic[var]/bytes}' | sort -nr -k 2,2 | head -n 10 > top10senders.txt
#tail -n +2 flow.csv | cut -d "," -f 6,17 --output-delimiter " " | sort | awk '{bytes += $1; sender = $2; traffic[sender] += $1} END {for (var in traffic) print var, traffic[var], traffic[var]/bytes}' | sort -nr -k 2,2 | head -n 10 > top10recievers.txt

# Answers are output to files Top10Senders and Top10Recievers

#2.4

#tail -n +2 flow.csv | cut -d "," -f 6,11,21 --output-delimiter "." | awk 'BEGIN{FS=".";} {prefix = int(($2*2^24+$3*2^16+$4*2^8+$5)/2^(32-$6)); Ip[prefix] +=1; bytes[prefix] += $1} END {for (var in Ip) print var, Ip[var], bytes[var]}' | sort -nr -k 2,2 | tr " " , > prefix.csv
#awk 'BEGIN{total = 3375; FS=","} {bytes += $3; pop += 1; if (pop < total/1000) topbytes += $3;} END {print topbytes/bytes}' prefix.csv
#awk 'BEGIN{total = 3375; FS=","} {bytes += $3; pop += 1; if (pop < total/100) topbytes += $3;} END {print topbytes/bytes}' prefix.csv
#awk 'BEGIN{total = 3375; FS=","} {bytes += $3; pop += 1; if (pop < total/10) topbytes += $3;} END {print topbytes/bytes}' prefix.csv

# Top 0.1% = 47.6% of total data
# Top 1% = 59.1% of total data
# Top 10% = 91.3% of total data

#2.5 

#tail -n +2 flow.csv | cut -d "," -f 6,12,22 --output-delimiter "." | awk 'BEGIN{FS=".";} {prefix = int(($2*2^24+$3*2^16+$4*2^8+$5)/2^(32-$6)); Ip[prefix] +=1; bytes[prefix] += $1} END {for (var in Ip) print var, Ip[var], bytes[var]}' | sort -nr -k 2,2 | tr " " , > recvprefix.csv
#awk 'BEGIN{FS=","} {bytes += $3; packets += $2; if ($1 == (134 * 2^8 + 121)){wsubytes += $3; wsuhits += $2}} END {print "byte ratio: " wsubytes/bytes, " traffic ratio: " wsuhits/packets}' prefix.csv
#awk 'BEGIN{FS=","} {bytes += $3; packets += $2; if ($1 == (134 * 2^8 + 121)){wsubytes += $3; wsuhits += $2}} END {print "byte ratio: " wsubytes/bytes, " traffic ratio: " wsuhits/packets}' recvprefix.csv

#  byte ratio: 0.000159167%  traffic ratio: 0.0121319%
#  byte ratio: 0.0187531%  traffic ratio: 0.0320464%