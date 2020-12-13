#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct timetable{
	int days;
	int st_time;
	int fn_time;
	char subject[30];
	struct timetable* next;
}timetable_t;

char *days_name[] = {"월","화","수","목","금","토","일"};
void check_timetable(timetable_t* list_head,int (*func)(int,int));
timetable_t* add_timetable(timetable_t* list_head,timetable_t*(*func)(int,int,int,timetable_t*));
timetable_t* del_timetable(timetable_t* list_head, timetable_t*(*func)(char*,timetable_t*));
timetable_t* clr_timetable(timetable_t* list_head);
void search_timetable(timetable_t* list_head,int (*func)(int,int));
void check_freetime(timetable_t* list_head);
void save_file(timetable_t* list_head);
timetable_t* load_file(timetable_t* list_head);
timetable_t* search(char* subject, timetable_t* list_head);
timetable_t* check_time(int days, int st_time, int fn_time, timetable_t* list_head);
int timesize(int x, int y);

void main()
{
	int choice;
	timetable_t* list_head=NULL;
	list_head = load_file(list_head);
        printf("--------------------------------------\n");
        printf("<시간표 관리 프로그램을 시작합니다.>\n");
	while(choice != 8)
	{
		printf("--------------------------------------\n");
		printf("1. 시간표 확인\n");
		printf("2. 시간표 추가\n");
		printf("3. 시간표 삭제\n");
		printf("4. 시간표 초기화\n");
		printf("5. 검색\n");
		printf("6. 공강시간 확인\n");
		printf("7. 시간표 저장\n");
		printf("8. 나가기\n");
		printf("--------------------------------------\n");
		printf("기능 선택: ");
		scanf("%d", &choice);
		switch(choice){
			case 1:
				check_timetable(list_head,timesize);
				break;
     			case 2:
				list_head = add_timetable(list_head,check_time);
				break;
			case 3:
				list_head = del_timetable(list_head,search);
				break;
			case 4:
				list_head = clr_timetable(list_head);
				printf("시간표가 초기화되었습니다.\n");
				break;
			case 5:
				search_timetable(list_head,timesize);
				break;
			case 6:
				check_freetime(list_head);
				break;
			case 7:
				save_file(list_head);
				break;
			case 8:
				printf("시간표 관리 프로그램을 종료합니다\n");
				break;
			default:
				printf("잘못된 번호를 입력하였습니다.\n"); 
		   }
	}
}

void check_timetable(timetable_t* list_head,int (*func)(int,int))
{
	int check;
	timetable_t *tmp = list_head;
	printf("\n=======================================\n");
	while(list_head != NULL){
		if(func(list_head->st_time,list_head->fn_time)==0)
			printf("%s %d교시 %s\n",days_name[list_head->days-1],list_head->st_time,list_head->subject);
		else
			printf("%s %d교시~ %d교시 %s\n",days_name[list_head->days-1],list_head->st_time,list_head->fn_time,list_head->subject);
		list_head = list_head -> next;
	}
	printf("=======================================\n");
	list_head = tmp;
	printf(" ");
	for(int i=0;i<7;i++)
	{
		printf("  %s",days_name[i]);
	}
	printf("\n");
	for(int j=0;j<9;j++)
	{
		printf("%d  ",j+1);
		for(int k=0;k<7;k++)
		{
			list_head = tmp;
			check = 0;
			while(list_head != NULL)
			{	
				if(list_head->days==k+1)
				{
					if(list_head->st_time<=j+1&&j+1<=list_head->fn_time)
					{
						printf("■   ");
						check = 1;
						break;
					}
				}
				list_head = list_head -> next;
			}
			if(check==0)
				printf("□   ");
		}
		printf("\n");
	}
        printf("=======================================\n");
}	
 
timetable_t* add_timetable(timetable_t* list_head,timetable_t*(*func)(int,int,int,timetable_t*))
{
	timetable_t* new_node;
	timetable_t* check;
	timetable_t* next_p = list_head;
	timetable_t* prev_p = NULL;
	new_node = (timetable_t*)malloc(sizeof(timetable_t));
	printf("\n1.월 2.화 3.수 4.목 5.금 6.토 7.일\n");
	printf("요일을 입력하시오(1~7 숫자): ");
	scanf("%d",&new_node->days);
	if(new_node->days<1||new_node->days>7)
	{
		printf("잘못 입력 하셨습니다.\n");
		return list_head;
	}
	printf("수업이 시작하는 교시를 입력하시오: ");
	scanf("%d",&new_node->st_time);
	printf("수업이 종료하는 교시를 입력하시오: ");
        scanf("%d",&new_node->fn_time);
	if(new_node->st_time > new_node->fn_time)
	{
		printf("잘못 입력 하셔습니다.\n");
		return list_head;
	}
        check = func(new_node->days,new_node->st_time,new_node->fn_time,list_head);
	if(check)
	{
		printf("이미 그 시간대에 수업이 있습니다.\n");
		return list_head;
	}
	printf("과목을 입력하시오: ");
        scanf("%s", new_node->subject);
	while(next_p){
		if(next_p->days > new_node->days)
			break;
		else if(next_p->days==new_node->days)
		{
			if(next_p->st_time > new_node->st_time)
				break;
		}
		prev_p = next_p;
		next_p = next_p->next;
	}
	new_node -> next = next_p;
	if(prev_p)
		prev_p->next = new_node;
	else
		list_head = new_node;
	
	printf("추가 완료 되었습니다.\n");
	return list_head;
}

