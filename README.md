# Apple

The folder imu_webserver_test contains the code that collects data from an imu and publishes it to a webserver.

To Use:
 - Make sure you have Telnet working or an equivalent software. How to enable Telnet in windows 10: https://social.technet.microsoft.com/wiki/contents/articles/38433.windows-10-enabling-telnet-client.aspx
 - Make sure arduino_secrets.h has the correct SSID and password for the wifi you are trying to connect over.
 - Once arduino is connected to wifi, connect via telnet: open command prompt, then run the command <b>Telnet "IP ADDRESS" 80</b>
 - Replace "IP ADDRESS" with the IP address that the arduino is connected to. This is obtained from running the testing code and reading the printed IP address from the serial monitor
 - Once connected, type through telnet the time (in ms) that you want the arduino to collect data for and the press "enter" to confirm that time. After pressing "enter" you should see the values of the IMU being printed to the terminal.
 
 You can also see the values of the IMU being printed to a website if you go to http://"IP ADDRESS"/
