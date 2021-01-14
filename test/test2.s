
test.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <add>```````````````:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	89 7d fc             	mov    %edi,-0x4(%rbp)
   7:	89 75 f8             	mov    %esi,-0x8(%rbp)
   a:	8b 45 f8             	mov    -0x8(%rbp),%eax
   d:	8b 55 fc             	mov    -0x4(%rbp),%edx
  10:	01 d0                	add    %edx,%eax
  12:	5d                   	pop    %rbp
  13:	c3                   	retq   

0000000000000014 <main>:
  14:	55                   	push   %rbp
  15:	48 89 e5             	mov    %rsp,%rbp
  18:	48 83 ec 10          	sub    $0x10,%rsp
  1c:	be de 00 00 00       	mov    $0xde,%esi
  21:	bf 6f 00 00 00       	mov    $0x6f,%edi
  26:	e8 00 00 00 00       	callq  2b <main+0x17>
  2b:	89 45 fc             	mov    %eax,-0x4(%rbp)
  2e:	c9                   	leaveq 
  2f:	c3                   	retq   
