A few notes on waveform selector and its effect on combined waveforms
---

* T  
When triangle is selected the LSB is grounded.

* S  
When saw is selected the XOR circuit is disabled.
On 6581 the top bit is directly connected with the oscillator MSB adder, causing the output top bit to pull down the accumulator MSB.

* P  
When pulse is selected all bits are interconnected.

* N  
When noise is selected lower 4 bits are grounded.
The output bits pull down the LFSR register bits during shifting.

* S+T  
When saw and triangle are both selected all bits are interconnected.
As the XOR circuit is disabled we have a combination of saw and the same saw left shifted by one.
On 8580 the upper 8 bit selectors have double resistance.

When one of the input bits of the waveform selector is zero the output is being pull down, basically we have an AND of the selected waveforms. Additionally, when pulse or a combination of saw and triangle are selected, the bits are interconnected so some more neighboring output lines are pulled down by grounded bits, with a strength depending on the model, temperature and the status of inputs.


Noise writeback
---
Normally, when noise is selected along with another waveform, c2 is on and the output bits pull down the corresponding shift register bits.
During shift phase 1 the SR bits are floating and will be driven by the output of combined waveforms, or slowly turn high.
When switching to phase 2 the last value of the bit from phase 1 is shifted into the following; from phi2 onward the output bits again will pull down the corresponding shift register bits.

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

_normal cycles_
~~~
         ^                     ^
         |                     |
         +-------------+       +-------------+
         |             |       |             |
         +---o<|---+   |       +---o<|---+   |
         |         |   |       |         |   |
         |         |   |       |         |   |
         |         |   |       |         |   |
 >---/---+---|>o---+   +---/---+---|>o---+   +---/--->
~~~

_shift phase 1_
~~~
         ^                     ^
         |                     |
         +-------------+       +-------------+
         |             |       |             |
         +---o<|---+   |       +---o<|---+   |
         |         |   |       |         |   |
         /         /   |       /         /   |
         |         |   |       |         |   |
 >-------+---|>o---+   +-------+---|>o---+   +------->
~~~

_shift phase 2 (phi1)_
~~~
         ^                     ^
         |                     |
         +-------------+       +-------------+
         |             |       |             |
         +---o<|---+   |       +---o<|---+   |
         |         |   |       |         |   |
         /         |   |       /         |   |
         |         |   |       |         |   |
 >---/---+---|>o---+   +---/---+---|>o---+   +---/--->
~~~


About the samplings
---

The ST, PS and PST samplings for the 6581 are affected by the saw top bit writeback issue: once the top bit is driven low the accumulator is altered rendering the second half of the values invalid (those from 2048 to 4095). A few PS samplings, marked as odd, are in fact good.
New correct samplings are now available at https://sourceforge.net/p/vice-emu/bugs/1887/
