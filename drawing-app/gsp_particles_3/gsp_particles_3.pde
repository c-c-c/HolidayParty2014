ParticleSystem ps;

void setup() {
  size(1920,1080);
  ps = new ParticleSystem(new PVector(width/2,50));
  background(255);
}

void draw() {
  
  ps.addParticle();
  ps.run();
}





// A class to describe a group of Particles
// An ArrayList is used to manage the list of Particles 

class ParticleSystem {
  ArrayList<Particle> particles;
  PVector origin;

  ParticleSystem(PVector location) {
    origin = location.get();
    particles = new ArrayList<Particle>();
  }

  void addParticle() {
    if(mousePressed == true) {
      PVector loc = new PVector(mouseX, mouseY);
      particles.add(new Particle(loc));    
    }
  }

  void run() {
    for (int i = particles.size()-1; i >= 0; i--) {
      Particle p = particles.get(i);
      p.run();
      if (p.isDead()) {
        particles.remove(i);
      }
    }
  }
}



// A simple Particle class

class Particle {
  PVector location;
  PVector velocity;
  PVector acceleration;
  float lifespan;
  color col;
  
  // ff5185
  // c1b257
  // 58585b
  // 36f2b8
  int[][] colors = {
    {255, 81, 133},
    {193, 178, 87},
    {88, 88, 91},
    {54, 242, 184}
  };
  
  Particle(PVector l) {
    acceleration = new PVector(1,1);
    velocity = new PVector(random(-1,1),random(-2,0));
    location = l.get();
    lifespan = 255.0;
    
    // set a color
    int colNum = round(random(0, 3));    
    col = color(colors[colNum][0], colors[colNum][1], colors[colNum][2]);
  }

  void run() {
    update();
    display();
  }

  // Method to update location
  void update() {
    velocity.add(acceleration);
    location.add(velocity);
    lifespan -= random(1, 50);
  }

  // Method to display
  void display() {
    stroke(col,lifespan);
    fill(col,lifespan);
    ellipse(location.x,location.y,sin(lifespan/2) * 20, cos(lifespan/2) * 15);
  }
  
  // Is the particle still useful?
  boolean isDead() {
    if (lifespan < 0.0) {
      return true;
    } else {
      return false;
    }
  }
}
