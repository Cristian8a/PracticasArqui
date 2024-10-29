.data
    Arr1: .word 1, 2, 3, 4, 5, 6, 7, 8, 9
    Arr2: .word -1, 2, -3, 4, -5, 6, -7, 8, -9

.text

#int Vector_1[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
#int Vector_2[9] = { -1,2,-3,4,-5,6,-7,8,-9 };
#int i = 0;
#int resultado = 0;
#int Producto(int, int);
#int main(void) {
#	for (i = 0; i < 9; i++)
#	{
#		result = result + ProductFunction(Vector_1[i], Vector_2[i]);
#	}
#}
#int ProductFunction(int a, int b)
#{
#	return(a*b);
#}

main:
    # Inicialización de registros
    lui t0, %hi(Arr1)
    addi s0, t0, %lo(Arr1)   # s0 = dirección base de Arr1
    lui t1, %hi(Arr2)
    addi s1, t1, %lo(Arr2)   # s1 = dirección base de Arr2
    addi s2, zero, 0             # s2 = resultado = 0
    addi s3, zero, 0             # s3 = i = 0
    addi s4, zero, 9             # s4 = tamaño del vector (9)

for:
    beq s3, s4, end_for # Si i == 9, salir del bucle


    lw t2, 0(s0)                
    lw t3, 0(s1)                


    addi sp, sp, -12 # Decrementa el puntero de pila en 12 bytes para hacer espacio para 3 palabras.
    sw ra, 8(sp)                 
    sw t2, 4(sp)                 
    sw t3, 0(sp)                 
    jal ra, ProductFunction      
    lw ra, 8(sp)                 
    addi sp, sp, 12              


    add s2, s2, a0 # resultado += ProductFunction(Vector_1[i], Vector_2[i])

    addi s0, s0, 4              
    addi s1, s1, 4               
    addi s3, s3, 1 # i++

    # Repetir bucle
    jal zero, for

end_for:
	nop

ProductFunction:

    lw a0, 4(sp)                 
    lw a1, 0(sp)                 
    mul a0, a0, a1  # a0 = a * b
    jalr zero, ra, 0            
    

