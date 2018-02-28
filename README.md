# Wimos_Dimmer

This sketch is made to run on a Wemod D1 mini. 

I used IRF520 mos fet's but there is better mos fet's out there.

The Wemos connects to the specified WiFi and gets an IP from DHCP, I would advice settings a static IP in your DHCP server.

Then the Wemos starts a webserver on port 80.

The dimmers can be controled directly from a browser, via the following URL: http://{Wemos IP}/Dimmer_{Dimmer number}-{Strenght in %}
An example would be: http://192.168.0.111/Dimmer_1-75 this will set Dimmer 1 to 75%

App can be found at: https://play.google.com/store/apps/details?id=mbojer.wemosdimmer 

Hardware needed:
Android phone
Wireless Access point
Wemos D1 Mini - https://wiki.wemos.cc/products:d1:d1_mini
IRF520 mosfet - https://www.ebay.com/sch/i.html?_from=R40&_trksid=p2380057.m570.l1313.TR0.TRC0.H0.XIRF520+module.TRS0&_nkw=IRF520+module&_sacat=0
# Wemos_Relay
