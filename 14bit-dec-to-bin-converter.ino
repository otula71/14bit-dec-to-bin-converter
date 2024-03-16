#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, A5, A4, U8X8_PIN_NONE); 
int16_t cislo = 0; //používám signed int, aby mohl podtečení nuly srovnat pomocí constrain
uint8_t cyklus=0;

void setup(){
  DDRD = 255;
  DDRB = 255; //nastavím všechny digitální piny jako výstupní
  u8g2.begin();
  zobraz(cislo);
}

void loop(){
 uint8_t zmena = PINC & 6; //přečtu pin A1 a A2

 switch (zmena) {
  case 2: // HIGH na pinu A1
   if (cyklus<8) {cislo+=1;cyklus+=1;}
   else if (cyklus<20) {cislo+=10;cyklus+=1;} //zrychluji při držení tlačítka
   else if (cyklus<30) {cislo+=100;cyklus+=1;}
   else {cislo+=1000;cyklus+=1;}
   cislo=constrain(cislo,0,16383); //omezuji minimum a maximum
                                   // šlo by to podmínkou, ale tohle je jednodušší 
   zobraz(cislo);
   break;

  case 4: // HIGH na pinu A2
   if (cyklus<8) {cislo-=1;cyklus+=1;}
   else if (cyklus<20) {cislo-=10;cyklus+=1;} 
   else if (cyklus<30) {cislo-=100;cyklus+=1;}
   else {cislo-=1000;cyklus+=1;}
   cislo=constrain(cislo,0,16383);
   zobraz(cislo);
   break;

  case 6: // HIGH na obou pinech
   cislo=0; 
   zobraz(cislo);
   break;

  default: // LOW na obou pinech
    cyklus=0;
    break;
 }

}



/*************************************************************************
* Název funkce: zobraz
**************************************************************************
* Funkce, která jen zavolá ostatní zobrazovací funkce
* 
* Parametry:
*  číslo uint16_t
* 
* Vrací:
*  none (volá zobrazovací funkce)
*************************************************************************/
void zobraz(int16_t x) {
      displej(x);
      led(x);
}



/*************************************************************************
* Název funkce: displej
**************************************************************************
* Funkce pro zobrazení desítkové hodnoty čísla na displeji 
* 
* Parametry:
*  číslo v desítkovém tvaru
* 
* Vrací:
*  none
*************************************************************************/
void displej(uint16_t x){
  u8g2.setFont(u8g2_font_BBSesque_tf);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("Desitkova hodnota");
  u8g2.setFont(u8g2_font_fub30_tn);
  uint8_t pozice=(x>9999)?0:(x>999)?20:(x>99)?30:(x>9)?40:50;
  u8g2.setCursor(pozice, 60);
  u8g2.print(x); 
  u8g2.sendBuffer();
  }


/*************************************************************************
* Název funkce: led
**************************************************************************
* Funkce pro zobrazení dvojkové hodnoty na řadě LED
* 
* Parametry:
*  číslo v desítkovém tvaru
* 
* Vrací:
*  none
*************************************************************************/
void led(uint16_t x){
  uint8_t blok1 = x/256;
  uint8_t blok2 = x-blok1*256;
  PORTB = blok1;
  PORTD = blok2;
  }
