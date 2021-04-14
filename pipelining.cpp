# include <iostream>
# include <bits/stdc++.h>
# include <fstream>
# include <sstream> 
using namespace std;

int R[32];         // Global array of Registers
int Mem[1024];     // Global array of Memory -> Creating array of 4KB memory = 4 bytes x ( 1024 length )
int base_address;  // For lw and sw we need addresses of memory locations, we can get them with this

int cycle = 0;
int stall = 0;

int n = 100;       // file size
string arr[100];   // Each one stores one line of file (Its not working if we write as arr[n] )


bool Check ( string sentence, string word ) 
{
    stringstream s(sentence); 
    string temp; 
  
    while (s >> temp) 
    {   // Comparing the current word with the word to be searched 
        if (temp.compare(word) == 0) 
            return true;                      
    } 

    return false; 
}

bool Check_any ( string sentence )
{
    stringstream s(sentence); 
    string temp; 
  
    s >> temp;

    if ( temp == "add" || temp == "addi" || temp == "sub" || temp == "subi" ) 
      return true;                       
    if ( temp == "li" || temp == "lw" || temp == "sw" )
      return true;
    if ( temp == "j" || temp == "bne" ) 
      return true;

    return false; 
}

bool Check_label ( string sentence )
{
    stringstream s(sentence); 
    string temp; 
    s >> temp;   /// Holde label

    if ( temp[temp.length()-1] == ':' )
        return true;
    else
        return false;    
    
}


bool Dependency ( string line1, string line2 )
{
    string Reg11,Reg12,Reg13;
    string Reg21,Reg22,Reg23;

    stringstream l1(line1); 
    stringstream l2(line2); 
    string temp1;
    string temp2;

    l1 >> temp1;
    l2 >> temp2;

    string inst1 = temp1;
    string inst2 = temp2;

    // Storing registers of line 1 --------------------------------------------------------

    l1 >> temp1;            // temp[0] = '$'                      
    Reg11 = temp1[1] + temp1[2]; //storing Register name of 1st register

    if( inst1 == "lw" || inst1 == "sw")
    {
        l1 >> temp1;            // temp = 0($t2)  

        int i=0;      
        while ( temp1[i]!='$' )
          i++;
                     
        Reg12 = temp1[i+1] + temp1[i+2]; //storing Register name of 1st register
    }

    else
    {
        if (inst1 == "li")
        {
            l1 >> temp1;            // temp = integer value  
            Reg12 = temp1;
        }

        else if ( inst1 == "addi" || inst1 == "subi" )
        {
            l1 >> temp1;             // temp[0] = '$'               
            Reg12 = temp1[1] + temp1[2]; //storing Register name of 2nd register

            l1 >> temp1;            // temp = integer value  
            Reg13 = temp1;
        }

        else if ( inst1 == "bne" || inst1 == "bne" )
        {
            l1 >> temp1;             // temp[0] = '$'               
            Reg11 = temp1[1] + temp1[2]; //storing Register name of 1st register

            l1 >> temp1;             // temp[0] = '$'               
            Reg12 = temp1[1] + temp1[2]; //storing Register name of 2nd register

            l1 >> temp1;            // label
            Reg13 = temp1;
        }

        else // add, sub
        {
            l1 >> temp1;             // temp[0] = '$'               
            Reg12 = temp1[1] + temp1[2]; //storing Register name of 2nd register

            l1 >> temp1;             // temp[0] = '$'               
            Reg13 = temp1[1] + temp1[2]; //storing Register name of 3rd register
        }
    }

    // -----------Storing registers of line 2 ----------------------------------------------------------
    l2 >> temp2;             // temp[0] = '$'                      
    Reg21 = temp2[1] + temp2[2]; //storing Register name of 1st register


    if( inst2 == "lw" || inst2 == "sw")
    {
        l2 >> temp2;            // temp = 0($t2)  

        int i=0;      
        while ( temp2[i]!='$' )
          i++;
                     
        Reg22 = temp2[i+1] + temp2[i+2]; //storing Register name of 1st register
    }

    else
    {
        if (inst2 == "li")
        {
            l2 >> temp2;            // temp = integer value  
            Reg22 = temp2;
        }

        else if (inst2 == "addi" || inst2 == "subi")
        {
            l2 >> temp2;             // temp[0] = '$'               
            Reg22 = temp2[1] + temp2[2]; //storing Register name of 2nd register

            l2 >> temp2;            // temp = integer value  
            Reg23 = temp2;
        }

         else if ( inst2 == "bne" || inst2 == "bne" )
        {
            l2 >> temp2;             // temp[0] = '$'               
            Reg21 = temp2[1] + temp2[2]; //storing Register name of 1st register

            l2 >> temp2;             // temp[0] = '$'               
            Reg22 = temp2[1] + temp2[2]; //storing Register name of 2nd register

            l2 >> temp2;            // label
            Reg23 = temp2;
        }

        else // add, sub
        {
            l2 >> temp2;             // temp[0] = '$'               
            Reg22 = temp2[1] + temp2[2]; //storing Register name of 2nd register

            l2 >> temp2;             // temp[0] = '$'               
            Reg23 = temp2[1] + temp2[2]; //storing Register name of 3rd register
        }
        
    }
   
    // ------------------- CHECKING DEPENDENCY ------------------------------
    
    // if ( Check_label(line1) )

    // All "j" cases 
    if (inst1 == "j" || inst2 == "j")
    {
       // if ( inst1 == "j"  )    // this case never occurs
       //    return true;

        if ( inst2 == "j" )   
            return false;
    }

    // All "bne" cases
    if (inst1 == "bne" || inst2 == "bne")
    {
        if ( inst1 == "bne"  )  
        {
            return false;      
        }

        if ( inst2 == "bne" )   
        {
            if( inst1 == "add" || inst1 == "addi" || inst1 == "sub" || inst1 == "subi" || inst1 == "lw" || inst1 == "li" )
            {
                if(Reg11 == Reg21 || Reg11 == Reg22 )
                  return true;

                else
                  return false;
            }

            else if ( inst1 == "sw" )
            {
                return false;
            }

            // No need to check inst1 == "j" , it cant be possible
        }

    }

    // All "sw" cases
    // There is a reason for writing "sw" first (to remove all exceptions)
    
    if ( inst1 == "sw" || inst2 == "sw" )
    {
        if ( inst1 == "sw" && inst2 == "sw" )   // Indepensent of Dependency
           return false;

        if ( inst1 == "sw" )     // Indepensent of Dependency
            return false;

        if ( inst2 == "sw" )   
        {
            if(Reg11 == Reg21 || Reg11 == Reg22 )
                return true;

            else
                return false;
        }
        
    }

    // All "lw" cases
    if ( inst1 == "lw" || inst2 == "lw" )
    {
        if ( inst1 == "lw" && inst2 == "lw" )   
        {
            if( Reg11 == Reg22 )
              return true;
            else
              return false;
        }

        if ( inst1 == "lw" )    
        {
            if(Reg11 == Reg22 || Reg11 == Reg23 )
                return true;
            else
                return false;
        }

        if ( inst2 == "lw" )   
        {
            if(Reg11 == Reg22 )
                return true;

            else
                return false;
        }
        
    }

    // All " li " cases
    if ( inst1 == "li" || inst2 == "li" )
    {
        if ( inst1 == "li" && inst2 == "li" )   // Indepensent of Dependency
            return false;

        if ( inst1 == "li" )     
        {
            // if(inst2 == "sw")   // Actually this case can never occur as all "sw" cases appeared above
            // {
            //     if ( Reg11 == Reg21 || Reg11 == Reg22 )
            //         return true;
            //     else
            //         return false;
            // }

            if ( Reg11 == Reg22 || Reg11 == Reg23 )
                return true;
            else
                return false;
        }

        if ( inst2 == "li" )   // Indepensent of Dependency
            return false;
    }


    // For all combinations of add,addi,sub,subi
    if(Reg11 == Reg22 || Reg11==Reg23)
      return true;

    else
      return false;
            
}

