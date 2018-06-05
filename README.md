######################
# Build instructions #
######################
GCC is needed for the build
If GNU make is available in your environment, simply run:
$>make
$>make clean

Alternatively, run the following:
$>gcc -c common.c ringbuffer.c && gcc -O2 -Wall -Wextra -Werror -o app common.o ringbuffer.o app.c -lpthread

After the build, you'll see a binary called: app

################
# Test the APP #
################
$>./app

By default, the random chars created by thread one will be shown in the console (stdout);
thread two will write random number of them to file: /tmp/app_output.txt

You could save or redirect the session output to a file (the source data), and compare it with app_output.txt

#######################
# More debug/settings #
#######################
Need to modify the Macro(s) in common.h
The Ring buffer debug is disabled by default.