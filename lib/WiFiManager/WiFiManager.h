/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   (https://github.com/esp8266/Arduino) to enable easy
   configuration and reconfiguration of WiFi credentials using a Captive Portal
   inspired by:
   http://www.esp8266.com/viewtopic.php?f=29&t=2520
   https://github.com/chriscook8/esp-arduino-apboot
   https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced
   Built by AlexT https://github.com/tzapu
   Licensed under MIT license
 **************************************************************/

#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <memory>

extern "C" {
  #include "user_interface.h"
}

const char HTTP_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\"/><title>ITEasy Button Network Setup| {v}</title>";
const char HTTP_STYLE[] PROGMEM           = "<style> .c a,.ql:after{display:inline-block}body{background:#122433;color:#fff;font-family:sans-serif;text-transform:uppercase}.wp{max-width:300px;margin:0 auto;text-align:center}.c a{border:2px solid#fff;padding:.6em;border-radius:4px;margin:1em 0} .ls{margin:.5em;text-align:left;text-transform:none!important} .ql:after{background:url(data:image/svg+xml;base64,PHN2ZyBmaWxsPSIjQjUzNTM1IiB4bWxuczp4PSJodHRwOi8vbnMuYWRvYmUuY29tL0V4dGVuc2liaWxpdHkvMS4wLyIgeG1sbnM6aT0iaHR0cDovL25zLmFkb2JlLmNvbS9BZG9iZUlsbHVzdHJhdG9yLzEwLjAvIiB4bWxuczpncmFwaD0iaHR0cDovL25zLmFkb2JlLmNvbS9HcmFwaHMvMS4wLyIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxuczp4bGluaz0iaHR0cDovL3d3dy53My5vcmcvMTk5OS94bGluayIgdmVyc2lvbj0iMS4xIiB4PSIwcHgiIHk9IjBweCIgdmlld0JveD0iMCAwIDEwMCAxMDAiIGVuYWJsZS1iYWNrZ3JvdW5kPSJuZXcgMCAwIDEwMCAxMDAiIHhtbDpzcGFjZT0icHJlc2VydmUiPjxzd2l0Y2g+PGZvcmVpZ25PYmplY3QgcmVxdWlyZWRFeHRlbnNpb25zPSJodHRwOi8vbnMuYWRvYmUuY29tL0Fkb2JlSWxsdXN0cmF0b3IvMTAuMC8iIHg9IjAiIHk9IjAiIHdpZHRoPSIxIiBoZWlnaHQ9IjEiLz48ZyBpOmV4dHJhbmVvdXM9InNlbGYiPjxwYXRoIGQ9Ik04OCwzOWgtNHYtNi44ODRDODQsMTQuMjk2LDY3LjgyLTAuMTQ2LDUwLTAuMTQ2UzE2LDE0LjI5NiwxNiwzMi4xMTZWMzloLTRjLTIuMiwwLTQsMS44LTQsNHYzOS42MDcgICAgQzgsOTEuNTY0LDE3LjY5NSwxMDAsMjYuNjYxLDEwMGg0OC4zOTNDODQuMDIsMTAwLDkyLDkxLjU2NCw5Miw4Mi42MDdWNDNDOTIsNDAuOCw5MC4yLDM5LDg4LDM5eiBNMzEsMzIuMTE2ICAgIGMwLTEwLjU0Myw4LjQ1Ny0xOS4wOTIsMTktMTkuMDkyczE5LDguNTQ5LDE5LDE5LjA5MlYzOUgzMVYzMi4xMTZ6Ii8+PC9nPjwvc3dpdGNoPjwvc3ZnPg==);content:\"\";position:relative;width:1em;height:1em;left:.5em}.q,.ql{color:#0ac80a}button,button[type=submit],input{background:0 0;border:1px solid#fff;color:#fff;border-radius:5px;display:inline-block;width:100%;padding:1em;margin-bottom:1em;text-transform:uppercase}svg{width:6em}a{color:#fff;text-decoration:none}a:hover{text-decoration:underline}#ship{animation:shake.5s.5s linear forwards infinite;transform-origin:center center;-webkit-transform-style:preserve-3d}@keyframes shake{0%,100%{transform:rotate(0)}33.33333%{transform:rotate(1deg)translateY(-.15em)}66.66666%{transform:rotate(-1deg)translateY(-.15em)}}#s,#p{text-transform:none}</style>";
const char HTTP_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();} </script>";
const char HTTP_HEAD_END[] PROGMEM        = "</head><body><div class=\"wp\"><svg xmlns=\"http://www.w3.org/2000/svg\"viewBox=\"0 0 234.7 234.7\"><style>.st0{fill:#2F4756;} .st1{opacity:0.5;fill:#0E191E;} .st2{fill:#B7B69A;} .st3{fill:#B53535;} .st4{fill:#898989;} .st5{fill:#EDEDEB;} .st6{fill:#D8D8D8;} .st7{fill:#7C7C7C;} .st8{fill:#708C9B;} .st9{fill:#186E93;}</style><circle class=\"st0\"cx=\"117.4\"cy=\"117.4\"r=\"117.4\"id=\"Circle\"/><path class=\"st1\"d=\"M134.5 34S81.4 144.7 58.9 206.8L81.1 229c11.4 3.7 23.6 5.7 36.2 5.7 59.8 0 109.1-44.7 116.4-102.5L134.5 34z\"id=\"Shadow\"/><g id=\"Stars\"><circle class=\"st2\"cx=\"37.6\"cy=\"83.7\"r=\"7.8\"/><circle class=\"st2\"cx=\"50.8\"cy=\"73.5\"r=\"2.7\"/><circle class=\"st2\"cx=\"51.6\"cy=\"48.4\"r=\"8.5\"/><circle class=\"st2\"cx=\"177.5\"cy=\"36.2\"r=\"2.2\"/><circle class=\"st2\"cx=\"175.5\"cy=\"44.2\"r=\"1.9\"/><circle class=\"st2\"cx=\"187.7\"cy=\"52.1\"r=\"4.4\"/><circle class=\"st2\"cx=\"19.5\"cy=\"117.6\"r=\"2.1\"/><circle class=\"st2\"cx=\"29.5\"cy=\"125.6\"r=\"4.4\"/><circle class=\"st2\"cx=\"185.8\"cy=\"123.3\"r=\"6.1\"/><circle class=\"st2\"cx=\"192.6\"cy=\"135.4\"r=\"2.4\"/></g><g id=\"ship\"><path class=\"st3\"d=\"M91.1 170.9c-3.2 10.7-31.7 34.8-31.7 34.8s-10.4-35.8-7.2-46.6c3.2-10.7 14.6-16.8 25.3-13.6s16.9 14.6 13.6 25.4zM140.1 172.4c3.2 10.7 31.7 34.8 31.7 34.8s10.4-35.8 7.2-46.6c-3.2-10.7-14.6-16.8-25.3-13.6-10.7 3.3-16.8 14.7-13.6 25.4z\"/><path class=\"st4\"d=\"M147.7 188.6H82c-2.6 0-4.8-2.1-4.8-4.8v-9c0-2.6 2.1-4.8 4.8-4.8h65.7c2.6 0 4.8 2.1 4.8 4.8v9c0 2.6-2.2 4.8-4.8 4.8z\"/><path class=\"st5\"d=\"M58.9 111.6c0 21.2 1.5 50.2 8.2 65.8l46.6.8v-162s-54.8 34.1-54.8 95.4z\"/><path class=\"st6\"d=\"M168.5 111.6c0 21.2-1.5 50.2-8.2 65.8l-46.6.8v-162s54.8 36.1 54.8 95.4z\"/><path class=\"st5\"d=\"M58.9 111.6c0 21.2 1.5 50.2 8.2 65.8l46.6.8v-162s-54.8 34.1-54.8 95.4z\"/><path class=\"st6\"d=\"M168.5 111.6c0 21.2-1.5 50.2-8.2 65.8l-46.6.8v-162s54.8 36.1 54.8 95.4z\"/><path class=\"st3\"d=\"M121.2 168.2c0 11.2-7.5 42.5-7.5 42.5s-7.5-31.3-7.5-42.5 3.4-20.3 7.5-20.3 7.5 9.1 7.5 20.3zM113.9 68.9c16 0 30.6-2.9 41.4-7.5-16.2-29.2-41.4-45.7-41.4-45.7S88.3 31.5 72.1 61.2c10.8 4.8 25.6 7.7 41.8 7.7z\"/><circle class=\"st7\"cx=\"113.7\"cy=\"75.9\"r=\"25.6\"/><circle class=\"st8\"cx=\"113.7\"cy=\"75.9\"r=\"18.9\"/><circle class=\"st8\"cx=\"113.7\"cy=\"75.9\"r=\"18.9\"/><circle class=\"st8\"cx=\"113.7\"cy=\"75.9\"r=\"18.9\"/><path class=\"st9\"d=\"M116.8 62.5c-1.3-.1-2.6-.1-3.8.1 6.1 1.2 10.4 6.8 9.7 12.9-.7 6.5-4 9.8-13.3 10.5-3.9-.4-7.1-2.6-9-5.6 1.3 6.1 6.6 11 13.3 11.7 8.4.9 15.8-5 16.7-13.2.8-8.2-5.3-15.5-13.6-16.4z\"/></g></svg>";
const char HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\"method=\"get\"><button>Scan WiFi</button></form><br/><form action=\"/0wifi\"method=\"get\"><button>Configure WiFi (No Scan)</button></form><br/><form action=\"/r\"method=\"post\"><button>Reset</button></form>";
const char HTTP_ITEM[] PROGMEM            = "<div class=\"ls\"><a href='#p'onclick='c(this)'>{v}</a>&nbsp;<span class='q{i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<form method='get'action='wifisave'><input id='s'name='s'placeholder='SSID'><br/><input id='p'name='p'type='password'placeholder='password'><br/>";
const char HTTP_FORM_PARAM[] PROGMEM      = "<input id='{i}'name='{n}'length='{l}'placeholder='{p}'value='{v}' {c}>";
const char HTTP_FORM_END[] PROGMEM        = "<button type='submit'>Save</button></form>";
const char HTTP_SCAN_LINK[] PROGMEM       = "<div class=\"c\"><a href=\"/wifi\">Scan for Networks</a></div>";
const char HTTP_SAVED[] PROGMEM           = "<div>Credentials Saved!<br/><br/>Trying to connect ITEasy Button to network.<br/><br/>If it fails reconnect to the ITEasy Button to try again</div>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";



