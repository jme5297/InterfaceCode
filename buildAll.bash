echo Compiling PWM code...
cd pwm
sudo pasm -b pru1.p
sudo pasm -b pru2.p
sudo gcc frequencyTest1.c -o frequencyTest1 -lpthread -lprussdrv
sudo gcc frequencyTest2.c -o frequencyTest2 -lpthread -lprussdrv
sudo gcc pwm_disabler1.c -o pwm_disabler1 -lpthread -lprussdrv
sudo gcc pwm_disabler2.c -o pwm_disabler2 -lpthread -lprussdrv
cd ..
echo Compiling Interface code...
sudo g++ gps.cpp -o gps
sudo g++ laser.cpp -o laser

