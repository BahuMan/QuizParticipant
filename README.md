# Quiz participant

code for Fri3dBadge 2024

# MQTT broker

[Mosquitto](https://mosquitto.org/) is opensource and free.
To install on OpenSuse Leap:
1. `sudo zypper in mosquitto`
2. create custom pasword file with mosquitto pwd tool, e.g. in /etc/mosquitto/conf.d
3. create custom .conf file in /etc/mosquitto/conf.d specifying port and custom password file
3. `sudo service mosquitto start`
4. `sudo zypper in mosquitto-clients`
5. to see incoming messages: `mosquitto_sub -h localhost -t hello/# -u usernae -P password`