string Get_Reg ( string sentence, string word, int num )
{
    int x,y,z;
    char a,b,c;
    string Reg1,Reg2,Reg3;

    stringstream s(sentence); 
    string temp; 
  
    while (s >> temp) 
    {   // Comparing the current word with the word to be searched 
        if (temp.compare(word) == 0) 
        {             

            if (word == "add" || word == "sub" )
            {
                s >> temp;                // temp[0] = '$'                      
                Reg1 = temp[1] + temp[2]; //storing Register name of 1st register

                s >> temp;                // temp[0] = '$'               
                Reg2 = temp[1] + temp[2]; //storing Register name of 2nd register

                s >> temp;                // temp[0] = '$'               
                Reg3 = temp[1] + temp[2]; //storing Register name of 3rd register
                
                
                if(num == 1)
                    return Reg1;
    
                else if (num == 2)
                    return Reg2;
                
                else if (num==3)
                    return Reg3;
            }

            else if (word == "addi" || word == "subi" )
            {

                s >> temp;                // temp[0] = '$'                      
                Reg1 = temp[1] + temp[2]; //storing Register name of 1st register

                s >> temp;                // temp[0] = '$'               
                Reg2 = temp[1] + temp[2]; //storing Register name of 2nd register

                s >> temp;                // temp = integer               
                Reg3 = temp; //storing Register name of 3rd register
                
                
                if(num == 1)
                    return Reg1;
    
                else if (num == 2)
                    return Reg2;
                
                else if (num==3)
                    return Reg3;
            }
            
        }
                       
    } 

}


int Hexa_To_Dec_Converter(char num[])
{
   int len = strlen(num);
   int base = 1;
   int temp = 0;

   for (int i=len-1; i>=0; i--)
   {
      if ( num[i]>='0' && num[i]<='9' ) 
      {
         temp += (num[i] - 48)*base;
         base = base * 16;
      }
      else if ( num[i]>='A' && num[i]<='F' ) 
      {
         temp += (num[i] - 55)*base;
         base = base*16;
      }
   }
   return temp;
}

