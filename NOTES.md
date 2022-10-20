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
When saw and triangle are both selected all bits interconnected.
On 8580 the upper 8 bit selectors have double resistance.
As the XOR circuit is disabled we have a combination of saw and the same saw left shifted by one.

When one of the input bits is zero the output is being pull down, basically we have an AND of the selected waveforms. Additionally, when pulse or a combination of saw and triangle are selected, the bits are interconnected so some more neighboring output lines are pulled down by grounded bits, with a strength depending on the model, temperature and the status of inputs.

About the samplings
---

The ST, PS and PST samplings for the 6581 are affected by the saw top bit writeback issue: once the top bit is driven low the accumulator is altered rendering the second half of the values invalid (those from 2048 to 4095). A few PS samplings, marked as odd, are in fact good.
