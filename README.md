# Device-Driver

## Description
In this program we develop a skeleton device driver that can be loaded and run in Linux. Then we add some minor functionality to the device driver such as the user/application passing in a value to the device driver and the device driver returns that nth word from a file. In my case, I build a program that returns the nth number from a Fibonacci sequence of numbers. It is written in C and is valid and loadable device driver. Methods such as open, release, read, write and two ioctl commands are included in the driver along with a method to calculate the nth digit in the Fibonacci sequence of numbers. The user program takes an input of a number which is the digit of the Fibonacci series that the user wants in the form of a string and then sends that string using write to the kernel mod, which then stores the string. The user program then retrieves the message from the kernel mod using read and displays the message back to the user in terminal. The string is then converted to integer and an ioctl call for write is used to send the integer to the kernel mod. Then the user program makes an ioctl read call which takes the stored integer in kernel mod. This sends it to the Fibonacci method that sends the calculated result back to the user program. Finally the user program displays the asked nth Fibonacci number.


## Steps to run:

1. Go to the Module/ folder and run ‘make’

2. Then type ‘sudo insmod kernelMod.ko’

3. Then go to Test/ folder and run ‘make'

4. Then run ‘sudo make run’ to run the user program


# Please refer to WriteUp for more information.
