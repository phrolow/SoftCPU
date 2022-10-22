in
dup
pop rax                 ; copy to rax
call fact
out
hlt
:next                   ; next step
push rax
mul                     ; res *= n
:fact
push rax
push -1
add 
pop rax                 ; n--
push rax
push 1
jb next                 ; if(n > 1) next
ret