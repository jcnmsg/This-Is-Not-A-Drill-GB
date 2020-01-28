c:\gbdk\bin\lcc -Wa-l -DUSE_SFR_FOR_REG -c -o build/main.o game/main.c
c:\gbdk\bin\lcc -Wl-yt2 -Wl-yo2 -Wl-ya1 -DUSE_SFR_FOR_REG -o build/platinum-orchid.gb build/main.o

del "build\main.o"
del "build\main.lst"