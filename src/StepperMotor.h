
// int rotateSpeed = 1;
// int maxRotateSpeed = 20;
// int minRotateSpeed = 1;

// void stepperMotorControl()
// /* -----------------------
// 1 rev = 360 degrees
// Use 1/N steps resolution
// 1 full step = 1.8 degrees
// 1 step = 1.8/N degrees
// For gear ratio 1:60
// 1 step(pulse) = (1/N)/60 degrees = 1/(60N) degrees

// 24hr 360degrees ;
// 1hr 15 degrees;
// 1mins = 0.25 degrees;
// 1s = 0.25/60 degrees;
// 1/1000s = 0.25/6000 degrees;
// 1/1000s = (0.25/6000)/( 1/(60N) ) steps = 0.64 steps
// 1.5625 (1/1000)s = 1 steps
// */
// {
//     digitalWrite(Step_Pin, HIGH);
//     delayMicroseconds(10); //control the speed
//     digitalWrite(Step_Pin, LOW);
//     delayMicroseconds(10);
// }