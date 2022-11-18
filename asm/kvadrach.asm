push 10000
pop [0]                 ; accuracy
in
pop rax
in
pop rbx
in
pop rcx
push rax
push 0
je LINEAR
push rbx
push rbx
mul
push -1
push 4
push rax
push rcx
mul
mul
mul
add
pop rdx         ; D
push rdx
push 0
ja NO_ROOT
push rdx
push 0
je ONE_ROOT
push rdx
push 0
jb TWO_ROOTS
:LINEAR
push rbx
push rcx
push -1
mul
div
out
hlt
:NO_ROOT
hlt
:ONE_ROOT
push 2
push rax
mul
push rbx
push -1
mul
div
out
hlt
:TWO_ROOTS
push 2
push rax
push [0]
mul
mul
push rbx
push -1
push [0]
mul
mul
push rdx
asqrt
add
div
out
push 2
push rax
push [0]
mul
mul
push rbx
push -1
push [0]
mul
mul
push rdx
asqrt
push -1
mul
add
div
out
hlt