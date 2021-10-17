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

//プロトタイプ宣言
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
	/**************メニュー画面**************/ //二回目以降はバグるから配列の初期化処理を追加予定
	while( 1 ){
		locate( 0, 0);
		printf("モードを選択してください\n\n"); 
		printf("1:席替え\n\n"); //2,9
		printf("2:終了\n\n");	//4,7
		
		//モードを選択//
		mode = input_check(1,2);

		//ユーザーから教室と第三実習室のモード選択をしてもらいmode_sekigaeに渡す
		switch(mode){
			case 1://席替えモード
				locate(2,15);
				printf("1:教室(A303)");
				locate( 4, 15);
				printf("2:第三実習室\n");
				room = input_check( 1, 2);
				system("cls");
				printf("席替えを行うクラスの人数を入力してください。\n人数:");
				studentCount = input_check( 1, 45);
				mode_seat_change( mode, studentCount);
				break;
				
			case 2://終了
				system("cls");
				printf("3秒後にプログラムを終了します。");
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

	/****エクセルファイル読み込み処理****/
	printf("Excelファイルから生徒情報を読み込みますか？\n\n");
	printf("1:はい\n");
	printf("2:いいえ\n");

	//csvの読み込みが選択されたら
	if(	excelInput = y_n_check() == true){
		while(flg == 0){
			printf("読み込み元ファイル名を入力してください:");
			scanf("%s",filename);
			
			/******csv読み込めなかったら******/
			if(( fp = fopen(filename,"r")) == NULL ){
				//ファイルを閉じる
				fclose(fp);
				printf("ファイルの読み込みに失敗しました。\n");
				printf("もう一度入力しますか？\n\n");
				printf("1:はい\n");
				printf("2:いいえ\n");	
				//ファイルの再度入力がNO ファイル入力は0のままループ抜ける
				if( retry = y_n_check() == false){
					flg = 2;
				}
				else{
					continue;
				}
			}
			/******読み込みに成功******/
			else{
				printf("ファイルの読み込みに成功しました。");
				fp = fopen(filename,"r");
				excelLoad = true;
				//読み込みモードでファイルオープン
				for(i=0;i<studentCount;i++){
					fscanf(fp,"%s",(studentInfoPointer+i)->name);
					studentInfoPointer->seatDecide=false;
				}
				//ファイルを閉じる
				fclose(fp);
				flg=1;
			}
		}
		//エクセルファイルが入力されなかったら
		if(excelInput==false || flg == 2){
			//生徒情報の番号を書き込み
			for( i=0 ;i < studentCount;i++){
				sprintf((studentInfoPointer+1)->name,"%d",i);
				(studentInfoPointer+1)->seatDecide=false;
			}
		}
		getch();
	}
	//*stup=stup[0];
	//生徒の名簿出力
	for( i = 0; i < studentCount ; i++){
		printf("\n");
		printf("%s\n",( studentInfoPointer + i )->name);
	}
	getch();

	//各々の教室席替え処理へ
	switch(room){ 
		case 1://教室
			mode_kyousitu(studentInfoPointer,studentCount);
			break;
		case 2://第三
			mode_daisan(studentInfoPointer,studentCount);
			break;
	}
}

void mode_kyousitu(STUDENT *studentInfoPointer,int studentCount)//教室のlocateを初期化
{
	int x,y,i;
	LOCATE *locatep;
	FILE *fp;
	locatep=(LOCATE*)malloc(45*(sizeof(LOCATE)));//教室(46席)分locate配列確保
	*locatep=locatep[0];
	x=2;
	y=2;

	for(i=0;i<45;i++){
		(locatep+i)->seatusages=100;//すべての席を未使用で初期化
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
	//ファイルに書き出し
	*locatep=locatep[0];
	for(i=0;i<45;i++){
		fprintf(fp,"%d \n",(locatep+i)->seatusages);
	}
	fclose(fp);


	printf("エクセルへの書き出しが完了しました。");

	getch();



}
//////////////////////////////////////////////////////////////////////////////////////////
void mode_daisan(STUDENT *studentInfoPointer,int ninzu)//第三実習室のlocateを初期化
{
	int x,y,i;
	LOCATE *locatep;
	FILE *fp;
	

	locatep=(LOCATE*)malloc(57*(sizeof(LOCATE)));
	*locatep=locatep[0];
	x=2;
	y=2;
	for(i=0;i<57;i++){
		//使用不可席情報
		if(i==0 || i==10 || i==17 || i==18 || i==19 || i==29 || i==36 || i==37 || i==48 || i==47 || i==55 || i==56 ){
			(locatep+i)->seatusages=99;	//使用済み
		}
		else{
			(locatep+i)->seatusages=100;	//未使用
		}
		//行
		if(i%19==0){
			y=2;
		}
		(locatep+i)->locate_x=x;
		(locatep+i)->locate_y=y;
		//列
		if((i+1)%19==0){
			x+=30;
		}
		y+=4;
	}
	*locatep=locatep[0];
	studentInfoPointer=output_seat(locatep,studentInfoPointer,ninzu,2);
	system("cls");
	fp=fopen("daisandata.csv","w");
	//ファイルに書き出し
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
		case 1://教室
			MAX=44;
			heya=1;
			occupiedseat=45-studentCount;
			break;
		case 2://第三実習室
			MAX=56;
			heya=2;
			occupiedseat=45-studentCount;
			break;
	}
	system("cls");
	cursor_y=0;
	cursor(FALSE);
	//教室の席UI表示
	layout_output(locatep,studentInfoPointer,room);
	//使わない席を選択
	while(sekicnt<occupiedseat){
		locate(50,200);
		printf("使用しない席を%dつ選択してください。(W,Sで操作、Enterで決定、Jで取り消)",occupiedseat-sekicnt);
		selected=getch();
		layout_outputfaster(locatep,studentInfoPointer,room);
		if( cursor_y >= 0 && (selected =='s' || selected =='w')){
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("　　　　　　　　　　  ");
			layout_outputfaster(locatep,studentInfoPointer,room);
		}
		if(selected=='w' && cursor_y>0){	//上に移動
			cursor_y--;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");
		}
		else if(selected=='w' && cursor_y==0){
			cursor_y=MAX;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");

		}
		else if(selected=='s' && cursor_y<MAX){//下に移動
			cursor_y++;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");
		}
		else if(selected=='s' && cursor_y==MAX){
			cursor_y=0;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");

		}
		else if(selected=='\r'){				//決定
			(locatep+cursor_y)->seatusages=99;
			layout_outputfaster(locatep,studentInfoPointer,room);
			sekicnt++;
		}
		else if(selected=='j'){	
			sekicnt--;			//取り消し
			(locatep+cursor_y)->seatusages=100;
			system("cls");
			layout_output(locatep,studentInfoPointer,room);
			locate(50,200);
			printf("使用しない席を選択してください。(W,Sで操作、Enterで決定、Jで取り消し)");
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");
		}
	}
	system("cls");
	/********席先取り********/
	cursor_y=0;
	locate(50,200);
	printf("先取り指定する席を選択してください。(W,Sで操作、Enterで決定、Jで取り消し,Gで終了)");
	layout_output(locatep,studentInfoPointer,heya);
	while(1){
		selected=getch();
		layout_outputfaster(locatep,studentInfoPointer,room);
		if(cursor_y>=0 && (selected=='s' || selected=='w')){
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("　　　　　　　　　　  ");
			layout_outputfaster(locatep,studentInfoPointer,room);
		}
		if(selected=='w' && cursor_y>0){	//上に移動
			cursor_y--;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");
		}
		else if(selected=='w' && cursor_y==0){
			cursor_y=MAX;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");

		}
		else if(selected == 's' && cursor_y < MAX){//下に移動
			cursor_y++;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");
		}
		else if(selected == 's' && cursor_y == MAX){
			cursor_y=0;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");

		}
		else if(selected == '\r' && (locatep+cursor_y)->seatusages != 1){	//決定
			locate(50,200);
			printf("この席を指定する生徒の出席番号を入力してください。(半角数字で入力してください)");
			locate(50,201);
			printf("番号:");
			num=input_check(1,studentCount);
			num--;
			if((studentInfoPointer+num)->seatDecide == TRUE){
				locate(50,202);
				printf("※すでに指定された生徒を指定しようとしています。");
			}
			else{
				(studentInfoPointer+num)->seatDecide = TRUE;
				(locatep+cursor_y)->seatusages=num;
				system("cls");
				layout_output(locatep,studentInfoPointer,room);
				locate(50,202);
				printf("先取り指定する席を選択してください。(W,Sで操作、Enterで決定、Jで取り消し,Gで終了)");
			}
		}
		else if(selected=='j'){				//取り消し
			if((locatep+cursor_y)->seatusages!=99){
				system("cls");
				layout_output(locatep,studentInfoPointer,room);
				locate(5,202);
				printf("先取り指定をする席を選択してください。(W,Sで操作、Enterで決定、Jで取り消し,Gで終了)");
				locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
				printf("  *******選択中*******");
			}
		}
		else if(selected == 'g'){
			break;
		}
	}
	//席替え//
	//生徒をランダムに割り振る席を選んでください
	system("cls");
	cursor_y=0;
	locate(50,202);
	printf("生徒をランダムに割り振る席を選択してください。(W,Sで操作、Enterで決定)");
	layout_output(locatep,studentInfoPointer,heya);
	sekicnt=0;
	while(1){
		selected=getch();
		layout_outputfaster(locatep,studentInfoPointer,heya);
		if(cursor_y>=0 && (selected=='s' || selected=='w')){
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("　　　　　　　　　　  ");
			layout_outputfaster(locatep,studentInfoPointer,heya);
		}
		if(selected=='w' && cursor_y>0){	//上に移動
			cursor_y--;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");
		}
		else if(selected=='w' && cursor_y==0){
			cursor_y=MAX;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");

		}
		else if(selected=='s' && cursor_y<MAX){//下に移動
			cursor_y++;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");
		}
		else if(selected=='s' && cursor_y==MAX){
			cursor_y=0;
			locate((locatep+cursor_y)->locate_y,(locatep+cursor_y)->locate_x);
			printf("  *******選択中*******");

		}
		else if(selected=='\r'  ){				//決定
			if((locatep+cursor_y)->seatusages==100){//席が未使用だったら
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

	if(room==2){/////////////////第三実習室////////////////////////////////////
		for(i=0;i<=HEIGHT;i++){
			locate(i,1);
			printf("|");//左の左
			locate(i,25);
			printf("|");//左の右
			locate(i,31);
			printf("|");//真ん中の左
			locate(i,55);
			printf("|");//真ん中の右
			locate(i,61);
			printf("|");//右の左
			locate(i,85);
			printf("|");//右の右
			/*******絶対消すな******/
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
			if((locatep+i)->seatusages==99 ){//使用不可
				locate((locatep+i)->locate_y,(locatep+i)->locate_x);
				//color(LIGHTBLUE);
				printf("＊＊＊＊使用不可＊＊＊");
				//color(WHITE);
			}
			else if((locatep+i)->seatusages<99){//生徒指定済み
				locate((locatep+i)->locate_y,(locatep+i)->locate_x);
				//color(GREEN);
				if(excelLoad == FALSE){//生徒情報読み込みされてなかったら
					printf("　 %d番の生徒が指定済",(locatep+i)->seatusages);

				}
				else{//生徒情報読み込みされてたら
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
				printf("しらのせんせ～");
				locate(27,106);
				printf("(=^・ω・^=)っ");
			}
		}
		
		//color(WHITE);

		for(i=0;i<seatmax;i++){
			if((locatep+i)->seatusages==99 ){//使用不可
				locate((locatep+i)->locate_y,(locatep+i)->locate_x);
				//color(LIGHTBLUE);
				printf("＊＊＊＊使用不可＊＊＊");
				//color(WHITE);
			}
			else if((locatep+i)->seatusages<99){//生徒指定済み
				locate((locatep+i)->locate_y,(locatep+i)->locate_x);
				//color(GREEN);
				if(excelLoad == FALSE){//生徒情報読み込みされてなかったら
					printf("　 %d番の生徒が指定済",(locatep+i)->seatusages);

				}
				else{//生徒情報読み込みされてたら
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
		case 1://教室
			MAX=45;
			break;
		case 2://第三実習室
			MAX=57;
			break;
	}


	for(i=0;i<MAX;i++){
		if((locatep+i)->seatusages==99 ){
			locate((locatep+i)->locate_y,(locatep+i)->locate_x);
			//color(LIGHTBLUE);
			printf("＊＊＊＊使用不可＊＊＊");
			//color(WHITE);
		}
		else if((locatep+i)->seatusages<99 && excelLoad == FALSE){
			locate((locatep+i)->locate_y,(locatep+i)->locate_x);
			//color(GREEN);
			printf("　 %d番の生徒が指定済",(locatep+i)->seatusages);
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

	if(cnt==0){//初めての呼び出しだったら
		for(i=0;i<studentCount;i++){
			if((studentInfoPointer+i)->seatDecide != TRUE){
				if(cnt2==0){
					top=(struct list*)malloc(sizeof(struct list));//先頭のリスト作成top
					top->num=i;
					head=top;
					cnt2++;
				}
				else{
					p=(struct list*)malloc(sizeof(struct list));//先頭以降のリストをランダムに割り振る生徒の人数分作成
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
			head=head->next;//参照されたデータがtop(最初のリスト以外)
			p=head;
		}
	}
	else{
		p=head;
		top=head->next;//参照されたデータがtop(最初のリスト)だったらtop->nextを次のtopとする
	}
	number=head->num;
	cnt++;

	head=delete_list(p);//参照済のリスト削除

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