string Dec_To_Hexa_Converter(int num)
{
    char hex_string[20];
    sprintf (hex_string, "%X", num); //convert number to hex

    return hex_string;
}


void PerformAdd (char a,char b, char c,int x,int y, int z)
{   
    // Adds the other 2 register elements and stores in 1st register
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if(c == 't')
        r = R[z];
    else if (c == 'r')
        r = R[10+z];

    p = q+r;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;

}

void PerformSub (char a,char b,char c, int x,int y,int z)
{
    // Subtracts the other 2 register elements and stores in 1st register
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if(c == 't')
        r = R[z];
    else if (c == 'r')
        r = R[10+z];

    p = q-r;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;

}


bool PerformEqual (char a,char b, int x,int y)
{
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if( p == q)
        return true;
    else
        return false;
}

bool PerformSLT (char a,char b, char c,int x,int y, int z)
{
    int p,q,r;

    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if(c == 't')
        r = R[z];
    else if (c == 'r')
        r = R[10+z];

    if(q < r)
    {
        if(a == 't')
            R[x] = 1;
        else if (a == 'r')
            R[10+x] = 1;
    }

    else
    {
        if(a == 't')
            R[x] = 0;
        else if (a == 'r')
            R[10+x] = 0;
    }
    
    return true;
}


bool PerformBEQZ (char a, int x)
{
    int p;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];


    if( p == 0)
        return true;
    else
        return false;
}

bool PerformBGE (char a,char b, int x,int y)
{
    int p,q;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if( p >= q)
        return true;
    else
        return false;
}

bool PerformBLE (char a,char b, int x,int y)
{
    int p,q;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if( p <= q)
        return true;
    else
        return false;
}


bool PerformLI (char a, int x,int y)
{
    int p = y; // Storing the immediate integer to be stored

    // Simply load that into the register
    if(a == 't')
        R[x] = p;
    else if (a == 'r')
        R[10+x] = p;

    return true;

}

bool PerformLW (char a,char b, int x,int y,int z)
{
    int q,r;

    if (b == 't')
        q = R[z];
    else if (b == 'r')
        q = R[10+z];

    
    // Now q will be having address in decimal value
    q = q + y;            // Adding offset
    q = q - base_address; // Removing base address
    q = q/4 ;             // To get index of Memory array

    r = Mem[q];     // If we use Mem[q] code is not working /No output
    
   
    //      int *p = &(Mem[1]);
    //      cout << *(p);         so , p = 0x40d0a4 , *p = 2
    
    if (a == 't')
        R[x] = r;
    else if (a == 'r')
        R[10+x] = r;

    return true;
}

bool PerformSW (char a,char b, int x,int y,int z)
{
    int q,r;

    if (b == 't')
        q = R[z];
    else if (b == 'r')
        q = R[10+z];

    // Now q will be having address in decimal value
    q = q + y;            // Adding offset
    q = q - base_address; // Removing base address
    q = q/4 ;             // To get index of Memory array 

    //      int *p = &(Mem[1]);
    //      cout << *(p);         so , p = 0x40d0a4 , *p = 2
    
    if (a == 't')
        Mem[q] = R[x] ;
    else if (a == 'r')
        Mem[q] = R[10+x] ;   

    return true;
}


void PerformAddI (char a,char b, int x,int y, int z)
{
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    p = q+z;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;
}

void PerformSubI (char a,char b, int x,int y, int z)
{
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    p = q-z;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;
}

void PerformSLL  (char a,char b, int x,int y, int z)
{
    int p,q,r;

    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];


    for (int i=0;i<z;i++)
    {
        q = q*2;  // Shifting left
    }


    p = q;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;
}



bool add_sub_Check   ( string sentence, string word ) 
{
    int x,y,z;
    char a,b,c;

    // To break the sentence in words 
    stringstream s(sentence); 
  
    // To temporarily store each individual word 
    string temp; 
  
    while (s >> temp) 
    {   // Comparing the current word with the word to be searched 
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            c = temp[p++];        // storing variable name of 3rd register
            z = temp[p++] - 48;   // storing value name of 3rd register

            if  (word == "add")
                PerformAdd(a,b,c,x,y,z);

            else if (word == "sub")
                PerformSub(a,b,c,x,y,z);

            return true; 
        }
           
            
    } 
    return false; 
}

bool addi_subi_Check ( string sentence, string word )
{
    int x,y,z;
    char a,b;

    // To break the sentence in words 
    stringstream s(sentence); 
  
    // To temporarily store each individual word 
    string temp; 
  
    while (s >> temp) 
    {   // Comparing the current word with the word to be searched 
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register


            s >> temp;  // temp[0] = '$'  
                      

            string str_temp = temp; 
            stringstream geek(str_temp); // object from the class stringstream 
  
            // The object has integer in string form and now, stream it to the integer int_temp 
            int int_temp = 0; 
            geek >> int_temp; 
  
            // Now the variable int_temp holds the integer value of the string        
              
            z = int_temp;   // storing integer value of 3rd register

            if  (word == "addi")
                PerformAddI(a,b,x,y,z);

            else if (word == "subi")
                PerformSubI(a,b,x,y,z);

            return true; 
        }
           
            
    } 
    return false; 
}


