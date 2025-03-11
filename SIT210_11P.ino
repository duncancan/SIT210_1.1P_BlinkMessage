// Global variables
char* message = "Duncan Ross McMurtrie";

// Variables I only want to declare once. One school of thought is that these should be encapsulated in their relevant functions, but this
// would cause them being instantiated each time the function is called. I am trading security for speed here, which seems appropriate for
// an offline embedded system.
int rhythm_unit = 125;
// Source for timings: https://en.wikipedia.org/wiki/Morse_code
const int DOT_TIME = rhythm_unit * 1;           // In Morse code, the correct rhythm is a "dot" (short sound) lasting one unit of time
const int DASH_TIME = rhythm_unit * 3;          // A "dash" (long sound) lasts three units of time
const int OFF_TIME = rhythm_unit * 1;           // Each dot or dash within an encoded character is followed by a period of signal absence equal to the dot duration.
const int INTER_LETTER_TIME = rhythm_unit * 1;  // Spaces between letters are  one unit long
const int INTER_WORD_TIME = rhythm_unit * 7;    // Spaces between words being seven units long.
// Our morse alphabet; 26 4-character strings. 0th element is A, 25th element is Z. Some wasted string space for characters less than 4 chars.
const char morse_alphabet[26][5] = {".-",	"-...",	"-.-.",	"-..",	".", "..-.",	"--.",	"....",	"..",	".---",	"-.-",	".-..",	"--",
                                  "-.",	"---",	".--.",	"--.-",	".-.",	"...",	"-", "..-",	"...-",	".--",	"-..-",	"-.--",	"--.."};
// Offsets to translate our ASCII code into the correct array index
const int ASCIIOFFSET = 65;
const int UPPERCASEEND = 90;
const int LOWERCASEOFFSET = 32;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600); // Set up our serial port to print to it for debugging purposes.
  Serial.println();
  Serial.println();
}

void loop()
{
  morse_message(message);
}

const char* morse_encode(char c) {  
  // Convert our character into its array index
  int i = c;
  if (i > UPPERCASEEND) i -= LOWERCASEOFFSET;
  i -= ASCIIOFFSET;
  
  // Print it to the serial port, for debugging purposes
  Serial.print("The index derived for character ");
  Serial.print(c);
  Serial.print(" is ");
  Serial.print(i);
  Serial.print(", which corresponds to morse code ");
  Serial.println(morse_alphabet[i]);
  
  return morse_alphabet[i];
  
}

void timed_blink(int on_time, int off_time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(on_time);
  digitalWrite(LED_BUILTIN, LOW);
  delay(off_time);
}

void blink_morse(char* letter_code) {
  // Iterate over each dot and dash of the letter
  for (int i = 0; i < strlen(letter_code); i++) {
    if (letter_code[i] == '.')  {
      timed_blink(DOT_TIME, OFF_TIME);
    }
    else if (letter_code[i] == '-') {
      timed_blink(DASH_TIME, OFF_TIME);
    }
    // Don't blink anything if it's an invalid character.
    else {
      return;
    } 

  }
  // And at the end, insert a space between letters
  delay(INTER_LETTER_TIME);
  
}

// Takes a message as an array of characters, and iterates over those characters, printing each as morse code
void morse_message(char message[]) {
  // Loop through our message
  for (int i = 0; i < strlen(message); i++) {
    // Convert each character of the message to its morse code equivalent,
    char c = message[i];
    if (c == ' ') {           // If it's a space
      delay(INTER_WORD_TIME); // Pause for that long without blinking
      Serial.println();       // Insert a line between words in our serial port too
      continue;               // And go to the next character instead of trying to encode as morse code.
    }
    char* letter_code = morse_encode(c);
    blink_morse(letter_code);
  }

  // Insert an inter-word-length delay between the end of our message and its repeat
  delay(INTER_WORD_TIME);
  // Print a new line to the serial port. This just makes our debugging output a bit easier to read
  Serial.println();
}
