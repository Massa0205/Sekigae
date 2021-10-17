#include "MASA.h"
#include <stdbool.h>


bool excelLoad;

struct list{
	int num;
	struct list* prev;
	struct list* next;
};

typedef struct{
	int locate_x;
	int locate_y;
	int seatusages;
}LOCATE;

typedef struct{
	char name[24];
	bool seatDecide;
}STUDENT;

#define LIGHTBLUE 6
#define WHITE 7
#define MAX_HEIGHT 76
#define GREEN 12
#define MAX_WIDTH
#define KYOUSITU 1
#define DAISAN 2

//�v���g�^�C�v�錾
void mode_seat_change(int room,int ninzu);
void mode_kyousitu(STUDENT *stup,int ninzu);
void mode_daisan(STUDENT *stup,int ninzu);
void layout_output(LOCATE *locatep,STUDENT* stup,int room);
void layout_outputfaster(LOCATE *locatep,STUDENT* stup,int room);
STUDENT* output_seat(LOCATE *locatep,STUDENT *stup,int ninzu,int room);
int ransuu(int ninzu,STUDENT* stup,int x,int y);
struct list* delete_list(struct list*current);
bool y_n_check(void);




int main(void)
{

	int mode,room,studentCount;
	int x,y;

	system("cls");
	/**************���j���[���**************/ //���ڈȍ~�̓o�O�邩��z��̏�����������ǉ��\��
	while( 1 ){
		locate( 0, 0);
		printf("���[�h��I�����Ă�������\n\n"); 
		printf("1:�ȑւ�\n\n"); //2,9
		printf("2:�I��\n\n");	//4,7
		
		//���[�h��I��//
		mode = input_check(1,2);

		//���[�U�[���狳���Ƒ�O���K���̃��[�h�I�������Ă��炢mode_sekigae�ɓn��
		switch(mode){
			case 1://�ȑւ����[�h
				locate(2,15);
				printf("1:����(A303)");
				locate( 4, 15);
				printf("2:��O���K��\n");
				room = input_check( 1, 2);
				system("cls");
				printf("�ȑւ����s���N���X�̐l������͂��Ă��������B\n�l��:");
				studentCount = input_check( 1, 45);
				mode_seat_change( mode, studentCount);
				break;
				
			case 2://�I��
				system("cls");
				printf("3�b��Ƀv���O�������I�����܂��B");
				Sleep(3000);
				exit(1);
		}
	}
}
void mode_seat_change( int room, int studentCount)
{

	int but, i, flg = 0;
	bool excelInput,retry;
	char filename[50];
	FILE *fp;
	STUDENT *studentInfoPointer;

	studentInfoPointer=(STUDENT*)malloc(studentCount*(sizeof(STUDENT)));
	*studentInfoPointer = studentInfoPointer[0];

	/****�G�N�Z���t�@�C���ǂݍ��ݏ���****/
	printf("Excel�t�@�C�����琶�k����ǂݍ��݂܂����H\n\n");
	printf("1:�͂�\n");
	printf("2:������\n");

	//csv�̓ǂݍ��݂��I�����ꂽ��
	if(	excelInput = y_n_check() == true){
		while(flg == 0){
			printf("�ǂݍ��݌��t�@�C��������͂��Ă�������:");
			scanf("%s",filename);
			
			/******csv�ǂݍ��߂Ȃ�������******/
			if(( fp = fopen(filename,"r")) == NULL ){
				//�t�@�C�������
				fclose(fp);
				printf("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n");
				printf("������x���͂��܂����H\n\n");
				printf("1:�͂�\n");
				printf("2:������\n");	
				//�t�@�C���̍ēx���͂�NO �t�@�C�����͂�0�̂܂܃��[�v������
				if( retry = y_n_check() == false){
					flg = 2;
				}
				else{
					continue;
				}
			}
			/******�ǂݍ��݂ɐ���******/
			else{
				printf("�t�@�C���̓ǂݍ��݂ɐ������܂����B");
				fp = fopen(filename,"r");
				excelLoad = true;
				//�ǂݍ��݃��[�h�Ńt�@�C���I�[�v��
				for(i=0;i<studentCount;i++){
					fscanf(fp,"%s",(studentInfoPointer+i)->name);
					studentInfoPointer->seatDecide=false;
				}
				//�t�@�C�������
				fclose(fp);
				flg=1;
			}
		}
		//�G�N�Z���t�@�C�������͂���Ȃ�������
		if(excelInput==false || flg == 2){
			//���k���̔ԍ�����������
			for( i=0 ;i < studentCount;i++){
				sprintf((studentInfoPointer+1)->name,"%d",i);
				(studentInfoPointer+1)->seatDecide=false;
			}
		}
		getch();
	}
	//*stup=stup[0];
	//���k�̖���o��
	for( i = 0; i < studentCount ; i++){
		printf("\n");
		printf("%s\n",( studentInfoPointer + i )->name);
	}
	getch();

	//�e�X�̋����ȑւ�������
	switch(room){ 
		case 1://����
			mode_kyousitu(studentInfoPointer,studentCount);
			break;
		case 2://��O
			mode_daisan(studentInfoPointer,studentCount);
			break;
	}
}

