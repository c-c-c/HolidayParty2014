// Daniel Shiffman
// Kinect Point Cloud example
// http://www.shiffman.net
// https://github.com/shiffman/libfreenect/tree/master/wrappers/java/processing

import org.openkinect.*;
import org.openkinect.processing.*;

// Kinect Library object
Kinect kinect;

float a = 0;

// Size of kinect image
int w = 640;
int h = 480;

int count = 0;


// We'll use a lookup table so that we don't have to repeat the math over and over
float[] depthLookUp = new float[2048];

void setup() {
  size(w,h,P3D);
  kinect = new Kinect(this);
  kinect.start();
  kinect.enableDepth(true);
  // We don't need the grayscale image in this example
  // so this makes it more efficient
  kinect.processDepthImage(false);

  // Lookup table for all possible depth values (0 - 2047)
  for (int i = 0; i < depthLookUp.length; i++) {
    depthLookUp[i] = rawDepthToMeters(i);
  }
}

void draw() {

  background(0);
  fill(255, 0, 0);
  textMode(SCREEN);
  text("Kinect FR: " + (int)kinect.getDepthFPS() + "\nProcessing FR: " + (int)frameRate,10,16);

  // Get the raw depth as array of integers
  int[] depth = kinect.getRawDepth();

  // We're just going to calculate and draw every 4th pixel (equivalent of 160x120)
  // by 20 (640/20 = 32 X 480/20 = 24) = 768 pixels
  // by 32 = 300 pixels
  int skip = 32;

  // Translate and rotate
  translate(width/2,height/2,-50);
//  rotateY(a);

  for(int x=0; x<w; x+=skip) {
    for(int y=0; y<h; y+=skip) {
      int offset = x+y*w;

      // Convert kinect data to world xyz coordinate
      int rawDepth = depth[offset];
      PVector v = depthToWorld(x,y,rawDepth);
//      println(rawDepth);

      strokeWeight(4);
      stroke(sin(rawDepth), cos(rawDepth), (int)((255.0/rawDepth) * 800.0) );
      pushMatrix();
      // Scale up by 200
      float factor = 300;
      // crazy dots
//      translate((v.x * sin(rawDepth)) *factor,(v.y * cos(rawDepth)) *factor,factor-v.z*factor);
      // regular
      translate((v.x) *factor,(v.y) *factor,factor-v.z*factor);
      // Draw a point
      count++;
      point(0,0);
      popMatrix();
    }
  }
  
//  println("Count: " + count);
  count = 0;

  // Rotate
  a += 0.015f;
}

// These functions come from: http://graphics.stanford.edu/~mdfisher/Kinect.html
float rawDepthToMeters(int depthValue) {
  if (depthValue < 2047) {
    return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
  }
  return 0.0f;
}

PVector depthToWorld(int x, int y, int depthValue) {

  final double fx_d = 1.0 / 5.9421434211923247e+02;
  final double fy_d = 1.0 / 5.9104053696870778e+02;
  final double cx_d = 3.3930780975300314e+02;
  final double cy_d = 2.4273913761751615e+02;

  PVector result = new PVector();
  double depth =  depthLookUp[depthValue];//rawDepthToMeters(depthValue);
  result.x = (float)((x - cx_d) * depth * fx_d);
  result.y = (float)((y - cy_d) * depth * fy_d);
  result.z = (float)(depth);
  return result;
}

void stop() {
  kinect.quit();
  super.stop();
}

