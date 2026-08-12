// ============================================================
// VU מטר קולי - 6 נורות (2 ירוק, 2 צהוב, 2 אדום)
// אורט בראודה - הנדסת חשמל
// ============================================================
// מתחת ל-QUIET_LEVEL (עם שוליים מעל רעש הרקע הטבעי) - הכל כבוי.
// מעל זה - עולה בהדרגה (קפיצה מיידית למעלה, דעיכה איטית למטה),
// בדיוק כמו מד עוצמה אמיתי.
// ============================================================

#define SOUND_AO A0

// 6 הנורות בסדר עולה: ירוק, ירוק, צהוב, צהוב, אדום, אדום
int ledPins[6] = {2, 3, 4, 5, 6, 7};

// טווח כיול - לפי הטווח האמיתי המלא שגילינו: שקט~18, עד נשיפה חזקה 300-400+
// (הרכיב הזול הזה רגיש הרבה יותר ללחץ אוויר ישיר מאשר לקול רגיל ממרחק - מאפיין
// אמיתי שלו, לא תקלה - ראה תיעוד בפרויקט)
const int QUIET_LEVEL = 25;   // מתחת לזה - הכל כבוי (שוליים מעל 18 שראינו בשקט)
const int LOUD_LEVEL  = 1000; // כמעט קצה הסקאלה - כדי שגם נשיפה חלשה תזיז קצת,
                               // וצריך נשיפה ממש חזקה כדי להדליק את כל 6 הנורות

const float DECAY_RATE = 20.0; // מותאם לטווח הגדול - דעיכה מלאה תוך כשנייה

float displayLevel = QUIET_LEVEL; // רמה מוחלקת, מתעדכנת בהדרגה

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  Serial.begin(9600);
}

void loop() {
  int raw = analogRead(SOUND_AO);

  if (raw > displayLevel) {
    displayLevel = raw;          // קפיצה מיידית למעלה כשיש קול חזק
  } else {
    displayLevel -= DECAY_RATE;  // דעיכה איטית כשהקול נחלש
  }
  if (displayLevel < QUIET_LEVEL) displayLevel = QUIET_LEVEL; // לא לרדת מתחת לאזור השקט
  if (displayLevel > LOUD_LEVEL) displayLevel = LOUD_LEVEL;   // לא לעלות מעבר למה שצריך ל-6 נורות
  // בלי השורה האחרונה הזו, צעקה חזקה מאוד (למשל 900) הייתה "נתקעת" גבוה
  // ולוקחת עשרות שניות לרדת בחזרה, למרות שכל 6 הנורות כבר דלוקות מזמן.

  int numLedsOn = 0;
  if (displayLevel > QUIET_LEVEL) {
    numLedsOn = map((int)displayLevel, QUIET_LEVEL, LOUD_LEVEL, 0, 6);
    numLedsOn = constrain(numLedsOn, 0, 6);
  }

  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], (i < numLedsOn) ? HIGH : LOW);
  }

  Serial.print("גולמי: ");
  Serial.print(raw);
  Serial.print("   מוחלק: ");
  Serial.print(displayLevel);
  Serial.print("   נורות: ");
  Serial.println(numLedsOn);

  delay(20);
}
