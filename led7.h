
#ifndef LED7_H_
#define LED7_H_
// LED 7 Segment indicator with dot 4 point
//   _   _   _   _       A
//  |_|	|_|	|_|	|_|    F G B
//  |_|.|_|.|_|.|_|.   E   C
//                       D  H

// Укажите чисто знакомест индикатора
#define COUNT_DIG 4 //Всего знакомест

// Укажите какой режим работы с общим анодом или катодом, 
//расскомментировав соответствующую строку
#define COMMON_ANODE
//#define COMMON_CATODE

// Выбор пинов и порта для выбора цифры  (аноды)
#define IND_DDR_SEL				DDRB
#define IND_PORT_SEL			PORTB   //Порт для выбора знакоместа
#define PIN_SYMBOL_1			PORTB0  //Пин цифры 1
#define PIN_SYMBOL_2			PORTB1  //Пин цифры 2
#define PIN_SYMBOL_3			PORTB2  //Пин цифры 3
#define PIN_SYMBOL_4			PORTB3  //Пин цифры 4
// Порт для данных (катоды)
#define IND_DDR_DATA		DDRA
#define IND_PORT_DATA		PORTA
#define PIN_A				PORTA0
#define PIN_B				PORTA1
#define PIN_C				PORTA2
#define PIN_D				PORTA3
#define PIN_E				PORTA4
#define PIN_F				PORTA5
#define PIN_G				PORTA6
#define PIN_H				PORTA7


//--------------------------------------------------
#ifdef COMMON_ANODE
#undef COMMON_CATODE
#endif
#ifdef COMMON_CATODE
#undef COMMON_ANODE
#endif



#define TRUE  1
#define FALSE 0


typedef unsigned char byte;

typedef void FuncDelay();

typedef enum {
	LEFT=0,
	RIGHT=1
} Align;

/*typedef enum  {
	OFF=0,
	HALT=1,
	ONTHIRD=2,
	OFFTHIRD=3
} BlinkMode;
*/

typedef struct {
	unsigned char Value;
	byte Pos:4;			 //Позиция на индикаторе MAX=8
	byte Visible:1;      //Видимо или нет
	byte DotPoint:1;     //Точка включена или нет	
	//BlinkMode Blink:2;   //TODO: Мигающий символ 00-нет 01-1/2частоты 10-1/3частоты вкл 11-1/2 выкл
	//BlinkMode BlinkDot:2;//TODO: Мигающая точка 00-нет 01-1/2частоты 10-1/3частоты вкл 11-1/2 выкл
} Symbol;

typedef struct {
	byte position;
	Symbol Symbols[COUNT_DIG];
} Indicator;

void Indicator_init();
void DoIndicate(Indicator symbol,int repeat,FuncDelay fn);
Indicator Indicator_Prints(char *data,Align a);
Indicator Indicator_PrintInt(int32_t value, Align a);
Indicator Indicator_PrintFloat(float value,int accuracy,Align a);
byte Indicator_IsDone(Indicator *symout);
void Indicator_Print(Indicator *symout);
Symbol Indicator_Char(unsigned char s,byte pos);


//TODO: in PROGMEM
#define SYMBOL_OFF 0

// Для таблицы режим по умолчанию указан для общего анода
// бит равный 0 выставляем логический ноль на выходе
// для общего катода в Indicator_Symbol инвертировать значения
static const unsigned char Symbols [][2] =
{	//       HGFEDCBA
	{ SYMBOL_OFF  , 0b11111111 }, //OFF
	{ '0', 0b11000000 }, // 0
	{ '1', 0b11111001 }, // 1
	{ '2', 0b10100100 }, // 2
	{ '3', 0b10110000 }, // 3
	{ '4', 0b10011001 }, // 4
	{ '5', 0b10010010 }, // 5
	{ '6', 0b10000010 }, // 6
	{ '7', 0b11111000 }, // 7
	{ '8', 0b10000000 }, // 8 
	{ '9', 0b10010000 }, // 9
		
	{ '-', 0b10111111 }, // -
	// добавь свои	
	
};



#endif /* LED7_H_ */