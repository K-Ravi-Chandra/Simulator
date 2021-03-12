# include <iostream>
# include <bits/stdc++.h>
# include <fstream>
# include <sstream> 
using namespace std;

int R[32];     // Global array of Registers
int Mem[1024]; // Global array of Memory -> Creating array of 4KB memory = 4 bytes x ( 1024 length )
int base_address;


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


bool PerformLI (char a, int x,int y)
{
    int p = y; // Storing the immediate integer to be stored

    // Simply load that into the register
    if(a == 't')
        R[x] = p;
    else if (a == 'r')
        R[10+x] = p;

    base_address = p;

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
    
    r = Mem[q];

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
            //temp = temp+":";
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
                return temp;   
            
        }
        else
            return wrong;
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


int main()
{
    int n = 10;      // file size
    string arr[n];   // Each one stores one line of file

    // Assigning values to registers
    int i=0;         
    for(int i=0;i<32;i++)
       R[i] = i;

    // Creating array of 4KB memory = 4 bytes x ( 1024 length )
    for (int i=0;i<1024;i++)
        Mem[i] = 2 * i;
   

    // Here we can clearly observe that this array holds fixed memory address in all cases
    // cout << Mem[0] << endl <<  &(Mem[0]) << endl << *(&(Mem[0])) << endl;
    // cout << Memory[1] << endl <<  &(Memory[1]) << endl;

    ////***********************************************//// 

    std :: string line;
    ifstream file("assembly.txt");

    if(!file.is_open())
        cout << "error" << endl;

    while (getline(file,line))
        arr[i++] = line;

    map <string,int> search;

    for(int i=0 ;i<n ; i++)
    {
        string temp  = arr[i];
        string label = search_label(temp);

        //cout << label;        
        //label = label + ":";  // Shoud not do this ,we need without colon only

        search.insert({label,i});
        //search.insert({"while",4});
    } 

    //   If input file is "I am a human:"
    //    search ["human"];   // Prints in which line it is there

    //****************************************************//


    for (int k=0;k<n;k++)
    {  
        // if(arr[k] == "")
        // cout << "* ";      // This will print only if a line dont have anything (not even space)
        //                       or in the case of  the last unused remaining lines

        if(arr[k] == "")
        k++;

        add_sub_Check  ( arr[k], "add" ) ;
        add_sub_Check  ( arr[k], "sub" ) ;
            
        addi_subi_Check( arr[k], "addi" ) ;
        addi_subi_Check( arr[k], "subi" ) ;

        li_Check ( arr[k], "li" );
        lw_Check ( arr[k], "lw" );
        sw_Check ( arr[k], "sw" );

        slt_Check( arr[k], "slt");


        // Checking 'J'
        string jumpLabel = jump_Check(arr[k], "j");   
        if( arr[k]!= ""  &&  jump_Check(arr[k], "j") != "fault" )
        {
            // jumpLabel = jumpLabel +":";     //should not do
            // cout << jumpLabel;
            // cout << search ["while"];       //Shows where while: is located
            
            // cout <<search [jumpLabel];
            k = search [jumpLabel];   //jump_Index;

            continue;
        }

        // Checking 'bne'
        string  bne_Label =  bne_Check (arr[k], "bne");
        if( arr[k]!= ""  &&  bne_Check(arr[k], "bne") != "fault" )
        {
            // # Similar to above Jump case
            k = search [bne_Label];   //jump_Index;

            continue;
        }

    }

    cout << "\n  Register Elements :" << "\n\n";
    PrintAllRegisters(R, 32);   // Printing 32 registers

    cout << "\n  Memory Elements :" << "\n\n";
    PrintAllMemory (Mem, 15);   // Printing required number of memory elements upto 1024


    file.close();
    return 0;
}

/*  EXCEPTIONS 
  
    -> should follow good pattern or spacing in add ,addi,.....
    -> file should not be empty
    -> each line should end with space
    -> we have 20 registers at present
    -> If u find any other include here
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