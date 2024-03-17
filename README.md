# 14bit decimal to binary converter (Arduino)

Převodník mezi desítkovou a dvojkovou soustavou. Desítková čísla zobrazuje na displeji, dvojková pomocí řady diod.

Na piny D0 až D13 připojíme 14 LED (přes přiměřený rezistor), na piny A1 a A2 tlačítka zapojená jako pull down, OLED displej na piny A4 (SDA) a A5 (SCL).
Jedno tlačítko funguje pro navyšování čísla, druhé pro zmenšování čísla. LED diody v řadě za sebou (D13 D12 ... D0) vyjadřují dvojkové číslo.

![prevodnik](photo.jpg)

## Úprava s funkční sériovou komunikací
Pokud bychom chtěli piny 0 a 1 (tedy RX a TX) nechat volné pro sériovou komunikaci, tak by se nám to zkomplikovalo. Ale stačí se nad tím jen trochu zamyslet a jde to vyřešit.
Prvně bychom chtěli nastavit DDRD tak, abychom jedničku (OUTPUT) nastavili jen na piny 2-7, takže bychom binárně přičetli 11111100 (což je desítkově 252) k aktuálnímu stavu, tedy 
 `DDRD |= 252;`

A potom bychom museli vyřešit zápis. Blok B je snadný, ten bychom posunuli pouze o 6 bitů: `PORTB = x>>6;` a blok D bychom museli zadat tak, abychom zachovali poslední 2 bity -- ty získáme tak, že aktuální stav vynásobíme binárně 00000011, tedy `PORTD&3`, naše číslo vynásobíme binárně 00111111, tedy `x&63` a posuneme o 2 bity doleva, tedy `(x&63)<<2` a získané dvě hodnoty spolu binárně sečteme, výsledek by tedy byl `PORTD = ((x&63)<<2)|(PORTD&3);` (ty závorky tam asi nejsou všechny potřeba, ale nechce se mi zjišťovat, co má před čím přednost).
