import ddf.minim.*;
import ddf.minim.ugens.*;
import processing.video.*;

Movie mov;

Minim       minim;
AudioOutput out;

int waveCount = 12;
ArrayList waves = new ArrayList();
ArrayList<Integer> freqs = new ArrayList<Integer>();
 
void setup() {
  size(1280, 720, P3D);
  
  mov = new Movie(this, "data.mp4");
  mov.loop();
 
  minim = new Minim(this);
 
  // use the getLineOut method of the Minim object to get an AudioOutput object
  out = minim.getLineOut();
 
  for (int i = 0; i < waveCount; i++) {
    Oscil wave = new Oscil( 440, 0.5f, Waves.SINE );
    wave.patch(out);
    waves.add(wave);
    freqs.add(0);
  }
}
 
int idx = 0;
 
void draw() {
  background(0);
  
  image(mov, 0, 0);
  
  for (int i = 0; i < waveCount; i++) {
    //Oscil wave = (Oscil)waves.get(i);
    //wave.setFrequency(freqs.get(i));
  }
  //wave0.setFrequency(freq0);
  //wave1.setFrequency(freq1);
  //wave2.setFrequency(freq2);
  //wave3.setFrequency(freq3);
  
  //println(freq0 + " :: " + freq1);

  
  // draw the waveform of the output
  //stroke(0);
  //strokeWeight(1);
  //for(int i = 0; i < out.bufferSize() - 1; i++) {
  // line( i, 50  - out.left.get(i)*50,  i+1, 50  - out.left.get(i+1)*50 );
  // line( i, 150 - out.right.get(i)*50, i+1, 150 - out.right.get(i+1)*50 );
  //}
}

int col;
 
void movieEvent(Movie m) {
  m.read();
  
  mov.loadPixels();
  int length = height*width-1;
  if (mov.pixels.length > 0) {
    for (int i = 0; i < waveCount; i++) {
     int idx = length/(i+1);
    //  if (idx > mov.pixels.length)
    //    idx = mov.pixels.length-1;
    //  int col = mov.pixels[idx];
    //  int bri = (int)map( brightness(col), 0, 255, 220, 2200 );
    //  freqs.set(i, bri);
    //  println(freqs.get(i));
    }
  }
}
 
//void keyPressed()
//{ 
//  switch( key )
//  {
//    case '1': 
//      wave.setWaveform( Waves.SINE );
//      break;
 
//    case '2':
//      wave.setWaveform( Waves.TRIANGLE );
//      break;
 
//    case '3':
//      wave.setWaveform( Waves.SAW );
//      break;
 
//    case '4':
//      wave.setWaveform( Waves.SQUARE );
//      break;
 
//    case '5':
//      wave.setWaveform( Waves.QUARTERPULSE );
//      break;
 
//    default: break; 
//  }
//}