#define WIFI_MANAGER_MAX_PARAMS 10

class WiFiManagerParameter {
  public:
    WiFiManagerParameter(const char *custom);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    const char *getID();
    const char *getValue();
    const char *getPlaceholder();
    int         getValueLength();
    const char *getCustomHTML();
  private:
    const char *_id;
    const char *_placeholder;
    char       *_value;
    int         _length;
    const char *_customHTML;

    void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    friend class WiFiManager;
};


class WiFiManager
{
  public:
    WiFiManager();

    boolean       autoConnect();
    boolean       autoConnect(char const *apName, char const *apPassword = NULL);

    //if you want to always start the config portal, without trying to connect first
    boolean       startConfigPortal();
    boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();

    void          resetSettings();

    //sets timeout before webserver loop ends and exits even if there has been no setup.
    //useful for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(unsigned long seconds);
    void          setTimeout(unsigned long seconds);

    //sets timeout for which to attempt connecting, useful if you get a lot of failed connects
    void          setConnectTimeout(unsigned long seconds);


    void          setDebugOutput(boolean debug);
    //defaults to not showing anything under 8% signal quality if called
    void          setMinimumSignalQuality(int quality = 8);
    //sets a custom ip /gateway /subnet configuration
    void          setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //sets config for a static IP
    void          setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //called when AP mode and config portal is started
    void          setAPCallback( void (*func)(WiFiManager*) );
    //called when settings have been changed and connection was successful
    void          setSaveConfigCallback( void (*func)(void) );
    //adds a custom parameter
    void          addParameter(WiFiManagerParameter *p);
    //if this is set, it will exit after config, even if connection is unsuccessful.
    void          setBreakAfterConfig(boolean shouldBreak);
    //if this is set, try WPS setup when starting (this will delay config portal for up to 2 mins)
    //TODO
    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(boolean removeDuplicates);

