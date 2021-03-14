# Simulator
# CO PROJECT

This is a simple C++ file which supports assembly language written with following MIPS instructions (same syntax of those):

    ( add,addi,sub,subi,j,bne,slt,li,lw,sw,sll,beq,beqz,bnez,bge,ble ) 
        
We have used 20 Register variabes of integer type (Fixed as of now) assuming that we may require some other kind of stuff in further phases
and 1024 size Memory array of integer type (You can change it in code directly if you want to increase)

   Registers R[0],R[1].......R[9]  are indicated with t0,t1,t2,...t9
   
   Registers R[10],R[11].....R[19] are indicated with r0,r1,r2,...r9

By default the code will display the contents of 20 Registers and 20 Memory values (you can change it in code directly if you are using more values).

BubbleSort file (having name -'assembly') is also attached to check its working condition (You can notice the sorted order in Memory array).
        
        
TO AVOID ANY ERRORS(For our code) FOLLOW THESE CONDITIONS :

  -> Keep a space at the end of every singe line you use,
  -> You need to follow this format or syntax for all MIPS instructions:
         - add  $t0, $t1, $t2   (To be simple dont keep any other spaces between them i.e, between $, register variable and register number)
         - addi $t0, $t1, 10 
  -> file should not be empty,
  -> we have only 20 registers at present ( t0,t1...t9 and r0,r1,...r9 ) so be careful of size and names of registers,
  -> While using .word type extra ", ;" at the end of the line after giving inputs
         - .word 1, 3, -9, 5, 8, ;
  -> Dont use $zero , simpy use normal 0
  
 As of now we haven't applied any GUI, single step execution....
 
 ********* Thats it SIR ********