bool li_Check ( string sentence, string word )
{
    int x,y;
    char a;

    // To break the sentence in words 
    stringstream s(sentence); 
  
    // To temporarily store each individual word 
    string temp; 
  
    while (s >> temp) 
    {   // Comparing the current word with the word to be searched 
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register

            s >> temp;  // temp[0] = '$'  
                      
            if( temp[1] == 'x' )  // If that number is in hexadecimal
            {
                char hex[1000000] ;

                for(int d=2; temp[d]!=' ' ; d++)
                hex[d-2] = temp[d];

                // cout << a << endl;   
                // Now this string 'a' will be storing the part after '0x' in hexadecimal input given

                // I will convert this 'hexadecimal string' into 'decimal int' and store in register array
                int int_temp = int_temp = Hexa_To_Dec_Converter( hex );
                
                y = int_temp;   // storing integer value of 3rd register

                base_address = y; // Assigning given address to  base_address

                PerformLI (a,x,y);

            }

            else  // if that number is in Decimal value
            {
                string str_temp = temp; 
                stringstream geek(str_temp); // object from the class stringstream 
  
                // The object has integer in string form and now, stream it to the integer int_temp 
                int int_temp = 0; 
                geek >> int_temp; 
  
                // Now the variable int_temp holds the integer value of the string        
              
                y = int_temp;   // storing integer value of 3rd register
            
                PerformLI (a,x,y);
            }
            

            return true; 
        }
           
            
    } 
    return false; 
}

bool lw_Check ( string sentence, string word )
{
    int x,y,z;
    char a,b;

    stringstream s(sentence); 
    string temp; 
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register

            s >> temp;   
           
            string offset = "";     
            int k=0;       
            for( k=0 ; temp[k]!='(' ; k++)
            {
                offset += temp[k];     // collecting offset as a string
                //offset[i] = temp[k]; //This method is not working
            }

            stringstream geek(offset);  
            geek >> y;            // converting offset into int
           
            p = k+2;   
            b = temp[p++];        // storing variable name of 2nd register 
            z = temp[p++] - 48;   // storing value name of 2nd register 

            PerformLW (a,b,x,y,z); 
               
            return true; 
        }
        else
            return false;
    } 
}

bool sw_Check ( string sentence, string word )
{
    int x,y,z;
    char a,b;

    stringstream s(sentence); 
    string temp; 
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register

            s >> temp;   
           
            string offset = "";     
            int k=0;       
            for( k=0 ; temp[k]!='(' ; k++)
            {
                offset += temp[k];     // collecting offset as a string
                //offset[i] = temp[k]; //This method is not working
            }

            stringstream geek(offset);  
            geek >> y;            // converting offset into int
           
            p = k+2;   
            b = temp[p++];        // storing variable name of 2nd register 
            z = temp[p++] - 48;   // storing value name of 2nd register 

            PerformSW (a,b,x,y,z); 
               
            return true; 
        }
        else
            return false;
    } 
}


bool slt_Check ( string sentence, string word )
{
    int  x,y,z;
    char a,b,c;

    stringstream s(sentence); 
    string temp; 
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            c = temp[p++];        // storing variable name of 3rd register
            z = temp[p++] - 48;   // storing value name of 3rd register

            PerformSLT (a,b,c,x,y,z);
            
            return true; 
        }
        else
            return false;
    } 
}

bool sll_Check ( string sentence, string word )
{
    int x,y,z;
    char a,b;

    stringstream s(sentence); 
    string temp; 
  
    while (s >> temp) 
    {   // Comparing the current word with the word to be searched 
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register


            s >> temp;  // temp[0] = '$'  
                      

            string str_temp = temp; 
            stringstream geek(str_temp); // object from the class stringstream 
  
            // The object has integer in string form and now, stream it to the integer int_temp 
            int int_temp = 0; 
            geek >> int_temp; 
  
            // Now the variable int_temp holds the integer value of the string        
              
            z = int_temp;   // storing integer value of 3rd register

            PerformSLL(a,b,x,y,z);

            return true; 
        }
           
            
    } 
    return false; 
}


string jump_Check ( string sentence, string word )
{ 
    stringstream s(sentence); 
    string temp; 
    string example = "fault";
  
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // This temp will store label i.e, where to jump
            //temp = temp+":"; This is not required, we need only label
        
            return temp;
        }
        else
            return example;
    } 
}

string bne_Check  ( string sentence, string word )
{
    int x,y;
    char a,b;

    stringstream s(sentence); 
    string temp; 
    string wrong = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register

            s >> temp; // This temp will be having our label 
            
            if( PerformEqual(a,b,x,y) == true )
                return wrong; // So that control just goes to next line
            else
                return temp;  // So that control flows through the mentioned label
            
        }
        else
            return wrong;
    } 
}

