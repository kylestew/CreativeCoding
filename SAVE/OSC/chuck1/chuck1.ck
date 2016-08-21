OscRecv recv;
12000 => recv.port;
recv.listen();

recv.event("note, i f") @=> OscEvent oe;

SinOsc s => JCRev r => dac;
0.2 => s.gain;
0.4 => r.mix;

fun void pollEvents() {
    while (true) {
        oe => now;
        while (oe.nextMsg() != 0) {
            Std.mtof(oe.getInt()) => s.freq;
            oe.getFloat() => r.mix;
        }
    }
}

spork ~pollEvents();
1::hour => now;