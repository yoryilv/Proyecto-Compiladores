
file /home/yoryi/Desktop/compiladores/Proyecto-Compiladores/visualizador/temp/program
set pagination off
set width 0
break main
run

set $cnt = 0

# Bucle con límite de seguridad (Max 200 pasos)
while ($cnt < 200) 
  echo \n---STEP---\n
  echo Info registers:\n
  info registers rax rbx rcx rdx rsi rdi rsp rbp
  echo Stack dump:\n
  x/10xw $rsp
  
  # IMPORTANTE: nexti ejecuta la instrucción actual. 
  # Si es un CALL (como printf), lo pasa por encima sin entrar.
  nexti
  
  set $cnt = $cnt + 1
  
  # Truco: Si el Stack Pointer ($rsp) sube demasiado, significa que salimos del main.
  # (Esto es opcional, el límite de 200 suele bastar).
end
quit