void mode_kyousitu(STUDENT *studentInfoPointer,int studentCount)//������locate��������
{
	int x,y,i;
	LOCATE *locatep;
	FILE *fp;
	locatep=(LOCATE*)malloc(45*(sizeof(LOCATE)));//����(46��)��locate�z��m��
	*locatep=locatep[0];
	x=2;
	y=2;

	for(i=0;i<45;i++){
		(locatep+i)->seatusages=100;//���ׂĂ̐Ȃ𖢎g�p�ŏ�����
		(locatep+i)->locate_x=x;
		(locatep+i)->locate_y=y;

		if((i+1)%5==0){
			if((i+1)%15==0){
				x+=31;
			}
			else{
				x+=24;
			}
		}
		y+=4;
		if((i+1)%5==0){
			y=2;
		}

	}
/*for(i=0;i<45;i++){
printf("x:%d",(locatep+i)->locate_x);
printf("y:%d",(locatep+i)->locate_y);
printf("a:%d\n",(locatep+i)->seatusages);
}
getch();*/
	*locatep=locatep[0];
	studentInfoPointer=output_seat(locatep,studentInfoPointer,studentCount,1);
	fp=fopen("kyousitudata.csv","w");
	//�t�@�C���ɏ����o��
	*locatep=locatep[0];
	for(i=0;i<45;i++){
		fprintf(fp,"%d \n",(locatep+i)->seatusages);
	}
	fclose(fp);


	printf("�G�N�Z���ւ̏����o�����������܂����B");

	getch();



}
//////////////////////////////////////////////////////////////////////////////////////////
void mode_daisan(STUDENT *studentInfoPointer,int ninzu)//��O���K����locate��������
{
	int x,y,i;
	LOCATE *locatep;
	FILE *fp;
	

	locatep=(LOCATE*)malloc(57*(sizeof(LOCATE)));
	*locatep=locatep[0];
	x=2;
	y=2;
	for(i=0;i<57;i++){
		//�g�p�s�ȏ��
		if(i==0 || i==10 || i==17 || i==18 || i==19 || i==29 || i==36 || i==37 || i==48 || i==47 || i==55 || i==56 ){
			(locatep+i)->seatusages=99;	//�g�p�ς�
		}
		else{
			(locatep+i)->seatusages=100;	//���g�p
		}
		//�s
		if(i%19==0){
			y=2;
		}
		(locatep+i)->locate_x=x;
		(locatep+i)->locate_y=y;
		//��
		if((i+1)%19==0){
			x+=30;
		}
		y+=4;
	}
	*locatep=locatep[0];
	studentInfoPointer=output_seat(locatep,studentInfoPointer,ninzu,2);
	system("cls");
	fp=fopen("daisandata.csv","w");
	//�t�@�C���ɏ����o��
	*locatep=locatep[0];

	for(i=0;i<57;i++){
		fprintf(fp,"%d \n",(locatep+i)->seatusages);
	}
	fclose(fp);
}
STUDENT* output_seat(LOCATE *locatep,STUDENT *studentInfoPointer,int studentCount,int room)
{

	int cursor_y,i,num,number,unoccupiedseat,sekicnt=0,listninzu=0,MAX,heya,occupiedseat,x,y;
	char selected;
	STUDENT* workp=studentInfoPointer;

	switch(room){
		case 1://����
			MAX=44;
			heya=1;
			occupiedseat=45-studentCount;
			break;
		case 2://��O���K��
			MAX=56;
			heya=2;
			occupiedseat=45-studentCount;
			break;
	}
	system("cls");
	cursor_y=0;
	cursor(FALSE);
	//�����̐�UI�\��
	layout_output(locatep,studentInfoPointer,room);
	//�g��Ȃ��Ȃ�I��
	while(sekicnt<occupiedseat){
		locate(50,200);
		printf("�g�p���Ȃ��Ȃ�%d�I�����Ă��������B(W,S�ő���AEnter�Ō���AJ�Ŏ���)",occupiedseat-sekicnt);
		selected=getch();
		layout_outputfaster(locatep,studentInfoPointer,room);
		if( cursor_y >= 0 && (selected =='s' || selected =='w')){
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("�@�@�@�@�@�@�@�@�@�@  ");
			layout_outputfaster(locatep,studentInfoPointer,room);
		}
		if(selected=='w' && cursor_y>0){	//��Ɉړ�
			cursor_y--;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");
		}
		else if(selected=='w' && cursor_y==0){
			cursor_y=MAX;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");

		}
		else if(selected=='s' && cursor_y<MAX){//���Ɉړ�
			cursor_y++;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");
		}
		else if(selected=='s' && cursor_y==MAX){
			cursor_y=0;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");

		}
		else if(selected=='\r'){				//����
			(locatep+cursor_y)->seatusages=99;
			layout_outputfaster(locatep,studentInfoPointer,room);
			sekicnt++;
		}
		else if(selected=='j'){	
			sekicnt--;			//������
			(locatep+cursor_y)->seatusages=100;
			system("cls");
			layout_output(locatep,studentInfoPointer,room);
			locate(50,200);
			printf("�g�p���Ȃ��Ȃ�I�����Ă��������B(W,S�ő���AEnter�Ō���AJ�Ŏ�����)");
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");
		}
	}
	system("cls");
	/********�Ȑ���********/
	cursor_y=0;
	locate(50,200);
	printf("����w�肷��Ȃ�I�����Ă��������B(W,S�ő���AEnter�Ō���AJ�Ŏ�����,G�ŏI��)");
	layout_output(locatep,studentInfoPointer,heya);
	while(1){
		selected=getch();
		layout_outputfaster(locatep,studentInfoPointer,room);
		if(cursor_y>=0 && (selected=='s' || selected=='w')){
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("�@�@�@�@�@�@�@�@�@�@  ");
			layout_outputfaster(locatep,studentInfoPointer,room);
		}
		if(selected=='w' && cursor_y>0){	//��Ɉړ�
			cursor_y--;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");
		}
		else if(selected=='w' && cursor_y==0){
			cursor_y=MAX;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");

		}
		else if(selected == 's' && cursor_y < MAX){//���Ɉړ�
			cursor_y++;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");
		}
		else if(selected == 's' && cursor_y == MAX){
			cursor_y=0;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");

		}
		else if(selected == '\r' && (locatep+cursor_y)->seatusages != 1){	//����
			locate(50,200);
			printf("���̐Ȃ��w�肷�鐶�k�̏o�Ȕԍ�����͂��Ă��������B(���p�����œ��͂��Ă�������)");
			locate(50,201);
			printf("�ԍ�:");
			num=input_check(1,studentCount);
			num--;
			if((studentInfoPointer+num)->seatDecide == TRUE){
				locate(50,202);
				printf("�����łɎw�肳�ꂽ���k���w�肵�悤�Ƃ��Ă��܂��B");
			}
			else{
				(studentInfoPointer+num)->seatDecide = TRUE;
				(locatep+cursor_y)->seatusages=num;
				system("cls");
				layout_output(locatep,studentInfoPointer,room);
				locate(50,202);
				printf("����w�肷��Ȃ�I�����Ă��������B(W,S�ő���AEnter�Ō���AJ�Ŏ�����,G�ŏI��)");
			}
		}
		else if(selected=='j'){				//������
			if((locatep+cursor_y)->seatusages!=99){
				system("cls");
				layout_output(locatep,studentInfoPointer,room);
				locate(5,202);
				printf("����w�������Ȃ�I�����Ă��������B(W,S�ő���AEnter�Ō���AJ�Ŏ�����,G�ŏI��)");
				locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
				printf("  *******�I��*******");
			}
		}
		else if(selected == 'g'){
			break;
		}
	}
	//�ȑւ�//
	//���k�������_���Ɋ���U��Ȃ�I��ł�������
	system("cls");
	cursor_y=0;
	locate(50,202);
	printf("���k�������_���Ɋ���U��Ȃ�I�����Ă��������B(W,S�ő���AEnter�Ō���)");
	layout_output(locatep,studentInfoPointer,heya);
	sekicnt=0;
	while(1){
		selected=getch();
		layout_outputfaster(locatep,studentInfoPointer,heya);
		if(cursor_y>=0 && (selected=='s' || selected=='w')){
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("�@�@�@�@�@�@�@�@�@�@  ");
			layout_outputfaster(locatep,studentInfoPointer,heya);
		}
		if(selected=='w' && cursor_y>0){	//��Ɉړ�
			cursor_y--;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");
		}
		else if(selected=='w' && cursor_y==0){
			cursor_y=MAX;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");

		}
		else if(selected=='s' && cursor_y<MAX){//���Ɉړ�
			cursor_y++;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");
		}
		else if(selected=='s' && cursor_y==MAX){
			cursor_y=0;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******�I��*******");

		}
		else if(selected=='\r'  ){				//����
			if((locatep+cursor_y)->seatusages==100){//�Ȃ����g�p��������
				if(room==1){
					if(cursor_y==19  || cursor_y==29){
						//masa();
						system("cls");
						layout_output(locatep,studentInfoPointer,room);

					}
				}
				if(room==2){
					if(cursor_y==35 || cursor_y==34){
						//masa();
						system("cls");
						layout_output(locatep,studentInfoPointer,room);


					}
				}
				y=(locatep+cursor_y)->locate_y;
				x=(locatep+cursor_y)->locate_x;
				number=ransuu(studentCount,studentInfoPointer,x,y);
				if(number==-1){
					continue;
				}
				(locatep+cursor_y)->seatusages=number;
				(studentInfoPointer+number)->seatDecide=TRUE;
				layout_outputfaster(locatep,studentInfoPointer,room);
			}
		}
		else if(selected == 'g'){
			break;
		}
	}

	return studentInfoPointer;

}
//////////////////////////////////////////////////////////////////////////
void layout_output(LOCATE *locatep,STUDENT* studentInfoPointer,int room)
{

	int i,j,HEIGHT=20,seatmax=46,a;
	if(room==2){
		HEIGHT=76;
		seatmax=57;
	}

	if(room==2){/////////////////��O���K��////////////////////////////////////
		for(i=0;i<=HEIGHT;i++){
			locate(i,1);
			printf("|");//���̍�
			locate(i,25);
			printf("|");//���̉E
			locate(i,31);
			printf("|");//�^�񒆂̍�
			locate(i,55);
			printf("|");//�^�񒆂̉E
			locate(i,61);
			printf("|");//�E�̍�
			locate(i,85);
			printf("|");//�E�̉E
			/*******��Ώ�����******/
			if(i%4==0){
				for(j=1;j<85;j++){
					if((j>25 && j<31) || (j>55 && j<61)){
					}
					else{
						locate(i,j);
						printf("-");
					}
				}
			}
		}
		for(i=0;i<seatmax;i++){
			if((locatep+i)->seatusages==99 ){//�g�p�s��
				locate((locatep+i)->locate_y,(locatep+i)->locate_x);
				//color(LIGHTBLUE);
				printf("���������g�p�s������");
				//color(WHITE);
			}
			else if((locatep+i)->seatusages<99){//���k�w��ς�
				locate((locatep+i)->locate_y,(locatep+i)->locate_x);
				//color(GREEN);
				if(excelLoad == FALSE){//���k���ǂݍ��݂���ĂȂ�������
					printf("�@ %d�Ԃ̐��k���w���",(locatep+i)->seatusages);

				}
				else{//���k���ǂݍ��݂���Ă���
					a=(locatep+i)->seatusages;
					printf("   %s",(studentInfoPointer+a)->name);

				}
				//color(WHITE);
			}

		}
	}
	else if(room==1){
		for(i=0;i<=20;i++){
			locate(i,1);
			printf("|");
			locate(i,25);
			printf("|");
			locate(i,49);
			printf("|");
			locate(i,73);
			printf("|");

			locate(i,80);
			printf("|");
			locate(i,104);
			printf("|");
			locate(i,128);
			printf("|");
			locate(i,152);
			printf("|");

			locate(i,159);
			printf("|");
			locate(i,183);
			printf("|");
			locate(i,207);
			printf("|");
			locate(i,231);
			printf("|");



			if(i%4==0){
				for(j=1;j<231;j++){
					if((j>0 && j<74) || (j>79 && j<153) || (j>159 && j<232) ){
					locate(i,j);
					printf("-");
					}
				}
			}
		}
		//color(LIGHTBLUE);

		for(i=25;i<30;i++){
			locate(i,104);
			printf("|");
			locate(i,128);
			printf("|");
			if(i==25){
				locate(24,104);
				printf("------------------------");
				locate(26,106);
				printf("����̂��񂹁`");
				locate(27,106);
				printf("(=^�E�ցE^=)��");
			}
		}
		
		//color(WHITE);

		for(i=0;i<seatmax;i++){
			if((locatep+i)->seatusages==99 ){//�g�p�s��
				locate((locatep+i)->locate_y,(locatep+i)->locate_x);
				//color(LIGHTBLUE);
				printf("���������g�p�s������");
				//color(WHITE);
			}
			else if((locatep+i)->seatusages<99){//���k�w��ς�
				locate((locatep+i)->locate_y,(locatep+i)->locate_x);
				//color(GREEN);
				if(excelLoad == FALSE){//���k���ǂݍ��݂���ĂȂ�������
					printf("�@ %d�Ԃ̐��k���w���",(locatep+i)->seatusages);

				}
				else{//���k���ǂݍ��݂���Ă���
					a=(locatep+i)->seatusages;
					printf("   %s",(studentInfoPointer+a)->name);

				}
				//color(WHITE);
			}

		}
	}


}
void layout_outputfaster(LOCATE *locatep,STUDENT* studentInfoPointer,int room)
{
	int i,a,MAX;

	switch(room){
		case 1://����
			MAX=45;
			break;
		case 2://��O���K��
			MAX=57;
			break;
	}


	for(i=0;i<MAX;i++){
		if((locatep+i)->seatusages==99 ){
			locate((locatep+i)->locate_y,(locatep+i)->locate_x);
			//color(LIGHTBLUE);
			printf("���������g�p�s������");
			//color(WHITE);
		}
		else if((locatep+i)->seatusages<99 && excelLoad == FALSE){
			locate((locatep+i)->locate_y,(locatep+i)->locate_x);
			//color(GREEN);
			printf("�@ %d�Ԃ̐��k���w���",(locatep+i)->seatusages);
			//color(WHITE);
		}
		else if((locatep+i)->seatusages<99 && excelLoad == TRUE){
			a=(locatep+i)->seatusages;
			locate((locatep+i)->locate_y,(locatep+i)->locate_x);
			//color(GREEN);
			printf("   %s",(studentInfoPointer+a)->name);
			//color(WHITE);

		}
	}

}
/////////////////////////////////////////////////////////
int ransuu(int studentCount,STUDENT* studentInfoPointer,int x,int y)
{
	static int cnt=0,cnt2=0,outputcnt;
	int i,r,number,a,bangou,z;
	struct list *p,*work,*point,*t,*b;
	static struct list *head,*top;
	srand((unsigned int)time(NULL));
	*studentInfoPointer=studentInfoPointer[0];

	if(cnt==0){//���߂Ă̌Ăяo����������
		for(i=0;i<studentCount;i++){
			if((studentInfoPointer+i)->seatDecide != TRUE){
				if(cnt2==0){
					top=(struct list*)malloc(sizeof(struct list));//�擪�̃��X�g�쐬top
					top->num=i;
					head=top;
					cnt2++;
				}
				else{
					p=(struct list*)malloc(sizeof(struct list));//�擪�ȍ~�̃��X�g�������_���Ɋ���U�鐶�k�̐l�����쐬
					p->prev=head;
					p->next=NULL;
					p->num=i;
					head->next=p;
					head=p;
					cnt2++;
					outputcnt=cnt2;
				}
			}
		}
		top->prev=head;
		head->next=top;
	}
	else if(cnt==cnt2){
		return -1;
	}
	head=top;
	a=cnt2-cnt;
	for(i=0;i<20;i++){
		r=(int)(rand()*a/(RAND_MAX+1.0));
	}
	if(r!=0){
		for(i=0;i<r;i++){
			z=head->num;
			locate(y,x);
			printf("   %-20s",(studentInfoPointer+z)->name);
			Sleep(5);
			head=head->next;//�Q�Ƃ��ꂽ�f�[�^��top(�ŏ��̃��X�g�ȊO)
			p=head;
		}
	}
	else{
		p=head;
		top=head->next;//�Q�Ƃ��ꂽ�f�[�^��top(�ŏ��̃��X�g)��������top->next������top�Ƃ���
	}
	number=head->num;
	cnt++;

	head=delete_list(p);//�Q�ƍς̃��X�g�폜

	return number;
}
////////////////////////////////////////////////////////////////////////////////
struct list* delete_list(struct list*current)
{
	struct list* point;
	struct list* work;

	work=current->prev;
	point=current->next;
	work->next=point;
	point->prev=work;

	free(current);

	return point;

}

/////////////////////////////////////////////////////////
int input_check(int min,int max)
{
	int a=-1;

	while(1){
		scanf("%d",&a);
		if(a<=max && a>=min){
			return a;
		}
	}
}
bool y_n_check(void){
	int a=-1;
	while(1){
		scanf("%d",&a);
		if(a==1){
			return true;
		}
		else if(a==2){
			return false;
		}
	}
}