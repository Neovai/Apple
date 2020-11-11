# Apple

The folder imu_webserver_test contains the code that collects data from an imu and publishes it to a webserver. 
To Use:
 - Make sure you have Telnet working or an equivalent software. How to enable Telnet in windows 10: https://social.technet.microsoft.com/wiki/contents/articles/38433.windows-10-enabling-telnet-client.aspx
 - Make sure arduino_secrets.h has the correct SSID and password for the wifi you are trying to connect over.
 - Once arduino is connected to wifi, connect via telnet: Open command prompt, then run the command <b>Telnet <IP ADDRESS> 80</b>