string beq_Check  ( string sentence, string word )
{
    int x,y;
    char a,b;

    stringstream s(sentence); 
    string temp; 
    string wrong = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register

            s >> temp; // This temp will be having our label 

            if( PerformEqual(a,b,x,y) == false )
                return wrong; // So that control just goes to next line
            else
                return temp;  // So that control flows through the mentioned label
            
        }
        else
            return wrong;
    } 
}


string beqz_Check ( string sentence, string word )
{
    int x,y;
    char a,b;

    stringstream s(sentence); 
    string temp; 
    string wrong = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register

            s >> temp; // This temp will be having our label 

            if( PerformBEQZ(a,x) == false )
                return wrong; // So that control just goes to next line
            else
                return temp;  // So that control flows through the mentioned label
            
        }
        else
            return wrong;
    } 
}

string bnez_Check ( string sentence, string word )
{
    int x,y;
    char a,b;

    stringstream s(sentence); 
    string temp; 
    string wrong = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register

            s >> temp; // This temp will be having our label 

            if( PerformBEQZ(a,x) == true )
                return wrong; // So that control just goes to next line
            else
                return temp;  // So that control flows through the mentioned label
            
        }
        else
            return wrong;
    } 
}

string bge_Check  ( string sentence, string word )
{
    int x,y;
    char a,b;

    stringstream s(sentence); 
    string temp; 
    string wrong = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register

            s >> temp; // This temp will be having our label 

            if( PerformBGE(a,b,x,y) == false )
                return wrong; // So that control just goes to next line
            else
                return temp;  // So that control flows through the mentioned label
            
        }
        else
            return wrong;
    } 
}

string ble_Check  ( string sentence, string word )
{
    int x,y;
    char a,b;

    stringstream s(sentence); 
    string temp; 
    string wrong = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register

            s >> temp; // This temp will be having our label 

            if( PerformBLE(a,b,x,y) == false )
                return wrong; // So that control just goes to next line
            else
                return temp;  // So that control flows through the mentioned label
            
        }
        else
            return wrong;
    } 
}


string word_Check ( string sentence, string word )
{
    int x,y,count = 0;
    char a,b;

    stringstream s(sentence); 
    string temp; 
    string wrong = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp; 
            while (temp != ";")
            {             
                string offset = "";     
                int k=0;       
                for( k=0 ; temp[k]!=',' ; k++)
                {
                    offset += temp[k];     // collecting integers as a string
                    //offset[i] = temp[k]; //This method is not working
                }

                stringstream geek(offset);  
                geek >> y;            // converting string integers into int
                 
                // cout << y << "*";
                Mem[count] = y;
                // cout << Mem[count] << "*";
                count++;

                s >> temp;
            }
            
        }

    } 
}

string search_label(string str) 
{ 
    string word = ""; 
    for (auto x : str)  
    { 
        if (x == ':') 
        {   
            //cout << "*";
            return word;
            //cout << word << endl; 
            word = ""; 
        } 

        else if(x == ' ' || x == '\n') 
            word = ""; 

        else
             word = word + x; 
    } 
} 


void PrintAllRegisters (int R[] , int k)
{
    for(int i=0;i<k;i++)
        cout << " R[" << i << "] = " << R[i] << endl;
}

void PrintAllMemory ( int Mem[] , int k )
{
    for(int i=0;i<k;i++)
        cout << " Mem[" << i << "] = " << Mem[i] << endl;
}


void UPDATE_REGISTERS ( int k )
{    
    // Here we update the Register values at each line
    // We dont have to include "j" or "bne" as they dont update registers
    // No need of any FILE or MAP here

    add_sub_Check  ( arr[k], "add" ) ;
    add_sub_Check  ( arr[k], "sub" ) ;
            
    addi_subi_Check( arr[k], "addi" ) ;
    addi_subi_Check( arr[k], "subi" ) ;

    li_Check ( arr[k], "li" );      // Load immediate
    lw_Check ( arr[k], "lw" );      // Load word
    sw_Check ( arr[k], "sw" );      // Store word

    word_Check ( arr[k], ".word" );

}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


