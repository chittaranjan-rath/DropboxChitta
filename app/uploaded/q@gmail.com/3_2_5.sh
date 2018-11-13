# !/bin/bash
awk -v OFS="\t" 'BEGIN {printf("*** Grade Report for the ABC course *** \n\n");printf("\tName\tMarks\tGrade\n\n");average=0;noofstudent=0;highest=0;lowest=100;} 

NR>1 { if($3+$4+$5>highest) highest = $3+$4+$5; }
NR>1 { if($3+$4+$5<lowest)  lowest= $3+$4+$5; }
NR>1 { average += $3+$4+$5; noofstudent++; }

NR>=2 {a[$1] += ($3+$4+$5); sum+=a[$1] }
{for (i in a){
if(a[i]>=100 && a[i]<=95) 
{ gr="A"}
if(a[i]>=90 && a[i]<95) 
{ gr="A-"}
if(a[i]>=85 && a[i]<90) 
{ gr="B"}
if(a[i]>=80 && a[i]<85) 
{ gr="B-"}
if(a[i]>=75 && a[i]<80) 
{ gr="C"}
if(a[i]>=70 && a[i]<75) 
{ gr="C-"}
if(a[i]>=60 && a[i]<70) 
{ gr="D"}
if(a[i]<60) 
{ gr="F"}
}{print  "\t" i , a[i] , gr }
}
{printf(" \n");}
END { printf("\tTotal Students: %d\n\n",noofstudent);
    printf("\tHighest Score: %d\n\n",highest);
    printf("\tLowest Score: %d\n\n",lowest);
    printf("\tAverage Score: %d\n\n",average/noofstudent);
	print "​ *** End of Grade Report ***​ \n\n"}' marks.txt