timetable_t* del_timetable(timetable_t* list_head, timetable_t*(*func)(char*,timetable_t*))
{
	timetable_t* check;
	timetable_t* remove;
	timetable_t* tmp;
	timetable_t* inu = list_head;
	char subject[30];
	printf("\n삭제할 과목 이름: ");
	scanf("%s",subject);
	check = func(subject,list_head);
	if(check==NULL)
	{
		printf("시간표에 해당 과목이 없습니다.\n");
		return list_head;
	}
	else if(strcmp(list_head->subject,subject)==0)
	{
		remove = list_head;
		list_head = list_head -> next;
		free(remove);
		printf("삭제 완료되었습니다.\n");
		return list_head;
	}
	else
	{
		while(list_head!=NULL)
		{
			if(strcmp(list_head->subject,subject)==0)
			{
				tmp->next = list_head->next;
				free(list_head);
				printf("시간표에서 삭제되었습니다.\n");
				return inu;
			}
			tmp = list_head;
			list_head = list_head->next;
		}
	}
}

timetable_t* clr_timetable(timetable_t* list_head)
{
	if(list_head == NULL)
		return NULL;
	timetable_t* remove = list_head;
	timetable_t* tmp = remove;
	list_head = list_head -> next;
	while(list_head!= NULL)
	{
		tmp->next = list_head->next;
		free(list_head);
		list_head = tmp->next;
	}
	free(remove);
	return NULL;
}

void search_timetable(timetable_t* list_head,int (*func)(int, int))
{
	timetable_t* tmp;
	int choice;
	int date;
	int start;
	char subject[30];
re:
	printf("\n어떤 내용을 검색할지 선택하세요(1.시간 2.과목): ");
	scanf("%d",&choice);
	if(choice==1)
	{
		printf("요일을 입력하세요(1.월 2.화 3.수 4.목 5.금 6.토 7.일): ");
		scanf("%d",&date);
		printf("교시를 입력하세요: ");
		scanf("%d",&start);
		while(list_head != NULL)
		{
			if(list_head->days==date)
			{
				if(list_head->st_time<=start && start<=list_head->fn_time)
				{
					printf("해당 시간에 %s 과목이 있습니다.\n",list_head->subject);
					return;
				}
			}
			list_head = list_head -> next;
		}
		printf("해당 시간에 과목이 없습니다.\n");
		return;
	}
	else if(choice==2)
	{
		printf("과목을 입력하세요: ");
		scanf("%s",subject);
		while(list_head != NULL)
		{
			if(strcmp(list_head->subject,subject)==0)
			{
				if(timesize(list_head->st_time,list_head->fn_time)==0)
				{
					printf("해당 과목이 %s요일 %d교시에 있습니다.\n",days_name[list_head->days-1],list_head->st_time);
					return;
				}
				else
				{
					printf("해당과목이 %s요일 %d ~ %d교시에 있습니다.\n",days_name[list_head->days-1],list_head->st_time,list_head->fn_time);
					return;
				}
			}
			list_head = list_head -> next;
		}
		printf("해당 과목이 시간표에 없습니다.\n");
		return;
	}
	else
	{
		printf("번호를 잘못입력하였습니다.\n");
		goto re;
	}	
}
void check_freetime(timetable_t* list_head)
{
	int date;
	int check;
	timetable_t* tmp = list_head;
	printf("요일을 선택하세요(1.월 2.화 3.수 4.목 5.금 6.토 7.일): ");
	scanf("%d",&date);
	printf("-%s요일의 공강시간-\n",days_name[date-1]);
	for(int j=0;j<9;j++)
       	{	 
		list_head = tmp;
                check = 0;
                while(list_head != NULL)
                {
                	if(list_head->days==date)
                        {
                        	if(list_head->st_time<=j+1 && j+1<=list_head->fn_time)
                                {
                                	check = 1;
                                       	break;
                                }
                        }
                      	list_head = list_head -> next;
               	}
               	if(check==0)
                	printf("%d교시 ",j+1);
        }
        printf("\n");
}

void save_file(timetable_t* list_head)
{
	FILE* fp = fopen("test.txt","w");
	if(fp==NULL)
	{
		printf("파일을 여는데 실패하였습니다.\n");
		return;
	}
	while(list_head != NULL)
	{
		fprintf(fp,"%d %d %d %s\n",list_head->days,list_head->st_time,list_head->fn_time,list_head->subject);
		list_head = list_head -> next;
	}
	printf("파일이 저장되었습니다.\n");
	fclose(fp);
}

timetable_t* load_file(timetable_t *list_head)
{
	timetable_t* new_node=NULL;
	FILE *fp = NULL;
	fp = fopen("test.txt","r+");
	if(fp == NULL){
		printf("파일을 여는데 실패하였습니다.\n");
		return list_head;
	}
	while(!feof(fp))
	{
        	new_node = (timetable_t*)malloc(sizeof(timetable_t));
		fscanf(fp,"%d %d %d %s\n",&new_node->days,&new_node->st_time,&new_node->fn_time,new_node->subject);
		if (list_head == NULL)
			list_head  = new_node;
		else 
		{
			timetable_t* tmp = list_head;
		       	while(tmp->next != NULL)
				tmp = tmp -> next;
			tmp -> next = new_node;
		}

	}
	fclose(fp);
	return list_head;
}

timetable_t* search(char *subject, timetable_t* list_head)
{
	timetable_t* tmp = list_head;
	while(tmp){
		if(!strcmp(tmp->subject, subject)){
			return tmp;
		}
		tmp = tmp->next;
	}
	return tmp;
}

timetable_t* check_time(int days,int st_time, int fn_time, timetable_t* list_head)
{
	timetable_t* tmp = list_head;
	while(tmp){
		if(tmp->days==days){
			if(fn_time >= tmp->st_time && tmp->fn_time >= st_time)
				return tmp;
		}
		tmp = tmp->next;
	}
	return tmp;
}

int timesize(int x, int y)
{
	if(x == y)
		return 0;
	else 
		return 1;
}
