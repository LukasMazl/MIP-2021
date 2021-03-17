# Úloha č. 2: Budíkobomba
## Zadání
Implementujte jednoduchou minutku (bombu). Po resetu zařízení se na displeji objeví odpočet 2:00. Zařízení odpočítává čas a když dojde k nule, mění displej (“exploze”, “zvonění”) a následně se zařízení resetuje.
Hodnotu času lze upravit pomocí tlačítek s funkcí “plus” a “minus” - dotykem hodnotu časovače snižujeme nebo zvyšujeme o vámi zvolený interval. Kdo si chce hrát, může udělat user-friendly ovládání se samostatnými tlačítky například pro minuty a vteřiny.

Můžete doplnit ještě tlačítko s funkcí “set” pro přepnutí z režimu odpočtu do režimu změny hodnoty časovače. Můžete ale budík udělat i bez této funkce, jde to. (S tím “set” je to zřejmě jednodušší).

Při zpracování úlohy maximálně využijte vše, co už máte v API systému mbed.com hotovo: vlákna, synchronizační nástroje, watchdog atd. atd.
## Co je cílem
Cílem je ukázat, jak snadný je vývoj aplikace, když se opřu byť o jednoduchý operační systém. Cílem je naučit vás správě pracovat s vlákny v mbed a dalšími součástmi.
## Co odevzdáte
- Video
- Kód aplikace v mbed
- Readme popisující, jak přesně jste k budíkobombě přistoupili.

## Ukazka

![Alt Text](https://github.com/LukasMazl/MIP-2021/blob/master/img/cv02.gif)


## TODO
  - Pouzit (nějak) watchdog
  - Opravit reset
  - Podrobnejsi readme