int main()
{
    cout<< "****************( REGISTER & MEMORY )**********************"<<endl;    

    // Assigning values to registers
    int i=0;         
    for(int i=0;i<32;i++)
       R[i] = 0;

    // Creating array of 4KB memory = 4 bytes x ( 1024 length )
    for (int i=0;i<1024;i++)
        Mem[i] = 0;


    // Here we can clearly observe that this array holds fixed memory address in all cases
    // cout << Mem[0] << endl <<  &(Mem[0]) << endl << *(&(Mem[0])) << endl;
    // cout << Memory[1] << endl <<  &(Memory[1]) << endl;

    ////********************* F I L E *************************//// 

    std :: string line;
    ifstream file("assembly.txt");

    if(!file.is_open())
        cout << "error" << endl;

    while (getline(file,line))
        arr[i++] = line;

    ////********************** M A P ********************////

    map <string,int> search;

    for(int i=0 ;i<n ; i++)
    {

        stringstream s(arr[i]); 
        string temp; 
  
        s >> temp;
        if ( temp [temp.length()-1] == ':')
        {
            string line  = arr[i];
            string label = search_label(line);

            //cout << label;        
            //label = label + ":";  // Shoud not do this ,we need without colon only

            search.insert({label,i});
            //search.insert({"while",4});
        }     
    } 

    //   If input file is "I am a human:"
    //    search ["human"];   // Prints in which line it is there


    //**************( PRINTING REGISTERS & MEMORY ELEMENTS )**************************************//


    for (int k=0;k<n;k++)
    {  
        // if(arr[k] == "")
        // cout << "* ";      // This will print only if a line dont have anything (not even space)
        //                       or in the case of  the last unused remaining lines

        //while(arr[k] == "")  // Never write this you wont get any output
        //k++;

        add_sub_Check  ( arr[k], "add" ) ;
        add_sub_Check  ( arr[k], "sub" ) ;
            
        addi_subi_Check( arr[k], "addi" ) ;
        addi_subi_Check( arr[k], "subi" ) ;

        li_Check ( arr[k], "li" );      // Load immediate
        lw_Check ( arr[k], "lw" );      // Load word
        sw_Check ( arr[k], "sw" );      // Store word

        slt_Check ( arr[k], "slt");      
        sll_Check ( arr[k], "sll");

        // Loading array elements into memory if you need
        word_Check ( arr[k], ".word" );


        // Checking 'J'
        string jumpLabel = jump_Check(arr[k], "j");   
        if( arr[k]!= ""  &&  jump_Check(arr[k], "j") != "fault" )
        {
            // jumpLabel = jumpLabel +":";     //should not do
            // cout << jumpLabel;
            // cout << search ["while"];       //Shows where while: is located
            
            k = search [jumpLabel];   //jump_Index;
            
            continue;
        }

        // Checking 'bne'
        string  bne_Label =  bne_Check (arr[k], "bne");
        if( arr[k]!= ""  &&  bne_Check (arr[k], "bne") != "fault" )
        {
            // # Similar to above Jump case
            k = search [bne_Label];   //jump_Index;

            continue;
        }

        // Checking 'beq'
        string  beq_Label =  beq_Check (arr[k], "beq");
        if( arr[k]!= ""  &&  beq_Check (arr[k], "beq") != "fault" )
        {
            // # Similar to above Jump case
            k = search [beq_Label];   //jump_Index;

            continue;
        }

        // Checking 'beqz'
        string  beqz_Label =  beqz_Check (arr[k], "beqz");
        if( arr[k]!= ""  &&   beqz_Check (arr[k], "beqz") != "fault" )
        {
            // # Similar to above Jump case
            k = search [beqz_Label];   //jump_Index;

            continue;
        }

        // Checking 'bnez'
        string  bnez_Label =  bnez_Check (arr[k], "bnez");
        if( arr[k]!= ""  &&   bnez_Check (arr[k], "bnez") != "fault" )
        {
            // # Similar to above Jump case
            k = search [bnez_Label];   //jump_Index;

            continue;
        }

        // Checking 'bge'
        string  bge_Label =  bge_Check (arr[k], "bge");
        if( arr[k]!= ""  &&  bge_Check (arr[k], "bge") != "fault" )
        {
            // # Similar to above Jump case
            k = search [bge_Label];   //jump_Index;

            continue;
        }

        // Checking 'ble'
        string  ble_Label =  ble_Check (arr[k], "ble");
        if( arr[k]!= ""  &&  ble_Check (arr[k], "ble") != "fault" )
        {
            // # Similar to above Jump case
            k = search [ble_Label];   //jump_Index;

            continue;
        }


    }

    cout << "\n  Register Elements :" << "\n\n";
    PrintAllRegisters(R, 20);   // Printing 32 registers
    cout << endl;

    cout << "\n  Memory Elements :" << "\n\n";
    PrintAllMemory (Mem, 20);   // Printing required number of memory elements upto 1024
    cout << endl;
    

    // ---------------------------_______________________----------------------------------
    // ---------------------------| P I P E L I N I N G |------------------------------------
    // -------------------------- |_____________________|------------------------------------------



    // ********************** ( F O R W A R D I N G ) ****************************

     // Assigning values to registers
    for(int i=0;i<32;i++)
       R[i] = 0;

    // Creating array of 4KB memory = 4 bytes x ( 1024 length )
    for (int i=0;i<1024;i++)
        Mem[i] = 0;

    cycle = 0;
    stall = 0;    

    cout << endl;
    cout << "**************** (PIPELINING - FORWARDING ) **********************"<<endl;    
    cout << endl;
     
    for (int k=0;k<n;k++)
    {  
        if ( Check_any ( arr[k] )  )
        {

            if (k == 0 )
            {
                // Here the 1st line cant be empty and write any instruction simply ( li $t0, 0 )
                cycle+=5;
            }   
            
            else
            {
                if ( Check(arr[k], "lw") )
                   {
                       cycle+=2;
                       stall++;
                   }   

                else
                    cycle++;
            }   
            
            // This step is really important to update registers at each line

            UPDATE_REGISTERS (k);       

            //Now, Just To change the control flow to required line

            if( Check(arr[k] ,"j") )
            {
                string jumpLabel = jump_Check(arr[k], "j"); // contains label or "fault"
                
                if ( jump_Check(arr[k], "j") != "fault" )
                { 
                    // cycle++;  // for its own 5 consecutive stages without any stalls
                    // cycle+=1;    // one stall for next instruction to get in IF stage 
                    k = search [jumpLabel] ;   //jump_Index;
                    // continue;                 // This increments k again in for loop
                    // cout <<" " << k << " ";
                }
               
            }

            if( Check(arr[k] ,"bne") )
            {
                string  bne_Label =  bne_Check (arr[k], "bne"); // contains label or "fault"
                
                if ( bne_Check (arr[k], "bne") != "fault" )
                { 
                    // cycle++;  // for its own 5 consecutive stages without any stalls
                    // cycle+=1;    // one stall for next instruction to get in IF stage 
                    k = search [bne_Label] ;   //jump_Index;
                    // continue;                 // This increments k again in for loop
                    // cout <<" " << k << " ";
                }
               
            }

            // Printing Cycles and Stalls at each line  
            cout << " CYCLES: " << cycle << "  " << "  STALLS: " << stall << "  >>    " << arr[k]<< endl;
            //cout << cycle << ",";
        }         

        //else
        //{
        //   cout << arr[k] << endl;
        //}

        
    } 

    cout << endl;
    cout << " Total Number of Cycles ( Forwarding ) : " << cycle << endl ;
    cout << " Total Number of STALLS ( Forwarding ) : " << stall << endl ;
    cout << endl;
    

    // **********************  ( N O N - F O R W A R D I N G )  *******************************************

     // Assigning values to registers         
    for(int i=0;i<32;i++)
       R[i] = 0;

    // Creating array of 4KB memory = 4 bytes x ( 1024 length )
    for (int i=0;i<1024;i++)
        Mem[i] = 0;

    cycle = 0;
    stall = 0;

    cout << endl;
    cout <<  "**************** (PIPELINING - NON FORWARDING ) **********************" << endl;   
    cout << endl;

    for (int k=0;k<n;k++)
    {  
        if ( Check_any ( arr[k] )  )
        {
           
            if(k == 0)
              cycle+=5;
            
            else if (k == 1)   
            {
               if ( Check_label( arr[k-1]) ) 
               {
                   cycle++;
               }

               else if( Check_any ( arr[k-1] )  )
               {
                   if( Dependency(arr[k-1],arr[k]) )
                    {
                        cycle+=3;
                        stall+=2;
                    } 

                   else
                     cycle++;  
               }                 
            }

            else
            {   
                // Every line should have either label or instruction
                if( Check_label (arr[k-1]) )
                {
                    cycle+=1;
                }

                else if( Check_any ( arr[k-1] )  )
                {
                    if( Dependency(arr[k-1],arr[k]) )
                    {
                        cycle+=3;
                        stall+=2;
                    } 
                    
                    else  
                    {
                        if ( Check_label(arr[k-2]) )
                        {
                            cycle+=1;
                        }
                        
                        else if( Check_any ( arr[k-2] ) ) 
                        {
                            if( Dependency(arr[k-2],arr[k]) )
                            {
                                cycle+=3;
                                stall++;
                            }   

                            else
                              cycle+=1; 
                        }   
                    } 

                }

                // else if( Check_any ( arr[k-2] ) )  
                // {
                //     if( Dependency(arr[k-2],arr[k]) )
                //      cycle+=2;  
                
                //     else
                //       cycle++;  
                // }  

                // else 
                //    cycle++;
                 
            }

            UPDATE_REGISTERS (k);


            // To change the control flow to required line

            if( Check(arr[k] ,"j") )
            {
                string jumpLabel = jump_Check(arr[k], "j"); // contains label or "fault"
                
                if ( jump_Check(arr[k], "j") != "fault" )
                { 
                    // cycle++;  // for its own 5 consecutive stages without any stalls
                    cycle+=1;    // one stall for next instruction to get in IF stage 
                    stall++;
                    k = search [jumpLabel] ;   //jump_Index;
                    // continue;                 // This increments k again in for loop
                    // cout <<" " << k << " ";
                }
               
            }

            if( Check(arr[k] ,"bne") )
            {
                string  bne_Label =  bne_Check (arr[k], "bne"); // contains label or "fault"
                
                if ( bne_Check (arr[k], "bne") != "fault" )
                { 
                    // cycle++;  // for its own 5 consecutive stages without any stalls
                    cycle+=1;    // one stall for next instruction to get in IF stage 
                    stall++;
                    k = search [bne_Label] ;   //jump_Index;
                    // continue;                 // This increments k again in for loop
                    // cout <<" " << k << " ";
                }
                else
                {
                    cycle++; // for branch instruction next line (beginning IF double times)
                    stall++;  
                }

               
            }
            
            cout << " CYCLES: " << cycle << "  " << "  STALLS: " << stall << "  >>    " << arr[k]<< endl;
            //cout << cycle << ",";
    
        }         
        
    }

    cout << " Total Number of CYCLES ( Non Forwarding ) : " << cycle << endl ;
    cout << " Total Number of STALLS ( Non Forwarding ) : " << stall << endl << endl ;



    cout <<  "*********************************************************************" << endl;  

    file.close();
    return 0;

}

