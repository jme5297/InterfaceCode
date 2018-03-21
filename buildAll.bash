cd pwm
sudo pasm -b pru1.p
sudo pasm -b pru2.p
sudo gcc dutyCycleTest1.c -o dutyCycleTest1 -lpthread -lprussdrv
sudo gcc dutyCycleTest2.c -o dutyCycleTest2 -lpthread -lprussdrv
