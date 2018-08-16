Servo srv;

float currentTime=0;
int lastFeeding=0;

/*=======================================
    SETUP
=======================================*/
void setup()
{
    
    srv.attach(D0);    
    delay(50);
    srv.write(90);
    
    Particle.function("detach", detachServo);
    Particle.function("attach", attachServo);
    Particle.function("feed", justFeed);
    
    
}

/*=======================================
    MAIN LOOP
=======================================*/
void loop(){
    Particle.publish("Loop");
    if(shouldFeed()){
        dispenseFood();
    }
    delay(60000 * 5);
}

/*=======================================
    ATTACH / DETACH SERVO
=======================================*/
int detachServo(String args){
    srv.detach();
    Particle.publish("Servo Detached");
    return 1;
}

int attachServo(String args){
    srv.attach(D0);
    Particle.publish("Servo Attached");
    return 1;
}


/*=======================================
    JUST FEED - Web callable function
=======================================*/
int justFeed(String val){
    dispenseFood();
    return 1;
}



/*=======================================
    SHOULD FEED
=======================================*/
bool shouldFeed(){
    int feedingInt=(Time.day()*100)+Time.hour();
    int utcOffset=4;
    
    if(
        feedingInt>lastFeeding && 
        ( 
            Time.hour()==(7+utcOffset) ||
            Time.hour()==(9+utcOffset) ||
            Time.hour()==(12+utcOffset) ||
            Time.hour()==(16+utcOffset) ||
            Time.hour()==(18+utcOffset) ||
            Time.hour()==(20+utcOffset) ||
            Time.hour()==(23+utcOffset) 
            
        ) 
    ){
        lastFeeding=feedingInt;
        return true;
    }else{
        return false;
    }
}



/*=======================================
    DISPENSE FOOD
=======================================*/

int dispenseFood(){
    int pos=0;
    int startPos=90;
    int speed=1;
    int servoMaxPos=150;
    int servoMinPos=50;
    
    attachServo("");
    delay(500);
    
    for(pos = startPos; pos <= servoMaxPos; pos += speed){
        srv.write(pos);
        delay(7);
    }
    
    delay(500);
    
    for(pos = servoMaxPos; pos >=servoMinPos; pos -= speed){
        srv.write(pos);
        delay(7);
    }
    
    delay(500);
    
    for(pos = servoMinPos; pos <=startPos; pos += speed){
        srv.write(pos);
        delay(7);
    }
    
    delay(50);
    
    detachServo("");
    
    Particle.publish("Feed");
    
    
    
    return 1;

}
