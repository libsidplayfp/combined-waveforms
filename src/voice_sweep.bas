
100 poke 53280,0: poke 53281,0
110 poke 53265,11
130 poke 54296,15
140 poke 54272+3,8:poke 54272+1,0
150 poke 54272+5,8:poke 54272+6,198
160 for a=16 to 128 step 16
170 if a>64 then poke 54272+3,0
180 poke 54272+4,a+1
190 for f=0 to 254 step 2:poke 54272+1,f:next f
200 poke 54272+4,a:for w=0 to 200:nextw
210 poke 54272+4,8:poke 54272+1,0
220 next a
290 poke 53265,27
300 poke 53280,254:poke 53281,246
310 poke 54295,0
