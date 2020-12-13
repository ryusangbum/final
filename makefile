timetable : final_pr.o
	gcc -o timetable final_pr.o
final_pr.o: final_pr.c
	gcc -c final_pr.c

