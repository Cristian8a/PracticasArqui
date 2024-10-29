#int Potencia(int m, int n);
#int main(void) {
#int potencia;
#potencia = Potencia(6, 6);
#return 0 ;
#}
#int Potencia(int m,int n){
#if (n < 1){
#return(1);
#}
#else {
#return(m*Potencia(m,n - 1));
#}
#}

.text
main: 	add a0, zero, zero
	addi a1, zero, 6  #m = 6
	addi a2, zero, 6  #n = 6 
	addi t0, zero, 1  #registro temporal para comparar con el #1
	jal ra, potencia  #llamada a la funcion
	jal zero, endCode  #salto para finalizar la funcion
	
	
potencia: 	blt a2, t0, if   # n < 1  saltar a if

else: 		#push -> ra, a2
		addi sp, sp, -4
		sw ra, 0(sp)
		addi sp, sp, -4
		sw a2, 0(sp)
		
		#modificar argumetos
		addi a2, a2, -1   #n --;
		
		#Llamada recursiva  potencia
		jal ra, potencia
		
		#Pop
		lw a2, 0(sp)
		addi sp, sp, 4
		lw ra, 0(sp)
		addi sp, sp, 4
		
		#Multiplicar n veces m
		mul a0, a0, a1
		
		#return
		jalr ra

if: 	addi a0, a0, 1 #asignar al return el 1
	jalr ra  #return
		
endCode: nop