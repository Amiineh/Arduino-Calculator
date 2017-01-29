
  volatile int a;
  volatile int b;
  String string;

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  Serial.print("please enter first number \n");
  while (Serial.available() <= 0){
    }
  if(Serial.available() > 0) {
    a = Serial.parseInt(); // get first number
    Serial.print(a);
    Serial.print("\n");
  }
  Serial.print("please type your instruction\n");
    while (Serial.available() <= 0){
    }
    if(Serial.available() > 0) {
    string = Serial.readString(); // get instruction
        Serial.print(string);
    Serial.print("\n");
  }
    Serial.print("please enter second number\n");
  while (Serial.available() <= 0){
    }
  if(Serial.available() > 0) {
    b = Serial.parseInt(); // get second instruction
        Serial.print(b);
    Serial.print("\n");
  }
    if ( string == "+"){
     asm("AddIns: \n"
    "add %0 , %1 \n": "+r"(a) : "r" (b));
    Serial.print("answer = ");
    Serial.println(a);
    Serial.println("----------------------------------------------");
  }
  else if (string== "-"){
    asm("SubIns: \n"
    "sub %0 , %1 \n": "+r"(a) : "r" (b)); 
    Serial.print("answer = ");
    Serial.println(a);
    Serial.println("----------------------------------------------");
  }
  else if ( string == "*"){
    asm("MulIns: \n"
    "mul %0, %1 \n"
    "mov %0, r0 \n" : "+r"(a) : "r" (b) : "r0" , "r1");
    Serial.print("answer = ");
    Serial.println(a);
    Serial.println("----------------------------------------------");    
  }
  else if ( string == "/"){
    asm("divIns: \n"
    "ldi r16 , 0 \n" // r16 is answer
    "divLoop: \n"
    "cp %0 , %1 \n" // if a < b
    "brlo exitOfDiv \n" 
    "inc r16 \n" // r16 += 1
    "sub %0 , %1 \n" // a = a - b
    "rjmp divLoop \n"
    "exitOfDiv: \n"
    "mov %1 , r16 \n" : "+r"(a) , "+r"(b)  :  : "r16");
    Serial.print("answer = ");
    Serial.println(b);
    Serial.print("remaining = ");
    Serial.println(a);
    Serial.println("----------------------------------------------");    
  }
  else if ( string == "^"){
    asm("PowerIns: \n"
    "ldi r16 , 1 \n" // use mul instruction
    "powerLoop: \n"
    "cpi %1 , 1 \n" // while b >= 1
    "brlo exitOfPower \n"
    "mul r16 , %0 \n" // answer = answer * a
    "mov r16 , r0 \n"
    "dec %1 \n"
    "rjmp powerLoop \n"
    "exitOfPower: \n"
    "mov %0 , r16 " : "+r"(a) , "+r"(b) : : "r16");
    Serial.print("answer = ");
    Serial.println(a);
    Serial.println("----------------------------------------------");    
  }
   else if ( string == "Log" || string == "log" ){
    asm("LogIns: \n"
    "ldi r17 , 0 \n"             //r17 is answer
    "mov r18 , %0 \n"            //a is in r18
    "mov r19 , %1 \n"            //b is in r19
    "LogLoop: \n"
    "mov %0 , r19 \n"
    "mov %1 , r17 \n"
                              // r16 is temporary regsiter for calc a^i
    "ldi r16 , 1 \n"
    "powerLoop2: \n"
    "cpi %1 , 1 \n"
    "brlo exitOfPower2 \n"
    "mul r16 , %0 \n"
    "mov r16 , r0 \n"
    "dec %1 \n"
    "rjmp powerLoop2 \n"
    "exitOfPower2: \n"
    "mov %0 , r16  \n"
    "cp 0 , r1 \n"
    "brlo exitOfLog \n"
    "cp r18 , %0 \n"          // if b^i > a then go to exit
    "brlo exitOfLog \n"
    "inc r17 \n" // increment i
    "rjmp LogLoop \n"
    "exitOfLog: \n"
    "mov %0 , r17 \n"         // a = r17 - 1. beacuse b^r17 > a. so Log_b(a) = r17 -1
    "dec %0 \n" 
    : "+r"(a) , "+r"(b) : : "r16" , "r17" , "r18" , "r19" );
    Serial.print("answer = ");
    Serial.println(a);
    Serial.println("----------------------------------------------");    
  }
   else if ( string == "GCD" || string == "gcd"){
    asm("GCD: \n"
    "cp %0, %1 \n"
    "breq exitOfGCD \n"   //if equal, then a is the answer
    "mov r16, %0 \n"      //we want to put the smaller number in r16
    "cp %0, %1 \n"
    "brlt GCDLoop \n"     //if a < b we're good to go, else put b in r16
    "mov r16, %1 \n"
    
    "GCDLoop: \n"
    "cpi r16 , 1 \n"     //if r16 == 1 then jump out
    "breq exitOfGCD \n"
    
    "mov r17, %1 \n"
    "forB: \n"           //check if b is dividable by the new r16
    "cpi r17, 1 \n"        //if b < 1 check if dividable
    "brlt checkB \n"
    "sub r17, r16 \n"
    "rjmp forB \n"
    
    "checkB: \n"
    "cpi r17, 0 \n"
    "brlt exitChecking \n"        //if b is not ok then check a don't needed.

    "mov r17, %0 \n"
    "forA: \n"           //check if a is dividable by the new r16
    "cpi r17, 1 \n"        //if a < 1 check if dividable
    "brlt checkA \n"
    "sub r17, r16 \n"
    "rjmp forA \n"

    "checkA: \n"
    "cpi r17, 0 \n"
    "breq exitOfGCD \n"

    "exitChecking: \n"
    "dec r16 \n"         //decrease r16
    "rjmp GCDLoop \n"
    
    "exitOfGCD: \n"
    "mov %0, r16 \n"    //the final answer is in a (%0)
    "rjmp exit3 \n"
    "exit3:" : "+r"(a) , "+r"(b) : : "r16", "r17");
    Serial.print("answer = ");
    Serial.println(a);
    Serial.println("----------------------------------------------");    
  }
   else if ( string == "LCM" || string == "lcm"){
    asm("LCM: \n"
    "mov r19, %1 \n" // r19 is b
    "mov r18, %0 \n" // r18 is a
    // we first use our GCD code
    "cp %0, %1 \n"
    "breq exitOfGCD2 \n"   //if equal, then a is the answer
    "mov r16, %0 \n"      //we want to put the smaller number in r16
    "cp %0, %1 \n"
    "brlt GCDLoop2 \n"     //if a < b we're good to go, else put b in r16
    "mov r16, %1 \n"
    
    "GCDLoop2: \n"
    "cpi r16 , 1 \n"     //if r16 == 1 then jump out
    "breq exitOfGCD2 \n"
    
    "mov r17, %1 \n"
    "forB2: \n"           //check if b is dividable by the new r16
    "cpi r17, 1 \n"        //if b < 1 check if dividable
    "brlt checkB2 \n"
    "sub r17, r16 \n"
    "rjmp forB2 \n"
    
    "checkB2: \n"
    "cpi r17, 0 \n"
    "brlt exitChecking2 \n"        //if b is not ok then check a don't needed.

    "mov r17, %0 \n"
    "forA2: \n"           //check if a is dividable by the new r16
    "cpi r17, 1 \n"        //if a < 1 check if dividable
    "brlt checkA2 \n"
    "sub r17, r16 \n"
    "rjmp forA2 \n"

    "checkA2: \n"
    "cpi r17, 0 \n"
    "breq exitOfGCD2 \n"

    "exitChecking2: \n"
    "dec r16 \n"         //decrease r16
    "rjmp GCDLoop2 \n"
    
    "exitOfGCD2: \n"
    "mov %0, r16 \n"    //the final answer is in a (%0)
    // we should now calculate (a/GCD) * b
    "mov %1, %0 \n"
    "mov %0, r18 \n"  // calc a / GCD
    
    "ldi r16 , 0 \n"
    "divLoop2: \n"
    "cp %0 , %1 \n"
    "brlo exitOfDiv2 \n" 
    "inc r16 \n"
    "sub %0 , %1 \n"
    "rjmp divLoop2 \n"
    "exitOfDiv2: \n"
    "mov %0 , r16 \n" //%0 is a /GCD
    "mul %0 , r19 \n" 
    "mov %0 , r0 \n" // %0 is answer
    "rjmp exit4 \n"
    "exit4: " : "+r"(a) , "+r"(b) : : "r16" , "r17" , "r18" , "r19");
    Serial.print("answer = ");
    Serial.println(a);
    Serial.println("----------------------------------------------");    
  }
    else{
      Serial.println("Instruction is not available");
      Serial.println("----------------------------------------------"); 
    }

}
