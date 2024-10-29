# CalculatorRisc.asm
.text

main:
    # Inicialización de registros
    addi s0, zero, 1  # selector = 0 (s0)
    addi s1, zero, 2  # a = 5 (s1)
    addi s2, zero, 2  # b = 3 (s2)
    addi s3, zero, 0  # c = 0 (s3)

    beq s0, zero, default  # if (selector == 0) goto default
    addi t0, zero, 1
    beq s0, t0, case1      # if (selector == 1) goto case1
    addi t0, zero, 2
    beq s0, t0, case2      # if (selector == 2) goto case2
    addi t0, zero, 3
    beq s0, t0, case3      # if (selector == 3) goto case3
    j default              # else goto default

case1:
    # c = suma(a, b)
    add a0, s1, zero  # Carga a en a0
    add a1, s2, zero  # Carga b en a1
    jal ra, suma
    add s3, a0, zero  # Guarda el resultado en c (s3)
    j end_switch

case2:
    # c = resta(a, b)
    add a0, s1, zero  # Carga a en a0
    add a1, s2, zero  # Carga b en a1
    jal ra, resta
    add s3, a0, zero  # Guarda el resultado en c (s3)
    j end_switch

case3:
    # c = multiplica(a, b)
    add a0, s1, zero  # Carga a en a0
    add a1, s2, zero  # Carga b en a1
    jal ra, multiplica
    add s3, a0, zero  # Guarda el resultado en c (s3)
    j end_switch

default:
    # c = andBitwise(a, b)
    add a0, s1, zero  # Carga a en a0
    add a1, s2, zero  # Carga b en a1
    jal ra, andBitwise
    add s3, a0, zero  # Guarda el resultado en c (s3)

end_switch:
    # Fin del programa
    jal zero, endCode
    #addi a7, zero, 10 # Syscall para salir del programa
    #ecall

suma:
    # c = a + b
    add a0, a0, a1
    jalr zero, ra, 0  # Retorno de la función

resta:
    # c = a - b
    sub a0, a0, a1
    jalr zero, ra, 0  # Retorno de la función

multiplica:
    # c = a * b
    mul a0, a0, a1
    jalr zero, ra, 0  # Retorno de la función

andBitwise:
    # c = a & b
    and a0, a0, a1
    jalr zero, ra, 0  # Retorno de la función
    
endCode: nop
    