#include "public.h"
#include "morse.h"

const static T_MORSE_PTN t_morse_ptn[26] =
{
	{	1	,	2	,	0	,	0	},	/*	A	*/
	{	2	,	1	,	1	,	1	},	/*	B	*/
	{	2	,	1	,	2	,	1	},	/*	C	*/
	{	2	,	1	,	1	,	0	},	/*	D	*/
	{	1	,	0	,	0	,	0	},	/*	E	*/
	{	1	,	1	,	2	,	1	},	/*	F	*/
	{	2	,	2	,	1	,	0	},	/*	G	*/
	{	1	,	1	,	1	,	1	},	/*	H	*/
	{	1	,	1	,	0	,	0	},	/*	I	*/
	{	1	,	2	,	2	,	2	},	/*	J	*/
	{	2	,	1	,	2	,	0	},	/*	K	*/
	{	1	,	2	,	1	,	1	},	/*	L	*/
	{	2	,	2	,	0	,	0	},	/*	M	*/
	{	2	,	1	,	0	,	0	},	/*	N	*/
	{	2	,	2	,	2	,	0	},	/*	O	*/
	{	1	,	2	,	2	,	1	},	/*	P	*/
	{	2	,	2	,	1	,	2	},	/*	Q	*/
	{	1	,	2	,	1	,	0	},	/*	R	*/
	{	1	,	1	,	1	,	0	},	/*	S	*/
	{	2	,	0	,	0	,	0	},	/*	T	*/
	{	1	,	1	,	2	,	0	},	/*	U	*/
	{	1	,	1	,	1	,	2	},	/*	V	*/
	{	1	,	2	,	2	,	0	},	/*	W	*/
	{	2	,	1	,	1	,	2	},	/*	X	*/
	{	2	,	1	,	2	,	2	},	/*	Y	*/
	{	2	,	2	,	1	,	1	},	/*	Z	*/
};


/* ローカル関数 */
/* パルス生成 */
static void morse_vdBlinkPulse(USHORT u2_high_length, USHORT u2_low_length)
{
	digitalWrite(P13, HIGH);
	delay(u2_high_length);
	digitalWrite(P13, LOW);
	delay(u2_low_length);
	return;
}
/* 短音 */
static void morse_vdBlinkShortPulse(void)
{
	morse_vdBlinkPulse(200, 200);
	return;
}
/* 長音 */
static void morse_vdBlinkLongPulse(void)
{
	morse_vdBlinkPulse(500, 200);
	return;
}
/* S */
static void morse_vdBlinkCharS(void)
{
	morse_vdBlinkShortPulse();
	morse_vdBlinkShortPulse();
	morse_vdBlinkShortPulse();
	return;
}
/* O */
static void morse_vdBlinkCharO(void)
{
	morse_vdBlinkLongPulse();
	morse_vdBlinkLongPulse();
	morse_vdBlinkLongPulse();
	return;
}
/* １文字をテーブルNoに変換 */
static UCHAR morse_vdCnvCharToTableNo(UCHAR u1_buf)
{
	/* 小文字を大文字に変換 */
	if(u1_buf >= 'a')
	{
		u1_buf -= ('a' - 'A');
	}
	if(('A' <= u1_buf) && (u1_buf <= 'Z'))	/* 大文字の範囲チェック */
	{
		u1_buf -= 'A';	/* 'A'をテーブルの0とする */
	}
	else
	{
		u1_buf = 0xFF;
	}
	return u1_buf;
}
/* 1文字出力 */
static void morse_vdBlinkChar(UCHAR u1_no)
{
	UCHAR i = 0;
	for(UCHAR i = 0; i < 4; i++)
	{
		switch(t_morse_ptn[u1_no].u1_ptn[i])
		{
			case 1:
				morse_vdBlinkShortPulse();
				break;
			case 2:
				morse_vdBlinkLongPulse();
				break;
			default:
				return;
		}
	}
	return;
}
static void morse_vdBlink(UCHAR *p_buf)
{
	UCHAR u1_no;

	while(*p_buf != 0)
	{
		u1_no = morse_vdCnvCharToTableNo(*p_buf);	/* １文字アルファベットををテーブル番号に変換 */
		morse_vdBlinkChar(u1_no);	/* １文字点滅実行 */
		p_buf++;
	}
	return;
}
static void morse_vdInputSerial(UCHAR *p_buf, UCHAR u1_lmt)
{
	UCHAR i = 0;

	while (Serial.available())
	{
		p_buf[i] = (char)Serial.read();
		Serial.write(p_buf[i]);
		if(++i == u1_lmt - 1)
		{
			p_buf[i] = '\0';
			break;
		}
	}
	return;
}
/* グローバル関数 */
/* 初期化 */
void MORSE_vdInit(void)
{
	Serial.begin(9600);
	pinMode(P13, OUTPUT);
	return;
}
/* モールスメイン関数 */
void MORSE_vdMain(void)
{
	unsigned char u1_buf[50];

	memset(u1_buf, '\0', 50);	/* 初期化 */
	morse_vdInputSerial(u1_buf, 50);	/* 文字列を入力 */
	morse_vdBlink(u1_buf);	/* 文字列をモールス出力 */
	

	// morse_vdBlinkCharS();
	// morse_vdBlinkCharO();
	// morse_vdBlinkCharS();
	// delay(1000);
	return;
}