  private:
    std::unique_ptr<DNSServer>        dnsServer;
    std::unique_ptr<ESP8266WebServer> server;

    //const int     WM_DONE                 = 0;
    //const int     WM_WAIT                 = 10;

    //const String  HTTP_HEAD = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";

    void          setupConfigPortal();
    void          startWPS();

    const char*   _apName                 = "no-net";
    const char*   _apPassword             = NULL;
    String        _ssid                   = "";
    String        _pass                   = "";
    unsigned long _configPortalTimeout    = 0;
    unsigned long _connectTimeout         = 0;
    unsigned long _configPortalStart      = 0;

    IPAddress     _ap_static_ip;
    IPAddress     _ap_static_gw;
    IPAddress     _ap_static_sn;
    IPAddress     _sta_static_ip;
    IPAddress     _sta_static_gw;
    IPAddress     _sta_static_sn;

    int           _paramsCount            = 0;
    int           _minimumQuality         = -1;
    boolean       _removeDuplicateAPs     = true;
    boolean       _shouldBreakAfterConfig = false;
    boolean       _tryWPS                 = false;

    const char*   _customHeadElement      = "";

    //String        getEEPROMString(int start, int len);
    //void          setEEPROMString(int start, int len, String string);

    int           status = WL_IDLE_STATUS;
    int           connectWifi(String ssid, String pass);
    uint8_t       waitForConnectResult();

    void          handleRoot();
    void          handleWifi(boolean scan);
    void          handleWifiSave();
    void          handleInfo();
    void          handleReset();
    void          handleNotFound();
    void          handle204();
    boolean       captivePortal();
    boolean       configPortalHasTimeout();

    // DNS server
    const byte    DNS_PORT = 53;

    //helpers
    int           getRSSIasQuality(int RSSI);
    boolean       isIp(String str);
    String        toStringIp(IPAddress ip);

    boolean       connect;
    boolean       _debug = true;

    void (*_apcallback)(WiFiManager*) = NULL;
    void (*_savecallback)(void) = NULL;

    WiFiManagerParameter* _params[WIFI_MANAGER_MAX_PARAMS];

    template <typename Generic>
    void          DEBUG_WM(Generic text);

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(  obj->fromString(s)  ) {
      return  obj->fromString(s);
    }
    auto optionalIPFromString(...) -> bool {
      DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
    }
};

#endif
