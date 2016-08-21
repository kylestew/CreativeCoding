SinOsc s => JCRev r => dac;

0.2 => s.gain;
0.9 => r.mix;

[0, 2, 4, 7, 9, 11] @=> int hi[];

while (true) {
    Std.mtof(45 + Math.random2(0, 3) * 12 + hi[Math.random2(0, hi.cap()-1)]) => s.freq;
    100::ms => now;
}