/*  EXCEPTIONS 

    -> If You feel you are not getting any output just save file again(any random comment) and run

    -> 1st line should definately have any instruction of add/addi/sub/subi/j/bne/li/lw/sw (cant be label)
    -> To check only Register and Memory elements many instructions are available
       But to check stalls and cycles only use {add/addi/sub/subi/j/bne/li/lw/sw} these instructions
       ( so we cant check the BUBBLE SORT algo as we used other instructions in it)
    
    -> file should not be empty
    -> each line should end with space
    -> Dont leave any line empty (You can just have empty line before any LABEL)

    -> Dont keep any label named "fault" ,its already reserved 
    -> should follow good pattern or spacing in add ,addi,.....as mentioned in README
    
    -> we have 20 registers at present

    -> If u feel like you fount out any other include here :)
*/


/* Test file to check j and bne

add $t0, $t1, $t2 
add $t3, $t4, $t5 
j while 
loop: 
    addi $r3, $r3, 1 
while: 
   add $r0, $r1, $r2 
bne $r3, $r4, loop 
   addi $r9, $r9, 10 

*/


/* Test file to check BUBBLE SORT

.data
.word  1, -3, 5, 7, 11, -223, 21, 45, 9, ;  
.text 
.globl main 
main: 

BubbleSort: 

li $t0, 0x10010000       
li $t1, 0      
li $t2, 0     
li $r1, 8 

OuterLoop:  
    beq $t1, $r1, exit      
    li  $t0, 0x10010000 
    li  $t2, 0   

    InnerLoop: 

        beq $t2, $r1, exitInnerLoop   
        lw  $t8, 0($t0)         	
        lw  $t9, 4($t0)         	

        ble $t8, $t9, GoAhead       
        sw  $t9, 0($t0)         	
        sw  $t8, 4($t0) 

        GoAhead: 
            addi $t2, $t2, 1                      
            addi $t0, $t0, 4   

        j InnerLoop 

    exitInnerLoop: 
        addi $t1, $t1, 1  
        j OuterLoop 
        
exit: 
*/







