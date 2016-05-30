/*
AVR platform specific implementation routines (for Atmega8, rewrite for your MC)
*/
#define _AVR_DEMO_VER "1.0"

// definition commands word
#define _CMD_HELP   "help"
#define _CMD_CLEAR  "clear"
#define _CMD_CLR    "clear_port"
#define _CMD_SET    "set_port"
#define _CMD_WR     "write"
#define _CMD_RD     "read"
// arguments for set/clear
#define _SCMD_PA  "PA"
#define _SCMD_PB  "PB"
#define _SCMD_PC  "PC"
#define _SCMD_PD  "PD"
#define _SCMD_PE  "PE"
#define _SCMD_PF  "PF"
#define _SCMD_PG  "PG"
#define _SCMD_PH  "PH"
#define _SCMD_PJ  "PJ"
#define _SCMD_PK  "PK"
#define _SCMD_PL  "PL"

#define _NUM_OF_CMD 6
#define _NUM_OF_SETCLEAR_SCMD 11

//available  commands
char * keyworld [] = {_CMD_HELP, _CMD_CLEAR, _CMD_SET, _CMD_CLR, _CMD_WR, _CMD_RD};
// 'set/clear' command argements
char * set_clear_key [] = {_SCMD_PA, _SCMD_PB, _SCMD_PC, _SCMD_PD, _SCMD_PE, _SCMD_PF, _SCMD_PG, _SCMD_PH, _SCMD_PJ, _SCMD_PK, _SCMD_PL};

// array for comletion
char * compl_world [_NUM_OF_CMD + 1];


//*****************************************************************************
void print_help (void)
{
  print ("Use TAB key for completion\n\rCommand:\n\r");
  print ("\tclear               - clear screen\n\r");
  print ("\tset_port port pin   - set 1 port[pin] value, support only 'port_b' and 'port_d'\n\r");
  print ("\tclear_port port pin - set 0 port[pin] value, support only 'port_b' and 'port_d'\n\r");
  print ("\twrite addr data - write [data] at addres [addr]\n\r");
  print ("\tread addr - read from addres [addr]\n\r");
}

//*****************************************************************************
void set_port_val (unsigned char * port, int pin, int val)
{
  if ((pin < 0) || (pin > 7)) {
    print ("only 0..7 pin avialable for PORTx\n\r");
    return;
  }
  if (val) {
    (*port) |= (1 << pin);
  } else {
    (*port) &= ~(1<<pin);
  }
}

//*****************************************************************************
#define HEXDUMP_COLS 48
 
void hexdump(unsigned int addr, unsigned int len)
{
  unsigned int i, j;
  uint8_t temp_data;
  for (i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++){
    // print offset
    if (i % HEXDUMP_COLS == 0) {
      Serial.print("0x");
      if (addr + i < 0x10) Serial.print("0");
      if (addr + i < 0x100) Serial.print("0");
      if (addr + i < 0x1000) Serial.print("0");
      Serial.print(addr + i, HEX);
      Serial.print("  ");
    }
    // print hex data
    if (i < len) {
      temp_data = *(uint8_t*)(addr + i);
      if (temp_data < 0x10) Serial.print("0");
      Serial.print(temp_data, HEX);
      Serial.print(" ");
    } else { // end of block, just aligning for ASCII dump
      Serial.print("   ");
    }

    // print ASCII dump
    if (i % HEXDUMP_COLS == (HEXDUMP_COLS - 1)) {
      for (j = i - (HEXDUMP_COLS - 1); j <= i; j++) {
        temp_data = *(uint8_t*)(addr + j);
        if (j >= len) { // end of block, not really printing
          Serial.print(" ");
        } else if (isprint(temp_data)) { // printable char
          Serial.write(temp_data);
        } else { // other char
          Serial.print(".");
        }
      }
    Serial.print("\r\n");
    }
  }
}

