# combined-waveforms

Tools to estimate the combined waveforms model parameters using the Monte-Carlo method based on samplings from real chips.

* combined: tool to estimate the model parameters based on samples;
* dump: saves the samples in csv format;
* rms: calculates the RMS of the samples for each combined waveform;
* voice_sweep: a BASIC program that plays a sweep for each waveform from $1 to $8.

Tools by Antti Lankila and Leandro Nini

Samples provided by
* Trurl
* ltx128
* reFX-Mike

https://sourceforge.net/p/vice-emu/bugs/1887  
https://github.com/libsidplayfp/libsidplayfp/issues/97

Sampling program by Dag Lem

https://github.com/daglem/reDIP-SID/blob/master/research/combsample.a65


## Notes

A few notes on waveform selector and its effect on combined waveforms
---

* Triangle  
When triangle is selected the LSB is grounded.

* Saw  
When saw is selected the triangle XOR circuit is disabled.
On 6581 the top bit is directly connected with the oscillator MSB adder, causing the output top bit to pull down the accumulator MSB when low.

* Pulse  
When pulse is selected all bits are interconnected.

* Noise  
When noise is selected lower 4 bits are grounded.
The output bits can pull down the LFSR register bits during shifting.

* Saw+Triangle  
When saw and triangle are both selected all bits are interconnected.
As the triangle XOR circuit is disabled we have a combination of saw and the same saw left shifted by one.
On 8580 the upper 8 bit selectors have double resistance.

When one of the input bits of the waveform selector is zero the output is connected to ground and thus pulled down, basically we have an AND of the selected waveforms. Additionally, when pulse and/or a combination of saw and triangle are selected, the bits are interconnected so some more neighboring output lines are pulled down by grounded bits, with a strength depending on the model, temperature, distance and the status of inputs.

When Pulse+Triangle are selected the LSB has a strong pulldown effect.


Noise writeback
---

Normally, when noise is selected along with another waveform, c2 is on and the output bits pull down the corresponding shift register bits.

~~~
       noi_out_x             noi_out_x+1
         ^                     ^
         |                     |
         +-------------+       +-------------+
         |             |       |             |
         +---o<|---+   |       +---o<|---+   |
         |         |   |       |         |   |
      c2 |      c1 |   |    c2 |      c1 |   |
         |         |   |       |         |   |
 >---/---+---|>o---+   +---/---+---|>o---+   +---/--->
     LC                    LC                    LC

         lfsr bit x            lfsr bit x+1

 |>o - inverter
  /  - transistor switch opened
~~~
_normal cycles_

During shift phase 1 the LFSR bits are floating and will be driven by the output of combined waveforms, or slowly turn high.

~~~
       noi_out_x             noi_out_x+1
         ^                     ^
         |                     |
         +-------------+       +-------------+
         |             |       |             |
         +---o<|---+   |       +---o<|---+   |
         |         |   |       |         |   |
      c2 /      c1 /   |    c2 /      c1 /   |
         |         |   |       |         |   |
 >-------+---|>o---+   +-------+---|>o---+   +------->
     LC                    LC                    LC
~~~
_shift phase 1_

When switching to phase 2 the last value of the bit from phase 1 is shifted into the following; from phi2 onward the output bits again will pull down the corresponding shift register bits.

~~~
       noi_out_x             noi_out_x+1
         ^                     ^
         |                     |
         +-------------+       +-------------+
         |             |       |             |
         +---o<|---+   |       +---o<|---+   |
         |         |   |       |         |   |
      c2 /      c1 |   |    c2 /      c1 |   |
         |         |   |       |         |   |
 >---/---+---|>o---+   +---/---+---|>o---+   +---/--->
     LC                    LC                    LC
~~~
_shift phase 2 (phi1)_


Noise shifting happens in two phases and is triggered by accumulator bit 19 or test bit (or reset).

~~~
     |       |     |                bit n     |   bit n+1
     | bit19 | clk | LC c1 c2    latch output | latch output
-----|-------|-----|----------  --------------|--------------
phi1 |   0   |  1  | 0  1  1       A <-> A    |   B <-> B
phi2 |   0   |  1  | 0  1  1       A <-> A    |   B <-> B
-----|-------|-----|----------  --------------|--------------
phi1 |   1   |  1  | 0  1  1       A <-> A    |   B <-> B      <- bit19 raises
phi2 |   1   |  1  | 0  1  1       A <-> A    |   B <-> B
-----|-------|-----|----------  --------------|--------------
phi1 |   1   |  0  | 1  0  0       X     A  --|-> A     B      <- shift phase 1
phi2 |   1   |  0  | 1  0  0       X     A  --|-> A     B
-----|-------|-----|----------  --------------|--------------
phi1 |   1   |  1  | 0  1  0       X --> X    |   A --> A      <- shift phase 2
phi2 |   1   |  1  | 0  1  1       X <-> X    |   A <-> A
~~~
_shift activated by bit 19_


While the test bit is held high shift phase 1 is repeated at every cycle. After some time, depending on technology and temperature, the bits turn high, as the input of the inverters is left floating.

~~~
     |      |                bit n     |   bit n+1
     | test | LC c1 c2    latch output | latch output
-----|------|----------  --------------|--------------
phi1 |  0   | 0  1  1       A <-> A    |   B <-> B
phi2 |  0   | 0  1  1       A <-> A    |   B <-> B
-----|------|----------  --------------|--------------
phi1 |  1   | 1  0  0       X     A  --|-> A     B      <- shift phase 1
phi2 |  1   | 1  0  0       X     A  --|-> A     B
-----|------|----------  --------------|--------------
                      ....
-----|------|----------  --------------|--------------
phi1 |  1   | 1  0  0       X     A  --|-> A     B      <- shift phase 1
phi2 |  1   | 1  0  0       X     A  --|-> A     B
-----|------|----------  --------------|--------------
phi1 |  0   | 0  1  0       X --> X    |   A --> A      <- shift phase 2
phi2 |  0   | 0  1  1       X <-> X    |   A <-> A
~~~
_shift activated by test bit_


About the samplings
---

The old ST, PS and PST samplings for the 6581 are affected by the saw top bit writeback issue: once the top bit is driven low the accumulator is altered rendering the second half of the values invalid (those from 2048 to 4095). A few PS samplings, marked as odd, seems to be in fact good for some reason (see https://github.com/libsidplayfp/libresidfp/issues/11).  
New samplings are no longer affected by this problem.


Hint
---

Perform multiple samplings on same chip and AND them toghether, this would eliminate random fluctuations and provide the base for a more stable model.