// TEST case to check PIPELINING

/*
-----------------(INPUT FILE)-----------------

li $t1, 25
li $t2, 25
bne $t1, $t2, loop  
li $t3, 3  
li $t4, 4  
li $t5, 5  
li $t6, 6   
li $t7, 7  
li $t8, 8

loop:
li $t9, 9  
lw $t7, 0($t9)

------------------( OUT PUT )-------------------

// After printing Register and Memory elements we get the following output for pipelining

**************** (PIPELINING - FORWARDING ) **********************

 CYCLES: 5    STALLS: 0  >>    li $t1, 25
 CYCLES: 6    STALLS: 0  >>    li $t2, 25
 CYCLES: 7    STALLS: 0  >>    bne $t1, $t2, loop
 CYCLES: 8    STALLS: 0  >>    li $t3, 3
 CYCLES: 9    STALLS: 0  >>    li $t4, 4
 CYCLES: 10    STALLS: 0  >>    li $t5, 5
 CYCLES: 11    STALLS: 0  >>    li $t6, 6   
 CYCLES: 12    STALLS: 0  >>    li $t7, 7
 CYCLES: 13    STALLS: 0  >>    li $t8, 8
 CYCLES: 14    STALLS: 0  >>    li $t9, 9
 CYCLES: 16    STALLS: 1  >>    lw $t7, 0($t9)

 Total Number of Cycles ( Forwarding ) : 16
 Total Number of STALLS ( Forwarding ) : 1


**************** (PIPELINING - NON FORWARDING ) **********************

 CYCLES: 5    STALLS: 0  >>    li $t1, 25
 CYCLES: 6    STALLS: 0  >>    li $t2, 25
 CYCLES: 10    STALLS: 3  >>    bne $t1, $t2, loop
 CYCLES: 11    STALLS: 3  >>    li $t3, 3  
 CYCLES: 12    STALLS: 3  >>    li $t4, 4
 CYCLES: 13    STALLS: 3  >>    li $t5, 5
 CYCLES: 14    STALLS: 3  >>    li $t6, 6
 CYCLES: 15    STALLS: 3  >>    li $t7, 7
 CYCLES: 16    STALLS: 3  >>    li $t8, 8
 CYCLES: 17    STALLS: 3  >>    li $t9, 9
 CYCLES: 20    STALLS: 5  >>    lw $t7, 0($t9)
 Total Number of CYCLES ( Non Forwarding ) : 20
 Total Number of STALLS ( Non Forwarding ) : 5


// In this way you can check which line is having how many stalls

*/
