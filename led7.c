

#include <avr/io.h>

#include <string.h>
#include <stdlib.h>
#include "led7.h"

static const byte pin_digit[COUNT_DIG] = {
	PIN_SYMBOL_1,
	PIN_SYMBOL_2,
	PIN_SYMBOL_3,
	PIN_SYMBOL_4
};

static const byte selector_mask = (1 << PIN_SYMBOL_1 )|(1 << PIN_SYMBOL_2 )|(1 << PIN_SYMBOL_3 )|(1 << PIN_SYMBOL_4);


void Indicator_init() {
	IND_DDR_DATA = 0xFF;
	IND_PORT_DATA =  0xFF;	
	IND_DDR_SEL = (1 << PIN_SYMBOL_1 )|(1 << PIN_SYMBOL_2 )|(1 << PIN_SYMBOL_3 )|(1 << PIN_SYMBOL_4);
	IND_PORT_SEL = 0x00;
}

int ipow(int n, int power) {
	int res = 1;
	while(power--) res *= n;
	return res;
}

//Копипаст с n5110 lib
char *ind_ftoa(float f, int dec) {
	static char buf[16];
	char *p = buf + 15;
	int i = f * ipow(10, dec);
	int sign = i < 0 ? -1 : 1;
	i *= sign;
	do {
		*--p = '0' + (i % 10);
		i /= 10;
		if (--dec == 0) *--p = '.';
	}
	while (i != 0);	
	if (dec > 0) {
		while (dec-- > 0)
		*--p = '0';
		*--p = '.';
	}	
	if (*p == '.') *--p = '0';
	if (sign < 0) *--p = '-';
	return p;
}

byte lookup(unsigned char ch){
	for (byte i=0;i<sizeof(Symbols);i++){
		if (Symbols[i][0]==ch)  return Symbols[i][1];
	}
	return Symbols[0][0];
}
void DoIndicate(Indicator symbol,int repeat,FuncDelay fn){	
	fn();
	int rc = 0;
	while(!Indicator_IsDone(&symbol) || rc<repeat){
		Indicator_Print(&symbol);
		fn();
		rc++;
	}	
}

void Indicator_Symbol(Symbol *symbol){
	#ifdef COMMON_ANODE 
	IND_PORT_SEL = (IND_PORT_SEL& (~selector_mask)) | (1<<pin_digit[symbol->Pos-1]) ;//выключить все, включить один разряд
	//ничего не нужно показывать выключаем все
	if ((symbol->Value==0  && !symbol->DotPoint) || !symbol->Visible) {
		IND_PORT_DATA = lookup(SYMBOL_OFF);
		return;
	}	
	IND_PORT_DATA = lookup(symbol->Value) & ~((symbol->DotPoint)<<PIN_H);	
	#endif
	#ifdef COMMON_CATODE 	
	# error  "This mode COMMON_CATODE is not implemented!"
	#endif
}

void Indicator_Print(Indicator *symout) {
	if (symout->position>=COUNT_DIG) {
		symout->position=0;
	}
	Indicator_Symbol(&symout->Symbols[symout->position++]);
	return;
}

Indicator GeNewSymbols(){
	Indicator out;
	for (byte i=0;i<COUNT_DIG;i++){
		out.Symbols[i].Pos = i+1;
		out.Symbols[i].Visible = TRUE;
		out.Symbols[i].DotPoint = 0;
		out.Symbols[i].Value = 0;
	}
	return out;
}

//Вывод дробного числа
Indicator Indicator_PrintFloat(float value,int accuracy,Align a){
	Indicator out=GeNewSymbols();	
	char *buf = ind_ftoa(value,accuracy);
	char *point;
	point = strchr(buf,'.');	
	size_t len=strlen(buf);
	if ((int) (point-buf)>0) len--;
	byte i=0;
	if (a==RIGHT && len<COUNT_DIG) {
		i=COUNT_DIG-len;
	}	
	while (*buf!='\0' && i<COUNT_DIG){
		if (*buf=='.') {
			out.Symbols[i-1].DotPoint=TRUE;
		} else {
			out.Symbols[i].Value = *buf;
			i++;
		}			
		++buf;
	}
	Indicator_Print(&out);
	return out;
}

Indicator Indicator_Prints(char *data,Align a){
	Indicator out=GeNewSymbols();
	size_t len=strlen(data);
	byte i=0,p=0;
	if (a==RIGHT && len<COUNT_DIG) {
		i=COUNT_DIG-len;
	}
	while(i<COUNT_DIG && p<len){
		out.Symbols[i].Value = data[p++];
		i++;
	}
	Indicator_Print(&out);
	return out;
}

//Вывод символа с выбором позиции
Symbol Indicator_Char(unsigned char s,byte pos) {
	Symbol symbol;
	symbol.Pos = pos;
	symbol.DotPoint = 0;
	symbol.Value = s;
	Indicator_Symbol(&symbol);
	return symbol;
}

// Вывод целого числа с выравниваем
Indicator Indicator_PrintInt(int32_t value, Align a){
	Indicator out=GeNewSymbols();
	char buffer[10];
	itoa(value,buffer,10);
	size_t len=strlen(buffer);
	byte i=0,p=0;
	if (a==RIGHT && len<COUNT_DIG) {
		i=COUNT_DIG-len;
	}
	while(i<COUNT_DIG){
		out.Symbols[i].Value = buffer[p++];
		i++;
	}
	Indicator_Print(&out);
	return out;
}

byte Indicator_IsDone(Indicator *symout){
	if (symout->position>=COUNT_DIG) {
		symout->position=0;
		return 1;
	}
	return 0;
}

