#Torres de Hanoi 2.0

# Practica 1
# Luis Gustavo Velasco Hernandez

main:
	addi s0, zero, 15 # Numero de discos en s0
	lui s1, 0x10010 # Direccion base del arreglo
	addi t0, zero, 0 # Contador

llenarTorre:
	# Llenado del arreglo de 1 a s0
	bge t0, s0, termina # Si t0 >= s0 termina
	addi t1, t0, 1
	sw t1, 0(s1)
	addi s1, s1, 32
	addi t0, t0, 1
	jal llenarTorre

termina:	
	nop
	
	addi s1, s1, -32 # Retrocede el arreglo 32 bytes
	addi s2, s1, 4
	addi s3, s1, 8
	
	# Argumentos para la funcion de hanoi
	add a2, zero, s1
	add a3, zero, s2
	add a4, zero, s3
	add a5, zero, s4
	addi t1, zero, 1 # Valor para comparaciones

	jal hanoi
	jal end
	
# Caso base si s0 = 1
base_case:	
	# Se cambia el disco en el caso base
	lw t2, 0(a2)
	sw zero, 0(a2)
	sw t2, 0(a4)
	
	jalr ra
	
hanoi: 
	# Ir al caso base si s0 = 1
	beq s0, t1, base_case
	
	nop
	
	addi sp, sp, -20 # Decrementa el puntero 20 bytes para hacer espacio para las variables
	sw ra, 16(sp) # Guarda ra, s0, a2, a3 y a4 en la pila
	sw s0, 12(sp)
	sw a2, 8(sp)
	sw a3, 4(sp)
	sw a4, 0(sp)		
	
	# Preparacion de argumentos para llamada recursiva
	addi s0, s0, -1 # Decrementa s0 (n-1)
	add a5, zero, a3
	add a3, zero, a4
	add a4, zero, a5
	
	# Actualizacion de direcciones para los siguientes discos
	addi a2, a2, -32
	addi a3, a3, -32
	addi a4, a4, -32
	
	jal hanoi # Llamada recursiva hanoi(n-1, orig, auci, dest)
	
	# Restaura la pila
	lw a4, 0(sp)
	lw a3, 4(sp)
	lw a2, 8(sp)
	lw s0, 12(sp)
	lw ra, 16(sp)
	addi sp, sp, 20 # Incrementa el puntero de pila 20 bytes
	
	# Movimiento de disco
	lw t2, 0(a2)
	sw zero, 0(a2)
	sw t2, 0(a4)
	
	# Preparacion de argumentos para siguiente llamada recursiva
	addi sp, sp, -20
	sw ra, 16(sp)
	sw s0, 12(sp)
	sw a2, 8(sp)
	sw a3, 4(sp)
	sw a4, 0(sp)	
	
	addi s0, s0, -1
	add a5, zero, a2
	add a2, zero, a3
	add a3, zero, a5
	
	addi a2, a2, -32
	addi a3, a3, -32
	addi a4, a4, -32
	
	# Llamada recursiva hanoi(n-1, auxi, dest, orig)
	jal hanoi
	
	lw a4, 0(sp)
	lw a3, 4(sp)
	lw a2, 8(sp)
	lw s0, 12(sp)
	lw ra, 16(sp)
	addi sp, sp, 20
	
	jalr ra # Retorna
	
end: 
	nop
