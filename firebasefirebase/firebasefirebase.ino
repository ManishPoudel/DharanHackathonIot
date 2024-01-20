#include <FirebaseESP8266.h>
#include <Firebase.h>
#include <FirebaseFS.h>

/*Define the WiFi credentials */
// #define WIFI_SSID "iPhone"
// #define WIFI_PASSWORD "nopassword"
#define WIFI_SSID "Hackathone_worldlink_Wi-Fi"
#define WIFI_PASSWORD "Hackathon@123"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* Define the API Key */
#define API_KEY "AIzaSyAZQcMH27Hlq31iVGoXWJXilQ1pNO60P-s"

/*Define the RTDB URL */
#define DATABASE_URL "https://esp8266-data-transfer-default-rtdb.firebaseio.com/" 
//<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/*Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "poudelmanish321@gmail.com"
#define USER_PASSWORD "123456"


#define DHTPIN D2   // Define the pin to which the DHT sensor is connected
#define DHTTYPE DHT11 // Use DHT22 sensor, you can also use DHT11

// Libs for Dht sensors.
#include <DHT.h>
#include <DHT_U.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
DHT dht(DHTPIN, DHTTYPE);

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis =0;

unsigned long count = 0;
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
    yield();
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token 
  generation task */
  config.token_status_callback = tokenStatusCallback; 
  // see addons/TokenHelper.h

  // Comment or pass false value when WiFi reconnection will 
 // control by your code or third party library e.g. WiFiManager
 //Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer 
  //need to be set.
  // Large data transmission may require larger RX buffer, 
  //otherwise connection issue or data read time out can 
  //be occurred.
  //fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  Serial.println("hello from end of the setup");

  Firebase.begin(&config, &auth);
  //Firebase.setDoubleDigits(5);
  // for dht sensor
  dht.begin();
}

void loop() {
  
  //delay(2000);  // Delay between readings
  //yield();
  Serial.println("hello from Loop");
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 
      || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    Serial.printf("Set int... %s\n", Firebase.setFloat(fbdo, F("/test/humit"), humidity) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.setFloat(fbdo, F("/test/temp"), temperature) ? "ok" : fbdo.errorReason().c_str());
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  delay(1000);
  yield();
}
