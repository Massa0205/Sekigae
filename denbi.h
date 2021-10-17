/*====================================================================*/
/*	denbi.h	                                                   		  */
/*====================================================================*/
#include <stdio.h>
#include <conio.h>
#include <windows.h>

/*====================================================================*/
/*	cursor	�J�[�\���\���^��\���̐؂�ւ�							  */
/*====================================================================*/
void cursor(int flag)
{
	HANDLE hdt;	/* �W���o�̓f�o�C�X�n���h�� */
	CONSOLE_CURSOR_INFO cci;
	hdt=GetStdHandle(STD_OUTPUT_HANDLE);	/* �W���o�̓f�o�C�X�n���h���擾 */
	GetConsoleCursorInfo(hdt, &cci);// CONSOLE_CURSOR_INFO�\���̂̌��݂̏�Ԃ��擾
	// �����o�ϐ��ł���bVisible���J�[�\���̕\���E��\���𐧌䂷��ϐ��Ȃ̂ŁA�����FALSE�ɂ��鎖�ŃJ�[�\�����\���ɂł���
	if(flag == FALSE)
		cci.bVisible = FALSE;
	else
		cci.bVisible = TRUE;

	// �ύX�����\���̏����R���\�[��Window�ɃZ�b�g����
	SetConsoleCursorInfo(hdt, &cci);
}
/*====================================================================*/
/*	locate	���W�ݒ菈��											  */
/*		�����P�F�w���@�@�����Q�F�x���@�@�߂�l�F�Ȃ�				  */
/*		���W�̌��_�͍��オ�i0,0)�Ƃ���								  */
/*====================================================================*/
void locate( int y, int x )
{
	HANDLE hdt;	/* �W���o�̓f�o�C�X�n���h�� */
	COORD pos;	/* ���W(X,Y) */

	hdt=GetStdHandle(STD_OUTPUT_HANDLE);	/* �W���o�̓f�o�C�X�n���h���擾 */

	pos.X = (SHORT)x;	/* �w���W�ݒ� */
	pos.Y = (SHORT)y;	/* �x���W�ݒ� */
	
	SetConsoleCursorPosition(hdt, pos);	/* �W���o�͂ɃJ�[�\���ʒu�ݒ� */
}

/*====================================================================*/
/*	color	�����F�ݒ菈��											  */
/*		�����P�F�O�i�F�@�@�߂�l�F�Ȃ�				  				  */
/*====================================================================*/
void color( int col )
{
	HANDLE hdt;		/* �W���o�̓f�o�C�X�n���h�� */
	WORD wcol = 0;	/* �e�L�X�g�F(�O�i�F) */

	hdt=GetStdHandle(STD_OUTPUT_HANDLE);	/* �W���o�̓f�o�C�X�n���h���擾 */

	switch(col)
	{
		case 1:/* �� */
			wcol = FOREGROUND_RED;
			break;
		case 2:/* �� */
			wcol = FOREGROUND_GREEN;
			break;
		case 3:/* �� */
			wcol = FOREGROUND_RED | FOREGROUND_GREEN;
			break;
		case 4:/* �� */
			wcol = FOREGROUND_BLUE;
			break;
		case 5:/* �� */
			wcol = FOREGROUND_RED | FOREGROUND_BLUE;
			break;
		case 6:/* ���F */
			wcol = FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;
		case 7:/* �� */
			wcol = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
			break;
		/* ���������͖��邢�F(INTENSITY ����) */
		case 11:/* �� */
			wcol = FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		case 12:/* �� */
			wcol = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case 13:/* �� */
			wcol = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case 14:/* �� */
			wcol = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			break;
		case 15:/* �� */
			wcol = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			break;
		case 16:/* ���F */
			wcol = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			break;
		case 17:/* �� */
			wcol = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		default:/* �� */
			wcol = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
			break;

	}
	SetConsoleTextAttribute(hdt, wcol);	/* �W���o�͂ɕ����F��ݒ� */
}
