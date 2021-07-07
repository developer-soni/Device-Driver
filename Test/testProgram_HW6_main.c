/**************************************************************
*
* File: testProgram_HW6_main.c
*
* Description: This program uses the Linux Kernel Module
* kernelMod to get nth number in Fibonacci Series
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define BUFFER 256
#define RD_VALUE _IOR('a', 'b', int)
#define WR_VALUE _IOW('a', 'a', int)

static char receive[BUFFER];

// Function for suffix of number
char *suffix(int n)
{
   switch (n % 100)
   {
   case 11:
   case 12:
   case 13:
      return "th";
   default:
      switch (n % 10)
      {
      case 1:
         return "st";
      case 2:
         return "nd";
      case 3:
         return "rd";
      default:
         return "th";
      }
   }
}

int main()
{
   int ret, fd;
   char stringPass[BUFFER];
   int val, num;

   printf("***Start***\n");

   // Opening the device
   fd = open("/dev/kernelmod", O_RDWR);
   if (fd < 0)
   {
      perror("ERROR: Failed to open device.");
      return errno;
   }

   printf("Get nth number in Fibonacci Series\n");
   printf("Enter a positive integer: ");
   scanf("%s", stringPass);

   // Sending string to kernel mod
   ret = write(fd, stringPass, strlen(stringPass));
   if (ret < 0)
   {
      perror("ERROR: Failed to write to device.");
      return errno;
   }

   // Reading the response from kernel mod
   ret = read(fd, receive, BUFFER);
   if (ret < 0)
   {
      perror("ERROR: Failed to read from device.");
      return errno;
   }

   printf("Your value: %s\n", receive);
   num = atoi(receive);

   if (num == 0)
   {
      printf("ERROR: Invalid entry of '%s'.\n", receive);
      printf("Try again.\n");
      printf("***End Program***\n");
      close(fd);
      return 0;
   }

   // Sending int to kernel mod
   ret = ioctl(fd, WR_VALUE, &num);
   if (ret < 0)
   {
      perror("ERROR: Failed to write to the device.");
      return errno;
   }

   // Reading Fibonacci value from kernel mod
   ret = ioctl(fd, RD_VALUE, &val);
   if (ret < 0)
   {
      perror("ERROR: Failed to read from the device.");
      return errno;
   }

   // Output
   printf("The %d%s number in Fibonacci Series is: %d\n", num, suffix(num), val);

   //cleanup
   close(fd);
   return 0;
}