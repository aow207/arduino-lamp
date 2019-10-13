// LED setup - only some pins provide 8-bit PWM (Pulse-width modulation)
// output with the analogWrite() function.
// http://www.arduino.cc/en/Main/ArduinoBoardDuemilanove
// PWM: 3,5,6,9,10,11
#defineredPin    (3)
#definegreenPin (5)
#definebluePin   (6)
 
// delay in ms between fade updates
// max fade time = 255 * 15 = 3.825s
#definefadeDelay (15)
 
// Wifi setup
#definenetwork ([your network])
#definepassword ([your network password])
#defineremoteServer ("twitter.com")
 
constchar* moodNames[NUM_MOOD_TYPES] = {
 "love",
 "joy",
 "surprise",
 "anger",
 "envy",
 "sadness",
 "fear",
};
 
constchar* moodIntensityNames[NUM_MOOD_INTENSITY] = {
 "mild",
 "considerable",
 "extreme",
};
 
// the long term ratios between tweets with emotional content
// as discovered by using the below search terms over a period of time.
floattempramentRatios[NUM_MOOD_TYPES] = {
 0.13f,
 0.15f,
 0.20f,
 0.14f,
 0.16f,
 0.12f,
 0.10f,
};
 
// these numbers can be tweaked to get the system to be more or less reactive
// to be more or less susceptible to noise or short term emotional blips, like sport results
// or bigger events, like world disasters
#define emotionSmoothingFactor (0.1f)
#define moodSmoothingFactor (0.05f)
#define moderateMoodThreshold (2.0f)
#define extremeMoodThreshold (4.0f)
 
// save battery, put the wifly to sleep for this long between searches (in ms)
#defineSLEEP_TIME_BETWEEN_SEARCHES (1000 * 5)
 
// Store search strings in flash (program) memory instead of SRAM.
// http://www.arduino.cc/en/Reference/PROGMEM
// edit TWEETS_PER_PAGE if changing the rpp value
prog_charstring_0[] PROGMEM = "GET /search.json?q=\"i+love+you\"+OR+\"i+love+her\"+OR+\"i+love+him\"+OR+\"all+my+love\"+OR+\"i'm+in+love\"+OR+\"i+really+love\"&rpp=30&result_type=recent";
prog_charstring_1[] PROGMEM = "GET /search.json?q=\"happiest\"+OR+\"so+happy\"+OR+\"so+excited\"+OR+\"i'm+happy\"+OR+\"woot\"+OR+\"w00t\"&rpp=30&result_type=recent";
prog_charstring_2[] PROGMEM = "GET /search.json?q=\"wow\"+OR+\"O_o\"+OR+\"can't+believe\"+OR+\"wtf\"+OR+\"unbelievable\"&rpp=30&result_type=recent";
prog_charstring_3[] PROGMEM = "GET /search.json?q=\"i+hate\"+OR+\"really+angry\"+OR+\"i+am+mad\"+OR+\"really+hate\"+OR+\"so+angry\"&rpp=30&result_type=recent";
prog_charstring_4[] PROGMEM = "GET /search.json?q=\"i+wish+i\"+OR+\"i'm+envious\"+OR+ \"i'm+jealous\"+OR+\"i+want+to+be\"+OR+\"why+can't+i\"+&rpp=30&result_type=recent";
prog_charstring_5[] PROGMEM = "GET /search.json?q=\"i'm+so+sad\"+OR+\"i'm+heartbroken\"+OR+\"i'm+so+upset\"+OR+\"i'm+depressed\"+OR+\"i+can't+stop+crying\"&rpp=30&result_type=recent";
prog_charstring_6[] PROGMEM = "GET /search.json?q=\"i'm+so+scared\"+OR+\"i'm+really+scared\"+OR+\"i'm+terrified\"+OR+\"i'm+really+afraid\"+OR+\"so+scared+i\"&rpp=30&result_type=recent";
 
// be sure to change this if you edit the rpp value above
#defineTWEETS_PER_PAGE (30)
 
PROGMEMconstchar *searchStrings[] =        
{  
 string_0,
 string_1,
 string_2,
 string_3,
 string_4,
 string_5,
 string_6,
};
 
voidsetup()
{
 Serial.begin(9600);
 delay(100);
}
 
voidloop()
{
 // create and initialise the subsystems 
 WiFlywifly(network, password, SLEEP_TIME_BETWEEN_SEARCHES, Serial);
 WorldMoodworldMood(Serial, emotionSmoothingFactor, moodSmoothingFactor, moderateMoodThreshold, extremeMoodThreshold, tempramentRatios);
 LEDled(Serial, redPin, greenPin, bluePin, fadeDelay);
 TwitterParsertwitterSearchParser(Serial, TWEETS_PER_PAGE);
 
 wifly.Reset();
 
 charsearchString[160];
 
 while (true)
 {
    for (inti = 0; i < NUM_MOOD_TYPES; i++)
    {
      twitterSearchParser.Reset();
 
      // read in new search string to SRAM from flash memory
      strcpy_P(searchString, (char*)pgm_read_word(&(searchStrings[i])));
 
      boolok = false;
      intretries = 0;
 
      // some recovery code if the web request fails
      while (!ok)
      {
        ok = wifly.HttpWebRequest(remoteServer, searchString, &twitterSearchParser);
 
        if (!ok)
        {
          Serial.println("HttpWebRequest failed");
 
          retries++;
          if (retries > 3)
          {
            wifly.Reset();
            retries = 0;
          }
        }
      }
 
      floattweetsPerMinute = twitterSearchParser.GetTweetsPerMinute();
 
      // debug code
      Serial.println("");
      Serial.print(moodNames[i]);
      Serial.print(": tweets per min = ");
      Serial.println(tweetsPerMinute);
 
      worldMood.RegisterTweets(i, tweetsPerMinute);
    }
 
    MOOD_TYPEnewMood = worldMood.ComputeCurrentMood();
    MOOD_INTENSITYnewMoodIntensity = worldMood.ComputeCurrentMoodIntensity();
 
    Serial.print("The Mood of the World is ... ");
    Serial.print(moodIntensityNames[(int)newMoodIntensity]);
    Serial.print(" ");
    Serial.println(moodNames[(int)newMood]);
 
    led.SetColor((int)newMood, (int)newMoodIntensity);
 
    // save the battery
    wifly.Sleep();
 
    // wait until it is time for the next update
    delay(SLEEP_TIME_BETWEEN_SEARCHES);
 
    Serial.println("");
 }
}