//*****************************************************************************
// execute callback for microrl library
// do what you want here, but don't write to argv!!! read only!!
int execute (int argc, const char * const * argv)
{
  int i = 0;
  // just iterate through argv word and compare it with your commands
  while (i < argc) {
    if (strcmp (argv[i], _CMD_HELP) == 0) {
      print ("microrl v");
      print (MICRORL_LIB_VER);
      print (" library AVR DEMO v");
      print (_AVR_DEMO_VER);
      print("\n\r");
      print_help ();        // print help
    } 
    //--------------CLEAR--------------
      else if (strcmp (argv[i], _CMD_CLEAR) == 0) {
      print ("\033[2J");    // ESC seq for clear entire screen
      print ("\033[H");     // ESC seq for move cursor at left-top corner
    } 
    //--------------SET_PORT CLEAR_PORT--------------
      else if ((strcmp (argv[i], _CMD_SET) == 0) || 
              (strcmp (argv[i], _CMD_CLR) == 0)) {
      if (++i < argc) {
        int val = strcmp (argv[i-1], _CMD_CLR);
        unsigned char * port = NULL;
        int pin = 0;
        if (strcmp (argv[i], _SCMD_PA) == 0) {
          port = (unsigned char *)&PORTA;
        } else if (strcmp (argv[i], _SCMD_PB) == 0) {
          port = (unsigned char *)&PORTB;
        } else if (strcmp (argv[i], _SCMD_PC) == 0) {
          port = (unsigned char *)&PORTC;
        } else if (strcmp (argv[i], _SCMD_PD) == 0) {
          port = (unsigned char *)&PORTD;
        } else if (strcmp (argv[i], _SCMD_PE) == 0) {
          port = (unsigned char *)&PORTE;
        } else if (strcmp (argv[i], _SCMD_PF) == 0) {
          port = (unsigned char *)&PORTF;
        } else if (strcmp (argv[i], _SCMD_PG) == 0) {
          port = (unsigned char *)&PORTG;
        } else if (strcmp (argv[i], _SCMD_PH) == 0) {
          port = (unsigned char *)&PORTH;
        } else if (strcmp (argv[i], _SCMD_PJ) == 0) {
          port = (unsigned char *)&PORTJ;
        } else if (strcmp (argv[i], _SCMD_PK) == 0) {
          port = (unsigned char *)&PORTK;
        } else if (strcmp (argv[i], _SCMD_PL) == 0) {
          port = (unsigned char *)&PORTL;
        } else {
          print ("port = '");
          print (argv[i]);
          print ("' not support\n\r");
          return 1;
        }
        if (++i < argc) {
          pin = atoi (argv[i]);
          set_port_val (port, pin, val);
          return 0;
        } else {
          print ("specify pin number, use Tab\n\r");
          return 1;
        }
      } else {
          print ("specify port, use Tab\n\r");
        return 1;
      }
    } 
    //--------------WRITE--------------
      else if (strcmp (argv[i], _CMD_WR) == 0) {
      if (++i < argc) {
        uint16_t addr = 0;
        uint8_t data = 0;
        if (addr = atoi(argv[i])) {
          ;
        } else {
          print ("addr = '");
          print (argv[i]);
          print ("' addr not support\n\r");
          return 1;
        }
        if (++i < argc) {
          if (data = atoi(argv[i])) {
            print ("curent data = 0x");
            Serial.print(*(volatile uint8_t*)(addr), HEX);
            print ("\n\rwrite... ");
            print ("addr = 0x");
            Serial.print (addr, HEX);
            print (" data = 0x");
            Serial.print (data, HEX);
            print ("\n\r");
            *(volatile uint8_t*)(addr) = data;
            print ("curent data = 0x");
            Serial.print(*(volatile uint8_t*)(addr), HEX);
            print ("\n\r");
            return 0;
          } else {
            print ("data = '");
            print (argv[i]);
            print ("' data not support\n\r");
            return 1;
          }
        } else {
          print ("specify data\n\r");
          return 1;
        }
      } else {
          print ("specify addr & data\n\r");
        return 1;
      }
    } 
    //--------------READ--------------
      else if (strcmp (argv[i], _CMD_RD) == 0) {
      if (++i < argc) {
        uint16_t row_addr;
        if (row_addr = atoi(argv[i])) {
          ;
        } else {
          row_addr = 0;
          /*print ("row_addr = '");
          print (argv[i]);
          print ("' addr not support\n\r");
          return 1;*/
        }
        print ("rd mem\n\r");
        
        uint8_t buf_id[5];
        print ("FLASH ID dump ---->\n\r");
        flash_read_id(&buf_id[0]);
        hexdump((unsigned int)&buf_id[0], 0x0005);
        
        uint8_t buf_mem[2112];
        print ("FLASH row_addr = 0x");
        Serial.print (row_addr, HEX);
        print (" dump j=");
        Serial.print (flash_read(&buf_mem[0], row_addr));
        print (" ---->\n\r");
        hexdump((unsigned int)&buf_mem[0], 2112);
      } else {
        print ("specify row_addr\n\r");
        return 1;
      }
    } else {
      print ("command: '");
      print ((char*)argv[i]);
      print ("' Not found.\n\r");
    }
    i++;
  }
  return 0;
}

#ifdef _USE_COMPLETE
//*****************************************************************************
// completion callback for microrl library
char ** complet (int argc, const char * const * argv)
{
  int j = 0;

  compl_world [0] = NULL;

  // if there is token in cmdline
  if (argc == 1) {
    // get last entered token
    char * bit = (char*)argv [argc-1];
    // iterate through our available token and match it
    for (int i = 0; i < _NUM_OF_CMD; i++) {
      // if token is matched (text is part of our token starting from 0 char)
      if (strstr(keyworld [i], bit) == keyworld [i]) {
        // add it to completion set
        compl_world [j++] = keyworld [i];
      }
    }
  } else if ((argc > 1) && ((strcmp (argv[0], _CMD_SET)==0) || 
                           (strcmp (argv[0], _CMD_CLR)==0))) { // if command needs subcommands
    // iterate through subcommand
    for (int i = 0; i < _NUM_OF_SETCLEAR_SCMD; i++) {
      if (strstr (set_clear_key [i], argv [argc-1]) == set_clear_key [i]) {
        compl_world [j++] = set_clear_key [i];
      }
    }
  } else { // if there is no token in cmdline, just print all available token
    for (; j < _NUM_OF_CMD; j++) {
      compl_world[j] = keyworld [j];
    }
  }

  // note! last ptr in array always must be NULL!!!
  compl_world [j] = NULL;
  // return set of variants
  return compl_world;
}
#endif

//*****************************************************************************
void sigint (void)
{
  print ("^C catched!\n\r");
}

